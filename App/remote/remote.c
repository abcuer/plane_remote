#include "remote.h"
#include "headfile.h"
#include "led.h"
#include "memory.h"
#include "nrf24l01.h"

// 遥控器的数据结构
Remote_Data_Struct tx_data = {
    .LOCK_KEY = 1,
}; 
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
static void Remote_PackData(RC_Frame_Struct *frame, Remote_Data_Struct *tx_data)
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

static uint8_t success_cnt = 0;
static uint8_t fail_cnt = 0;

void Remote_SendData(void)
{
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
        // if(tx_data.CONNECT == 1)    return;
        success_cnt++;
        fail_cnt = 0;
        if(success_cnt >= 2)  
        {
            tx_data.CONNECT = 1;
            tx_data.NRF_ERR = 0;
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
                NRF24L01_TX_Mode();     // 重新连接
                tx_data.NRF_ERR = 0;
            }
        }
    }
}
