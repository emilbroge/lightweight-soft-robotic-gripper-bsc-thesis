# Gripper Firmware Guide (`code/bsc-gripper`)

This folder contains the PlatformIO firmware project targeting an Arduino Uno (`env:uno` in `platformio.ini`) equipped with a Robotis DYNAMIXEL Shield[cite: 1].

---

## 1. Project Structure & Responsibilities

| File | Purpose / Responsibility |
|---|---|
| `src/main.cpp` | Main loop, button debounce, state machine invocation, periodic sampling, and serial formatting[cite: 1, 3]. |
| `src/config.h` | All hardware pins, motor IDs, safety limits (PWM/Current), timing periods, and grip-state tuning values[cite: 1, 6]. |
| `src/dynamixel_control.{h,cpp}` | DYNAMIXEL communication via `Dynamixel2Arduino`, sync-write packet packing, 12-byte feedback reading, and the grip state machine (`dxl_update_state()`)[cite: 1, 4, 5]. |
| `src/load_cell.{h,cpp}` | Reading and calibrating the optional HX711 strain-gauge amplifier (active when `debugLoadCells = true`)[cite: 1, 6]. |
| `src/serial_config.{h,cpp}` | Configures `SoftwareSerial` pins for telemetry output[cite: 1]. |

---

## 2. Crucial Hardware Quirks

### 1. The Single Hardware UART on Arduino Uno
The Arduino Uno has only one hardware serial port (`Serial`, pins 0 and 1)[cite: 1]. This hardware port is entirely dedicated to the high-speed half-duplex DYNAMIXEL bus running at **1,000,000 bps** (`dxl.begin(1000000)`)[cite: 1, 4].
- **Debug output cannot be sent over standard USB Serial.**
- Telemetry is transmitted via `SoftwareSerial` on **Pins 7 (RX)** and **8 (TX)** at **115200 baud**[cite: 1, 6].
- To read debug logs on your PC, connect an external USB-to-UART adapter (FTDI / CP2102) to pins 7 and 8[cite: 1].

### 2. Motor Ping Loop (`dxl_init()`)
During startup, `dxl_init()` executes a blocking `while` loop[cite: 1, 4]:
```cpp
while (!dxl.ping(DXL_ID_FLEXOR) || !dxl.ping(DXL_ID_EXTENSOR)) {
    delay(1000);
}
```[cite: 4]
If the motors do not have external power connected to the shield, or if the motor IDs do not match `DXL_ID_FLEXOR` (`9`) and `DXL_ID_EXTENSOR` (`8`), the board will hang here indefinitely[cite: 1, 4, 6].

---

## 3. One-Time Servo Setup (DYNAMIXEL Wizard 2.0)

Before wiring both servos onto the shared bus, plug each servo into a computer individually (using a U2D2 or USB adapter) and configure them via **DYNAMIXEL Wizard 2.0**[cite: 1]:

1. Set **Baud Rate** to `1,000,000 bps`[cite: 1, 4].
2. Set **Protocol** to `2.0`[cite: 1, 4].
3. Set IDs:
   - Extensor Servo: **ID 8**[cite: 1, 6]
   - Flexor Servo: **ID 9**[cite: 1, 6]
4. Check that operating mode is accessible (the firmware automatically sets `OP_PWM` during setup)[cite: 4].

---

## 4. Grip State Machine & Control Strategy

The gripper uses an antagonistic PWM profile across five states defined in `dynamixel_control.cpp`[cite: 1, 4]:

```text
[GRIP_OPEN]
     │ (Button press / grip = true)
     ▼
[GRIP_CLOSING]   --> Flexor ramps up with CLOSING_FLEX_SLOPE, extensor applies slight drag
     │ (Flexor reaches CLOSING_FLEX_LIMIT)
     ▼
[GRIP_TIGHTEN]   --> Peak PWM applied to overcome friction and seat the tendons
     │ (Flexor reaches TIGHTEN_FLEX_LIMIT)
     ▼
[GRIP_HOLDING]   --> PWM drops to low holding level (HOLDING_FLEX_LIMIT) to prevent motor heating
     │ (Button press / grip = false)
     ▼
[GRIP_RELEASING] --> Extensor pulls high (RELEASING_EXTE_LIMIT) while flexor releases
     │ (Extensor velocity slows & position reached)
     ▼
[GRIP_OPEN]
```[cite: 1, 4]

### Key Tunables in `config.h`
- `MAX_PWM` (default `600`): Maximum allowable PWM duty written to the registers[cite: 6].
- `MAX_CURRENT` (default `300`): Maximum allowable current limit[cite: 6].
- `CLOSING_FLEX_SLOPE` & `CLOSING_FLEX_LIMIT`: Adjust how quickly and firmly the hand initially curls[cite: 6].
- `HOLDING_FLEX_LIMIT` (default `250`): Steady-state holding power. Keep this low to avoid overheating the XL330 motors during continuous grasps[cite: 2, 6].
- `SAMPLING_PERIOD_MS` (default `100`): Frequency of state updates. *Note: If using the HX711 load cell, this must remain $\ge 100\text{ ms}$ due to the 10 SPS ADC conversion rate*[cite: 2, 6].

---

## 5. Notes on Auto-Calibration & Homing

- In `main.cpp`, you will notice a commented-out call: `// dxl_auto_calibration(250);`[cite: 1, 3]. This was an experimental auto-homing feature that was never fully implemented[cite: 1].
- The current firmware operates in open-loop PWM mode[cite: 2, 4]. Therefore, **the motors must start at their neutral open position when powered on**. Ensure tendon slack is taken up before running the state machine.
