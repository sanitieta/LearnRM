//
// Created by xuhao on 2025/10/2.
//
#include "tim.h"
#include "usart.h"
#include <cmath>
extern uint8_t rx_msg[4];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim1) {
    uint32_t arr_value = TIM1->ARR + 1;
    uint32_t brightness;
    brightness = (uint32_t)arr_value * sinf(4 * HAL_GetTick() / 1000.0f) - 1;
    TIM1->CCR2 = brightness;
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart7) {
    if (rx_msg[0] == 'R') {
      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_RESET);
    }else if (rx_msg[0] == 'M') {
      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);
    }
    HAL_UART_Receive_IT(&huart7, rx_msg, 1);
  }
}