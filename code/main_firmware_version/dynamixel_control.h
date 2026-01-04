#pragma once
#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include <config.h>



// Export the dxl object if you really need it in other files,
// but try to prefer using the functions instead.
extern Dynamixel2Arduino dxl;

enum GripState {
    GRIP_OPEN,      // Initial open gripper position
    GRIP_CLOSING,   // going to initial grip position
    GRIP_TIGHTEN,   // going to tighter position after detection
    GRIP_HOLDING,       // just holding
    GRIP_RELEASING  // Going back to open position
};

struct DxlState {
    int32_t current[DXL_ID_CNT];
    int32_t position[DXL_ID_CNT];
    int32_t pwm[DXL_ID_CNT];
    int32_t velocity[DXL_ID_CNT];
};

void dxl_init();


bool dxl_read_feedback_block(uint8_t id, uint8_t idx, DxlState &out);


// Read current & position into a struct
DxlState dxl_read_state();

void dxl_filter_current(DxlState& st);

bool dxl_motors_timeout();

// helper for syncWrite positions
void dxl_set_goal_values(int32_t goalFlexor, int32_t goalExtensor);

void dxl_update_state(const DxlState& st, bool grip);

GripState dxl_get_grip_state();