#include <dynamixel_control.h>
#include <config.h>
#include <serial_config.h>

// This namespace is required to use Control table item names
using namespace ControlTableItem;


Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

static uint8_t user_pkt_buf[USER_PKT_BUF_CAP];

typedef struct sw_data {
  int32_t goal_value;
} __attribute__((packed)) sw_data_t;

static sw_data_t sw_data[DXL_ID_CNT];
static DYNAMIXEL::InfoSyncWriteInst_t sw_infos;
static DYNAMIXEL::XELInfoSyncWrite_t info_xels_sw[DXL_ID_CNT];

static GripState s_grip_state = GRIP_RELEASING;

static int32_t s_control[DXL_ID_CNT] = {0,0};


static int32_t current_avg[RUNNING_AVG][DXL_ID_CNT];
static int8_t s_avg_index = 0;


void dxl_init() {

    dxl.begin(1000000);
    dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

    // Wait for motor power
    while (!dxl.ping(DXL_ID_FLEXOR) || !dxl.ping(DXL_ID_EXTENSOR)) {
        // DEBUG_SERIAL.println("Motors not reacting to ping...");
        delay(1000);
    }
    // DEBUG_SERIAL.println("Ping succesfull!");

    // Initial setup
    for (uint8_t i = 0; i < DXL_ID_CNT; i++) {
        dxl.torqueOff(DXL_ID_LIST[i]);
        dxl.setOperatingMode(DXL_ID_LIST[i], OP_PWM);
        dxl.writeControlTableItem(CURRENT_LIMIT, DXL_ID_LIST[i], MAX_CURRENT);
        dxl.writeControlTableItem(PWM_LIMIT, DXL_ID_LIST[i], MAX_PWM);
        dxl.writeControlTableItem(HOMING_OFFSET, DXL_ID_LIST[i], 0);
        dxl.writeControlTableItem(TEMPERATURE_LIMIT, DXL_ID_LIST[i], 60);
    }

    s_grip_state = GRIP_RELEASING;

    dxl.torqueOn(BROADCAST_ID);

    // Setup SyncWrite
    sw_infos.packet.p_buf = user_pkt_buf;
    sw_infos.packet.buf_capacity = USER_PKT_BUF_CAP;
    sw_infos.packet.is_completed = false;
    sw_infos.addr = SW_START_ADDR;
    sw_infos.addr_length = SW_ADDR_LEN;
    sw_infos.p_xels = info_xels_sw;
    sw_infos.xel_count = 0;

    for (uint8_t i = 0; i < DXL_ID_CNT; i++) {
        info_xels_sw[i].id = DXL_ID_LIST[i];
        info_xels_sw[i].p_data = (uint8_t*)&sw_data[i].goal_value;
        sw_infos.xel_count++;
    }
    sw_infos.is_info_changed = true;
}


void dxl_set_goal_values(int32_t goalFlexor, int32_t goalExtensor) {
    sw_data[IDX_FLEXOR].goal_value   = goalFlexor;
    sw_data[IDX_EXTENSOR].goal_value = goalExtensor;
    sw_infos.is_info_changed = true;
    dxl.syncWrite(&sw_infos);
}


bool dxl_read_feedback_block(uint8_t id, uint8_t idx, DxlState &out) {
    uint8_t buf[XL330_BLOCK_LEN];

    int recv_len = dxl.read(id,
                            XL330_ADDR_PRESENT_PWM,
                            XL330_BLOCK_LEN,
                            buf,
                            XL330_BLOCK_LEN
                            );

    if (recv_len != XL330_BLOCK_LEN) {
        // read failed or timed out
        return false;
    }

    auto get16 = [&](uint8_t offset) -> int16_t {
        return (int16_t)( (uint16_t)buf[offset] |
                          ((uint16_t)buf[offset + 1] << 8) );
    };

    auto get32 = [&](uint8_t offset) -> int32_t {
        return (int32_t)( (uint32_t)buf[offset] |
                          ((uint32_t)buf[offset + 1] << 8) |
                          ((uint32_t)buf[offset + 2] << 16) |
                          ((uint32_t)buf[offset + 3] << 24) );
    };

    // Layout in the 12-byte block:
    // 0–1: PWM (124–125)
    // 2–3: Current (126–127)
    // 4–7: Velocity (128–131)
    // 8–11: Position (132–135)

    out.pwm[idx]      = get16(0);
    out.current[idx]  = get16(2);
    out.velocity[idx] = get32(4);
    out.position[idx] = get32(8);

    return true;
}

