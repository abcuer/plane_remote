#include "remote.h"
#include "beep.h"
#include "led.h"
#include "nrf24l01.h"
#include "memory.h"

// 遥控器的数据结构
RC_Data_Struct tx_data = {0}; 
RC_Frame_Struct tx_frame;   // 发送的数据帧
/**
 * @brief 计算数据帧的校验和
 * @param frame 数据帧指针
 * @return 计算得到的校验和
 */
static uint8_t Calculate_Checksum(RC_Frame_Struct *frame)
{
    uint8_t checksum = 0;
    uint8_t *data = (uint8_t *)frame;
    // 遍历长度为 sizeof(RC_Frame_Struct) - 1
    for (int i = 0; i < sizeof(RC_Frame_Struct) - 1; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

/**
 * @brief 打包遥控数据到数据帧
 * @param frame 输出数据帧
 * @param tx_data 输入遥控数据
 */
static void Pack_Remote_Data(RC_Frame_Struct *frame, RC_Data_Struct *tx_data)
{
    memset(frame, 0, sizeof(RC_Frame_Struct));
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
 * @brief 遥控器数据发送任务
 * @note 建议每 20ms 调用一次
 */
void Remote_Send_Task(void)
{
    // 1. 更新数据 (这里假设你已经有函数获取了摇杆的 ADC 值并转化为了 1000-2000)
    // 示例：获取摇杆数据（需根据你的硬件实现）
    // remote_data.THR = Get_Joystick_Value(CH_THR); 
    // remote_data.ROL = Get_Joystick_Value(CH_ROL);
    // ...以此类推
    tx_data.THR = 1090;
    Limit(tx_data.THR, 1000, 2000);
    Limit(tx_data.PIT, 1000, 2000);
    Limit(tx_data.ROL, 1000, 2000);
    Limit(tx_data.YAW, 1000, 2000);
    // 2. 数据打包
    // 函数内部会自动清零 tx_frame 并填充帧头、通道数据及计算校验和
    Pack_Remote_Data(&tx_frame, &tx_data);
    
    // 3. 触发无线发送
    // NRF24L01_TxPacket 会负责将 tx_frame 发送出去并等待接收端的 ACK
    uint8_t result = NRF24L01_TxPacket((uint8_t*)&tx_frame);
    
    // 4. 状态指示
    if (result == 0) // 发送成功且收到应答
    {
        tx_data.CONNECT = 1;
        tx_data.NRF_ERR = 0;
        SetLedMode(rLED_UP, LED_TOGGLE);     // 上灯亮表示连接正常
        SetLedMode(rLED_DOWN, LED_OFF);
    }
    else // 发送失败，自动重连
    {
        tx_data.CONNECT = 0;
        tx_data.NRF_ERR++;
        if (tx_data.NRF_ERR > 50) { // 连续失败 50 次
            NRF24L01_TX_Mode(); // 重新初始化寄存器
            tx_data.NRF_ERR = 0;
        }
        SetLedMode(rLED_UP, LED_OFF);
        SetLedMode(rLED_DOWN, LED_TOGGLE); // 红灯/下灯闪烁表示断开
    }
}
