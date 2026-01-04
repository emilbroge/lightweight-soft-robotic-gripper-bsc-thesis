#include <load_cell.h>
#include <serial_config.h>
#include <config.h>

static HX711 cell0;
static HX711 scales[N_SCALES] = { cell0 };

static const uint8_t dataPin[N_SCALES] = { C_LOAD_CELL_DT };
static const uint8_t clockPin = ABC_LOAD_CELL_CLK;

static float calib[N_SCALES] = {
    C_LOAD_CELL_SCALE
};

void load_cell_init() {
    // DEBUG_SERIAL.print("HX711_LIB_VERSION: ");
    // DEBUG_SERIAL.println(HX711_LIB_VERSION);

    for (int i = 0; i < N_SCALES; i++) {
        scales[i].begin(dataPin[i], clockPin);
        scales[i].set_scale(calib[i]);
        scales[i].tare();
        scales[i].set_raw_mode();
    }
}

LoadCellState load_cell_read() {
    LoadCellState st;

    // Wait until both HX711 have done their conversion
    /*
    while (digitalRead(A_LOAD_CELL_DT) == HIGH ||
           digitalRead(B_LOAD_CELL_DT) == HIGH) {
        yield();
    }
    */

    for (int i = 0; i < N_SCALES; i++) {
        st.load[i] = scales[i].get_units();
    }
    return st;
}
