#ifndef MY_CONFIG_H
#define MY_CONFIG_H
#include <stdint.h>

#define PWM_65_1 3
#define PWM_65_2 5
#define PWM_65_3 6
#define INT_1S 7
#define PWM_18_1 9
#define PWM_18_2 10
#define PWM_MAINS 11
#define ONE_WIRE_BUS 13
#define BUTTON_1 0
#define BUTTON_2 1

enum class MessageType : uint16_t {
    None,Button,Serial,OneHz,Command
};

#define TEST_PWM 6

#endif