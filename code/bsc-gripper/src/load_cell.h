#pragma once
#include <Arduino.h>
#include <HX711.h>
#include <config.h>

struct LoadCellState {
    int32_t load[N_SCALES];
};

void load_cell_init();
LoadCellState load_cell_read();
