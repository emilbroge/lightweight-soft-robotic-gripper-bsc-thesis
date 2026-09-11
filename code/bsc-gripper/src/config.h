// hvordan kan det vlre at det fucker altid hele tiden? - Asbjærb Schroll Graae

#pragma once
#include <Arduino.h>// -------------------- General config --------------------


#define MAX_CURRENT          300
#define MAX_PWM 600
#define GRIPPING_PERIOD_MS   1000000
#define SAMPLING_PERIOD_MS   100 // If using load cell this needs to be 100ms.

#define DXL_VELOCITY_DEADBAND 5
#define CONTROL_CLOSING_SLOPE_FLEXOR 20

// Control
#define OPEN_FLEX_SLOPE 0
#define OPEN_EXTE_SLOPE 0
#define OPEN_FLEX_LIMIT -5
#define OPEN_EXTE_LIMIT 150

#define CLOSING_TIME 1000 // ms
#define CLOSING_FLEX_SLOPE 30
#define CLOSING_EXTE_SLOPE 0
#define CLOSING_FLEX_LIMIT 500
#define CLOSING_EXTE_LIMIT -200

#define TIGHTEN_TIME 500 // ms
#define TIGHTEN_FLEX_SLOPE 10
#define TIGHTEN_EXTE_SLOPE 10
#define TIGHTEN_FLEX_LIMIT 600
#define TIGHTEN_EXTE_LIMIT -20

#define HOLDING_TIME 0 // ms
#define HOLDING_FLEX_SLOPE 0
#define HOLDING_EXTE_SLOPE 0
#define HOLDING_FLEX_LIMIT 250
#define HOLDING_EXTE_LIMIT -5

#define RELEASING_TIME 0 // ms
#define RELEASING_FLEX_SLOPE 0
#define RELEASING_EXTE_SLOPE 0
#define RELEASING_FLEX_LIMIT -200
#define RELEASING_EXTE_LIMIT 600

#define CURRENT_DEADBAND 50
#define MAX_ACTUATION_CONTROL 600

#define BTN_DEBOUNCE 300 // ms

#define RUNNING_AVG 1


// -------------------- Load cell pins --------------------
#define ABC_LOAD_CELL_CLK   5
#define C_LOAD_CELL_DT     6

#define N_SCALES 1

#define C_LOAD_CELL_SCALE 223.12


// -------------------- Scope pins ------------------------
#define SCOPE_PIN_A 9
#define SCOPE_PIN_B 10

#define BTN_PIN 4

// -------------------- Dynamixel config ------------------


#define DXL_SHIELD_UART_RX 7
#define DXL_SHIELD_UART_TX 8
#define DXL_DIR_PIN 2


const float DXL_PROTOCOL_VERSION = 2.0f;


const bool debugLoadCells = false;   // or false


const uint8_t DXL_ID_EXTENSOR = 8;
const uint8_t DXL_ID_FLEXOR   = 9;

const uint8_t IDX_EXTENSOR = 0;
const uint8_t IDX_FLEXOR   = 1;

const uint8_t BROADCAST_ID = 254;
const uint8_t DXL_ID_CNT   = 2;
const uint8_t DXL_ID_LIST[DXL_ID_CNT] = {DXL_ID_EXTENSOR, DXL_ID_FLEXOR};

// SyncWrite (Goal Position)
const uint16_t SW_START_ADDR = 100; // 100 = Goal PWM, 102 = Goal Current, 116 = Goal Positiion
const uint16_t SW_ADDR_LEN   = 2; // PWM and Current -> 2, Position -> 4
const uint16_t USER_PKT_BUF_CAP = 128;

// XL330 feedback block
constexpr uint16_t XL330_ADDR_PRESENT_PWM      = 124;
constexpr uint16_t XL330_BLOCK_LEN          = 12;  // PWM+Current+Vel+Pos
