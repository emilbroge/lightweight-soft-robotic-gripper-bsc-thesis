#include <serial_config.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
SoftwareSerial soft_serial(DXL_SHIELD_UART_RX, DXL_SHIELD_UART_TX);  // DEFINITION (with constructor)
#endif
