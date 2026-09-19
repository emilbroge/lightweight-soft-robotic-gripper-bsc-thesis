# PlatformIO Setup & Build Guide (`code/bsc-gripper`)

This guide covers setting up your development environment, building, flashing, and monitoring the gripper firmware using **PlatformIO**.

Some of this text is written by us, some is AI generated, so take it with a grain of salt.

---

## 1. Prerequisites & Installation

You can develop using either the VS Code graphical environment or the command-line interface.

### Recommended way

1. Install [VS Code](https://code.visualstudio.com/) and the **PlatformIO IDE** extension
   (already recommended in `.vscode/extensions.json` — VS Code should prompt you to install it
   when you open `code/bsc-gripper`).
2. Open the folder `code/bsc-gripper` in VS Code (not the repo root — PlatformIO looks for
   `platformio.ini` in the opened folder).
3. Let PlatformIO install the `atmelavr` platform and the two libraries from `platformio.ini`
   on first build (it does this automatically).

### Other options
   - Alternatively, install the [PlatformIO Core CLI](https://docs.platformio.org/en/latest/core/installation/index.html).
   - Install PlatformIO Core via Python `pip`:
```bash
pip install -U platformio
```

> **Important:** Always open the subfolder `code/bsc-gripper` as the root workspace in VS Code, **not** the entire repository root. PlatformIO requires `platformio.ini` to be located directly at the workspace root to parse the project configuration.


---

## 2. Building, Uploading & Monitoring

Connect your Arduino Uno via USB. Stack the Robotis DYNAMIXEL Shield on top and connect external power to the shield's screw terminals.

### GUI (VS Code PlatformIO Toolbar)
Use the icons located in the bottom status bar:
- **Build:** Click the checkmark icon (`PlatformIO: Build`).
- **Upload:** Click the right arrow icon (`PlatformIO: Upload`).
- **Serial Monitor:** Click the plug icon (`PlatformIO: Serial Monitor`).

### Command-Line Interface (CLI)
From inside the `code/bsc-gripper/` directory:

```bash
# Compile the firmware
pio run

# Compile and upload to the connected Arduino Uno
pio run -t upload

# Open the serial monitor (115200 baud)
pio device monitor -b 115200
```

---

## 3. Hardware Quirks & Telemetry Setup

### Debug Telemetry Over SoftwareSerial
The Arduino Uno has only one hardware UART (`Serial`, pins 0 and 1), which is dedicated to the half-duplex DYNAMIXEL bus running at **1,000,000 bps**.

- **Debug logs will not appear over the default Arduino USB connection.**
- Debug telemetry is transmitted via `SoftwareSerial` on:
  - **Pin 7:** RX
  - **Pin 8:** TX
- Connect an external USB-to-UART adapter (such as an FTDI or CP2102 adapter) between pins 7/8 and your computer:
  - Adapter RX $\rightarrow$ Arduino Pin 8 (TX)
  - Adapter TX $\rightarrow$ Arduino Pin 7 (RX)
  - Adapter GND $\rightarrow$ Arduino GND
- Monitor the adapter's COM/TTY port at **115200 baud** to see the tab-separated telemetry (servo PWM, current, position, velocity, load cell forces, and state counter).

### Motor Power Requirement at Boot
During `setup()`, the firmware runs a blocking loop:
```cpp
while (!dxl.ping(DXL_ID_FLEXOR) || !dxl.ping(DXL_ID_EXTENSOR)) {
    delay(1000);
}
```
The Arduino will hang here indefinitely if:
1. External power is not applied to the DYNAMIXEL Shield's screw terminals (USB alone cannot power the servos).
2. The servo IDs in DYNAMIXEL Wizard do not match **ID 8** (extensor) and **ID 9** (flexor).
3. The baud rate of the servos is not set to `1,000,000 bps`.
