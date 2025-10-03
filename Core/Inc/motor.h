//
// Created by xuhao on 2025/10/2.
//

#ifndef M3508_MOTOR_H
#define M3508_MOTOR_H

#include <cstdint>
#include "can.h"

class M3508Motor {
public:
    M3508Motor(uint8_t escid, float ratio);
    void can_rx_msg_callback(const uint8_t rxdata[8]);
    void set_current(float current);
    static float linear_mapping(int in, int in_min, int in_max, float out_min, float out_max);
    // 获取电机状态信息
    float get_angle() const {
        return angle_;
    }

    float get_rotate_speed() const {
        return rotate_speed_;
    }

    float get_current() const {
        return current_;
    }

    float get_temperature() const {
        return temp_;
    }

    float get_delta_angle() const {
        return delta_angle_;
    }

    uint8_t get_esc_id() const {
        return escid_;
    }

    float get_ratio() const {
        return ratio_;
    }

private:
    // 电机参数
    const uint8_t escid_; // 电调号（1-8）
    const float ratio_; // 减速比

    // 电机状态
    float angle_ = 0.0f; // 累计角度（考虑减速比）
    float ecd_angle_ = 0.0f; // 原始编码器角度（0-360度）
    float last_ecd_angle_ = 0.0f; // 上一次编码器角度
    float delta_angle_ = 0.0f; // 角度变化量
    float delta_ecd_angle_ = 0.0f; // 原始编码器角度变化量
    float rotate_speed_ = 0.0f; // 转速
    float current_ = 0.0f; // 电流
    float temp_ = 0.0f; // 温度

    // CAN配置
    CAN_TxHeaderTypeDef motor_tx_header_; // CAN发送头
    static constexpr uint16_t TX_ID_1_4 = 0x200; // 1-4号电调发送标识符
    static constexpr uint16_t TX_ID_5_8 = 0x1FF; // 5-8号电调发送标识符
    // 电机限制参数
    static constexpr float MAX_CURRENT = 1.0f; // 最大电流限制
    static constexpr float MAX_SPEED = 10000.0f; // 最大转速限制

    static uint8_t tx_data_1_[8];
    static uint8_t tx_data_2_[8];
};

#endif // M3508_MOTOR_H