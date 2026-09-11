#pragma once
#include <Arduino.h>
#include <config.h>



// -------------------- Internal data --------------------
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  extern SoftwareSerial soft_serial;
  #define DXL_SERIAL   Serial
  #define DEBUG_SERIAL soft_serial
#else
  #define DXL_SERIAL   Serial1
  #define DEBUG_SERIAL Serial
#endif