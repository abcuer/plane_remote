#include "headfile.h"

void ShowData(void)
{
    char buf[20];
    
    u8g2_FirstPage(&u8g2);
    do {
        // --- 1. 顶部状态栏 (高度约 12 像素) ---
        u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
        
        // 遥控器电压 (V)
        sprintf(buf, "R:%2.1fV", (float)stick.BAT / 100.0f);
        u8g2_DrawStr(&u8g2, 0, 10, buf);
        
        // 飞控端电压 (F)
        sprintf(buf, "F:%2.1fV", (float)rx_data.fly_volt / 100.0f);
        u8g2_DrawStr(&u8g2, 50, 10, buf);
        
        // 连接与锁定状态
        if(!tx_data.CONNECT) {
            u8g2_DrawStr(&u8g2, 95, 10, "DIS!"); 
        } else {
            u8g2_DrawStr(&u8g2, 95, 10, tx_data.LOCK_KEY ? "LOCK" : "ACTV");
        }
        
        u8g2_DrawHLine(&u8g2, 0, 12, 128); // 分割线

        // --- 2. 左侧：遥控器发送数据 (TX) ---
        u8g2_SetFont(&u8g2, u8g2_font_5x8_tr);
        u8g2_DrawStr(&u8g2, 0, 22, "[TX STICK]");
        
        sprintf(buf, "THR:%4d", tx_data.THR); u8g2_DrawStr(&u8g2, 0, 32, buf);
        sprintf(buf, "YAW:%4d", tx_data.YAW); u8g2_DrawStr(&u8g2, 0, 42, buf);
        sprintf(buf, "PIT:%4d", tx_data.PIT); u8g2_DrawStr(&u8g2, 0, 52, buf);
        sprintf(buf, "ROL:%4d", tx_data.ROL); u8g2_DrawStr(&u8g2, 0, 62, buf);

        u8g2_DrawVLine(&u8g2, 62, 13, 51); // 垂直分割线

        // --- 3. 右侧：飞控回传数据 (RX) ---
        u8g2_DrawStr(&u8g2, 66, 22, "[RX ATTIT]");
        
        // 姿态角
        sprintf(buf, "P:%4.1f R:%4.1f", rx_data.pitch, rx_data.roll);
        u8g2_DrawStr(&u8g2, 66, 32, buf);
        sprintf(buf, "YAW:%5.1f", rx_data.yaw);
        u8g2_DrawStr(&u8g2, 66, 40, buf);

        // PID参数显示 (分两行)
        u8g2_DrawStr(&u8g2, 66, 50, "[PID]");
        sprintf(buf, "P:%.2f I:%.2f", rx_data.pid_kp, rx_data.pid_ki);
        u8g2_DrawStr(&u8g2, 66, 58, buf);
        sprintf(buf, "D:%.2f", rx_data.pid_kd);
        u8g2_DrawStr(&u8g2, 66, 64, buf);

    } while (u8g2_NextPage(&u8g2));
}