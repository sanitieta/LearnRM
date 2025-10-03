//
// Created by xuhao on 2025/10/2.
//
#include "tim.h"
#include <cmath>
#include "motor.h"
#include "can.h"
CAN_RxHeaderTypeDef rx_header;
uint8_t rx_data[8];
M3508Motor motor1(1, 3591.0f / 187);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim == &htim1) {
        uint32_t arr_value = TIM1->ARR + 1;
        uint32_t brightness = (uint32_t)arr_value * sinf(4 * HAL_GetTick() / 1000.0f) - 1;
        TIM1->CCR2 = brightness;
    } else if (htim == &htim6) {
        // float motor_current = 1.0f * sinf( HAL_GetTick() / 1000.0f);
        motor1.set_current(0.25f);
        // do something
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    if (hcan == &hcan1) {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK) {
            if (rx_header.StdId == 0x201)
                motor1.can_rx_msg_callback(rx_data);
        }
    }
}