DxlState dxl_read_state() {
    DxlState st;
    // flexor (index 0)
    dxl_read_feedback_block(DXL_ID_FLEXOR, IDX_FLEXOR, st);

    // extensor (index 1)
    dxl_read_feedback_block(DXL_ID_EXTENSOR, IDX_EXTENSOR, st);


    return st;                                          
}

void dxl_filter_current(DxlState& st) {
    for (uint8_t i = 0; i < DXL_ID_CNT; i++) {
        current_avg[s_avg_index][i] = st.current[i];
    }

    s_avg_index = (s_avg_index + 1) % RUNNING_AVG;

    for (uint8_t i = 0; i < DXL_ID_CNT; i++) {
        int32_t sum = 0;
        for (uint8_t j = 0; j < RUNNING_AVG; j++) {
            sum += current_avg[j][i];
        }
        st.current[i] = sum / 5;
    }
}


bool dxl_motors_timeout() {
    auto err = dxl.getLastLibErrCode();
    return (err == DXL_LIB_ERROR_TIMEOUT);
}


void dxl_update_state(const DxlState& st, bool grip) {
    switch (s_grip_state)
    {
    case GRIP_OPEN:
        /* code */
        


        if (grip) {
            s_grip_state = GRIP_CLOSING;
        }

        s_control[IDX_FLEXOR] = OPEN_FLEX_LIMIT;
        s_control[IDX_EXTENSOR] = OPEN_EXTE_LIMIT; 


        break;
    
    case GRIP_CLOSING:
        /* code */

        if (st.pwm[IDX_FLEXOR] >= CLOSING_FLEX_LIMIT*0.99 and st.pwm[IDX_EXTENSOR] <= CLOSING_EXTE_LIMIT*0.99) {
            s_grip_state = GRIP_TIGHTEN;
            break;
        }


        s_control[IDX_FLEXOR] += CLOSING_FLEX_SLOPE;
        s_control[IDX_EXTENSOR] = CLOSING_EXTE_LIMIT; 
        
        /*
        if (abs(st.velocity[IDX_FLEXOR]) < DXL_VELOCITY_DEADBAND and abs(st.current[IDX_FLEXOR]) > CURRENT_DEADBAND) {
            s_grip_state = GRIP_HOLDING;
        }
        */

        break;

    case GRIP_TIGHTEN: // 
        /* code */

        if (st.pwm[IDX_FLEXOR] >= TIGHTEN_FLEX_LIMIT*0.99 and st.pwm[IDX_EXTENSOR] >= TIGHTEN_EXTE_LIMIT*0.99) {
            s_grip_state = GRIP_HOLDING;
            break;
        }


        s_control[IDX_FLEXOR] += TIGHTEN_FLEX_SLOPE;
        s_control[IDX_EXTENSOR] += TIGHTEN_EXTE_SLOPE; 


        /*
        if (abs(st.velocity[IDX_FLEXOR]) < 5) {
            s_grip_state = GRIP_HOLDING;
        }
        */

        break;
    
    case GRIP_HOLDING:
        /* code */
        s_control[IDX_FLEXOR] = HOLDING_FLEX_LIMIT;
        s_control[IDX_EXTENSOR] = HOLDING_EXTE_LIMIT;

        
        if (!grip) {
            s_grip_state = GRIP_RELEASING;
        }

        break;
    

    case GRIP_RELEASING:
        /* code */

        
        if (abs(st.velocity[IDX_EXTENSOR]) < DXL_VELOCITY_DEADBAND and abs(st.pwm[IDX_EXTENSOR]) > MAX_ACTUATION_CONTROL*0.75) {
            s_grip_state = GRIP_OPEN;
        } 

        s_control[IDX_FLEXOR] = RELEASING_FLEX_LIMIT;
        s_control[IDX_EXTENSOR] = RELEASING_EXTE_LIMIT;



        break;
    
    default:
        s_grip_state = GRIP_RELEASING;
        break;
    }

    // Send values to the motors
    dxl_set_goal_values(s_control[IDX_FLEXOR], s_control[IDX_EXTENSOR]);

}

GripState dxl_get_grip_state() {
    return s_grip_state;
     

}
