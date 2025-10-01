#include "tim.h"
#include <cmath>
//
// Created by xuhao on 2025/10/2.
//
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim1) {
    uint32_t arr_value = TIM1->ARR + 1;
    uint32_t brightness;
    brightness = (uint32_t)arr_value * sinf(4 * HAL_GetTick() / 1000.0f) - 1;
    TIM1->CCR2 = brightness;
  }
}