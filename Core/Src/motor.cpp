//
// Created by xuhao on 2025/10/2.
//
#include "../Inc/motor.h"
#include "can.h"

uint8_t M3508Motor::tx_data_1_[8] = { 0 };
uint8_t M3508Motor::tx_data_2_[8] = { 0 };

M3508Motor::M3508Motor(uint8_t escid, float ratio):
    escid_(escid),
    ratio_(ratio) {
    if (1 <= escid && escid <= 4) {
        motor_tx_header_.StdId = TX_ID_1_4;
    } else if (5 <= escid && escid <= 8) {
        motor_tx_header_.StdId = TX_ID_5_8;
    }
    motor_tx_header_.DLC = 8;
    motor_tx_header_.IDE = CAN_ID_STD;
    motor_tx_header_.RTR = CAN_RTR_DATA;
    motor_tx_header_.TransmitGlobalTime = DISABLE;
}

void M3508Motor::can_rx_msg_callback(const uint8_t rxdata[8]) {
    int16_t tmp = (rxdata[0] << 8) | rxdata[1];

    last_ecd_angle_ = ecd_angle_;
    ecd_angle_ = linear_mapping(tmp, 0, 8191, 0.0f, 360.0f);
    float raw_delta_ecd_angle = ecd_angle_ - last_ecd_angle_;
    if (raw_delta_ecd_angle > 180.0f) // 解决临界跳变
        raw_delta_ecd_angle -= 360.0f;
    else if (raw_delta_ecd_angle < -180.0f)
        raw_delta_ecd_angle += 360.0f;
    delta_ecd_angle_ = raw_delta_ecd_angle;
    delta_angle_ = delta_ecd_angle_ / ratio_;
    angle_ += delta_angle_;

    tmp = (rxdata[2] << 8) | rxdata[3];
    rotate_speed_ = static_cast<float>(tmp);

    tmp = (rxdata[4] << 8) | rxdata[5];
    current_ = linear_mapping(tmp, -16384, 16384, -20.0f, 20.0f);

    temp_ = static_cast<float>(rxdata[6]);
}

void M3508Motor::set_current(float current) {
    if (current > MAX_CURRENT)
        current = MAX_CURRENT;
    else if (current < -MAX_CURRENT)
        current = -MAX_CURRENT;

    int16_t current_cmd = static_cast<int16_t>(current * 16384.0f / 20.0f);

    tx_data_1_[2 * escid_ - 2] = current_cmd >> 8;
    tx_data_1_[2 * escid_ - 1] = current_cmd & 0xff;
    HAL_CAN_AddTxMessage(&hcan1, &motor_tx_header_, tx_data_1_, nullptr);
}

float M3508Motor::linear_mapping(int in, int in_min, int in_max, float out_min, float out_max) {
    return (in - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}