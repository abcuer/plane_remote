#include "headfile.h"

void ShowData(void)
{
    const char *labels[] = {"T", "Y", "P", "R"};
    int32_t values[] = {tx_data.THR, tx_data.YAW, tx_data.PIT, tx_data.ROL};
    
    u8g2_FirstPage(&u8g2);
    do {
        // --- 1. 左侧四轴数据 ---
        u8g2_SetFont(&u8g2, u8g2_font_6x12_tr); 
        
        for(uint8_t i = 0; i < 4; i++) 
        {
            uint8_t y_pos = 14 + (i * 15); // 调整间距更紧凑一点

            // 显示标签
            u8g2_DrawStr(&u8g2, 0, y_pos, labels[i]);
            
            // --- 进度条优化 ---
            // 将宽度从 50 缩短为 30
            uint8_t bar_x = 12;
            uint8_t bar_width = 30; 
            u8g2_DrawFrame(&u8g2, bar_x, y_pos - 8, bar_width, 7); 
            
            // 假设 values[i] 范围是 1000-2000 (标准 RC 信号)
            // 如果是 0-100，直接使用: (values[i] * (bar_width-2)) / 100
            int32_t fill_val = (values[i] - 1000) * (bar_width - 2) / 1000;
            if(fill_val < 0) fill_val = 0;
            if(fill_val > (bar_width - 2)) fill_val = bar_width - 2;
            
            u8g2_DrawBox(&u8g2, bar_x + 1, y_pos - 7, (uint8_t)fill_val, 5);
            
            // --- 数值显示位置左移 ---
            char buf[6];
            sprintf(buf, "%4ld", values[i]);
            u8g2_DrawStr(&u8g2, 45, y_pos, buf); // 起始位从 65 改为 45
        }

        // --- 2. 垂直分割线 (可选，增加视觉秩序) ---
        u8g2_DrawVLine(&u8g2, 78, 0, 64);

        // --- 3. 右侧系统信息 (空间变大了) ---
        // 电压
        u8g2_SetFont(&u8g2, u8g2_font_7x14_tr);
        char v_buf[10];
        sprintf(v_buf, "%.1fV", (float)stick.BAT / 100.0f);
        u8g2_DrawStr(&u8g2, 85, 25, v_buf); // 稍微左移一点

        // 锁定状态
        if(tx_data.LOCK_KEY > 0) {
            u8g2_DrawRBox(&u8g2, 85, 42, 38, 16, 2); // 使用带圆角的矩形更美观
            u8g2_SetDrawColor(&u8g2, 0);
            u8g2_DrawStr(&u8g2, 90, 55, "LOCK");
            u8g2_SetDrawColor(&u8g2, 1);
        } else {
            u8g2_DrawStr(&u8g2, 90, 55, "ACT");
        }

    } while (u8g2_NextPage(&u8g2));
}