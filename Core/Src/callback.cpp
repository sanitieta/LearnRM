//
// Created by xuhao on 2025/10/2.
//
#include "tim.h"
#include "usart.h"
#include <cmath>
extern uint8_t rx_msg[10];
extern uint8_t tx_msg[10];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (htim == &htim1) {
    uint32_t arr_value = TIM1->ARR + 1;
    uint32_t brightness;
    brightness = (uint32_t)arr_value * sinf(4 * HAL_GetTick() / 1000.0f) - 1;
    TIM1->CCR2 = brightness;
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
  if (huart == &huart7) {
    for (size_t i = 0; i < sizeof(rx_msg) / sizeof(rx_msg[0]); i++) {
      tx_msg[i] = rx_msg[i];
    }
    HAL_UART_Transmit_IT(&huart7, tx_msg, sizeof(tx_msg));
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
  if (huart == &huart7) {
    HAL_UART_Receive_IT(&huart7, rx_msg, sizeof(rx_msg));
  }
}