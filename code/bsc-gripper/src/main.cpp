#include <Arduino.h>
#include <config.h>
#include <serial_config.h>
#include <dynamixel_control.h>
#include <load_cell.h>


// Variables used in main.cpp
bool grip = false;
unsigned long last_grip_change = 0;
unsigned long last_print = 0;
int round_i = 0;

static unsigned long lastBtnTime = 0;

// States to hold info
DxlState dxlSt;
LoadCellState lcSt;


// Simple helper for printing
static void print_state(const DxlState &dxlSt, const LoadCellState &lcSt, const GripState &gripSt) {

    for (uint8_t i = 0; i < DXL_ID_CNT; i++) {
        DEBUG_SERIAL.print(dxlSt.current[i]);
        DEBUG_SERIAL.print("\t");
        DEBUG_SERIAL.print(dxlSt.position[i]);
        DEBUG_SERIAL.print("\t");
        DEBUG_SERIAL.print(dxlSt.pwm[i]);
        DEBUG_SERIAL.print("\t");
        DEBUG_SERIAL.print(dxlSt.velocity[i]);
        DEBUG_SERIAL.print("\t");
    }
    for (uint8_t i = 0; i < N_SCALES; i++) {
        DEBUG_SERIAL.print(lcSt.load[i]);
        DEBUG_SERIAL.print("\t");
    }
    DEBUG_SERIAL.print(((int)gripSt)*10);
    DEBUG_SERIAL.print("\t");

    
    DEBUG_SERIAL.print(round_i);
    DEBUG_SERIAL.print("\t");
    
    DEBUG_SERIAL.println();
}


void setup() {

  // Set up pins
  pinMode(SCOPE_PIN_A, OUTPUT);
  pinMode(SCOPE_PIN_B, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  digitalWrite(SCOPE_PIN_A, LOW);
  digitalWrite(SCOPE_PIN_B, LOW);

  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  DEBUG_SERIAL.print("Test test");

  // Initialize motors
  dxl_init();

  // Only initialize if we need to test - flag set in config.h
  if (debugLoadCells) load_cell_init();
  
// Not needed anymore
  // dxl_auto_calibration(250);
  

}

void loop() {
  

  unsigned long now = millis();

  // Toggle grip command with external button
  if (digitalRead(BTN_PIN) == LOW && (now - lastBtnTime > BTN_DEBOUNCE)) {
    lastBtnTime = now;
    // button action
    grip = !grip; // Change gripping command
    if (grip) round_i++;
  }

  // Toggle grip command periodically with a MS period - not used anymore
  if (now - last_grip_change >= GRIPPING_PERIOD_MS) {
    
    last_grip_change = now;                 // reset timer
    // ---- do your timed action here ----
    grip = !grip; // Change gripping command every GRIPPING_PERIOD_MS
    if (grip) round_i++;


  }

  if (now - last_print >= SAMPLING_PERIOD_MS) {
      last_print = now;                 // reset timer

      digitalWrite(SCOPE_PIN_B, HIGH); // Used for debuging

      if(debugLoadCells) {
              lcSt = load_cell_read();
        }
      digitalWrite(SCOPE_PIN_B, LOW); // Used for debuging

      digitalWrite(SCOPE_PIN_A, HIGH); // Used for debuging

      dxlSt = dxl_read_state();

      digitalWrite(SCOPE_PIN_A, LOW); // Used for debuging

      // Checks if we got a timeout error, and if yes we reinit motors.
      if (dxl_motors_timeout()) {
            // DEBUG_SERIAL.println("Motors timed out");
            dxl_init();            
            // dxl_auto_calibration(250);
            grip = false;

      }

      dxl_filter_current(dxlSt);

      
      digitalWrite(SCOPE_PIN_B, HIGH); // Used for debuging

      
      GripState gripSt = dxl_get_grip_state();

      print_state(dxlSt, lcSt, gripSt);
      digitalWrite(SCOPE_PIN_B, LOW);


      dxl_update_state(dxlSt, grip);

      

    }
  
    





}
