#include "remote.h"
#include "headfile.h"
#include "led.h"
#include "memory.h"
#include "nrf24l01.h"

// 遥控器的数据结构
TX_Data_Struct tx_data = {
    .LOCK_KEY = 1,
}; 
RX_Data_Struct rx_data = {0};

TX_Frame_Struct tx_frame;   // 发送的数据帧
RX_Frame_Struct rx_frame;   // 接受的数据帧
/**
 * @brief 计算数据帧的校验和
 * @param frame 数据帧指针
 * @return 计算得到的校验和
 */
static uint8_t Calculate_Checksum(TX_Frame_Struct *frame)
{
    uint8_t checksum = 0;
    uint8_t *data = (uint8_t *)frame;
    // 遍历长度为 sizeof(TX_Frame_Struct) - 1
    for (int i = 0; i < sizeof(TX_Frame_Struct) - 1; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

/**
 * @brief 打包遥控数据到数据帧
 * @param frame 输出数据帧
 * @param tx_data 输入遥控数据
 */
static void Remote_PackData(TX_Frame_Struct *frame, TX_Data_Struct *tx_data)
{
    memset(frame, 0, sizeof(TX_Frame_Struct));
    // 设置帧头
    frame->header[0] = FRAME0;
    frame->header[1] = FRAME1;
    frame->header[2] = FRAME2;
    
    // 填充通道数据
    frame->THR = tx_data->THR;
    frame->YAW = tx_data->YAW;
    frame->PIT = tx_data->PIT;
    frame->ROL = tx_data->ROL;
    frame->FIX_HEIGHT = tx_data->FIX_HEIGHT;
    frame->LOCK_KEY = tx_data->LOCK_KEY;
    
    // 计算校验和
    frame->checksum = Calculate_Checksum(frame);
}

/**
 * @brief 解析飞控回传的数据包
 * @param buf: 接收到的原始数据缓冲区
 */
void Remote_ParseData(uint8_t *buf)
{
    RX_Frame_Struct *pFrame = (RX_Frame_Struct *)buf;

    // 1. 校验帧头
    if (pFrame->header[0] != FRAME0 
        || pFrame->header[1] != FRAME1 
        || pFrame->header[2] != FRAME2
    ) {
        return;
    }

    // 2. 校验和计算 (逻辑参考 remote.c 中的 Calculate_Checksum)
    uint8_t sum = 0;
    for (int i = 0; i < sizeof(RX_Frame_Struct) - 1; i++) {
        sum ^= buf[i];
    }

    if (sum == pFrame->checksum) {
        // 3. 校验通过，更新数据
        rx_frame = *pFrame;
        // 更新接受到的数据，供 OLED 显示
        rx_data.pitch = rx_frame.pitch;
        rx_data.roll = rx_frame.roll;
        rx_data.yaw = rx_frame.yaw;
        rx_data.fly_volt = rx_frame.voltage;
        rx_data.pid_kd = rx_frame.pid_kd;
        rx_data.pid_ki = rx_frame.pid_ki;
        rx_data.pid_kp = rx_frame.pid_kp;
    }
}

void Remote_SendData(void)
{
    static uint8_t success_cnt = 0;
    static uint8_t fail_cnt = 0;
    uint8_t rx_back_buf[32]; // 用于存放回传数据的临时缓冲区

    // 更新数据
    Stick_Scan();
    Update_TX_Data();
    // 打包数据
    Remote_PackData(&tx_frame, &tx_data);
    // 发送数据
    uint8_t result = NRF24L01_TxPacket((uint8_t*)&tx_frame);
    // 状态指示
    if (result == 0)
    {
        success_cnt++;
        fail_cnt = 0;
        if(success_cnt >= 2)  
        {
            tx_data.CONNECT = 1;
            tx_data.NRF_ERR = 0;
        }
        // 切换接收模式
        uint8_t status = NRF24L01_Read_Reg(STATUS);
        if (status & RX_OK) 
        {
            NRF24L01_Read_Buf(RD_RX_PLOAD, rx_back_buf, 32); // 读取数据
            NRF24L01_Write_Reg(SPI_WRITE_REG + STATUS, status); // 清除标志位
            Remote_ParseData(rx_back_buf); // 调用解析函数
        }
    }
    else
    {
        fail_cnt++;
        success_cnt = 0;
        if(fail_cnt >= 10) 
        {
            tx_data.CONNECT = 0;
            tx_data.NRF_ERR++;
            if (tx_data.NRF_ERR > 50) { // 连续失败 50 次
                NRF_Init(MODEL_TX2, CONNECT_CHANNAL);     // 重新连接
                tx_data.NRF_ERR = 0;
            }
        }
    }
}
