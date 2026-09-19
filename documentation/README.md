# Hardware & Fabrication Documentation

This guide provides the mechanical and fabrication specifics needed to print, cast, assemble, and wire the gripper.

---

## 1. 3D Printing & Fabrication

All design files are located in the `3d_files/` directory in both editable `.step` and slicer-ready `.stl` formats[cite: 1].

### Recommended Print Settings

| Part | File | Material | Slicer Notes |
|---|---|---|---|
| **Fingers** | `Finger - with supports.stl`[cite: 1] | TPU 95A[cite: 2] | Print flat on side[cite: 2]. Use the version with built-in supports[cite: 1, 2]. Speed: 20–30 mm/s. |
| **Finger Mount** | `Finger mount.step`[cite: 1] | PLA / PLA+[cite: 2] | 4+ perimeters, 35% gyroid infill. Note: 3 mm generative struts are delicate[cite: 2]. |
| **Cable Drums** | `Finger mount - * drum half.stl`[cite: 1] | PLA / Tough PLA[cite: 2] | 100% infill for shaft rigidity. |
| **Arm Interface** | `Finger mount - Arm Interface Cap.stl`[cite: 1] | PLA[cite: 2] | Standard print settings (0.2 mm layer height). |
| **Silicone Mold** | `Finger mold.stl`[cite: 1] | PLA / PETG[cite: 1] | Smooth layer lines help with demolding. |

### Silicone Coating
1. Print the monolithic TPU fingers[cite: 2].
2. Mix a soft 2-part addition-cure silicone rubber[cite: 2].
3. Either pour silicone into `Finger mold.stl` around the finger, or dip-coat the gripping surface[cite: 1, 2]. The small retention loops on the finger will lock the cured silicone in place mechanically so it doesn't peel off under shear forces[cite: 2].

---

## 2. Electronics & Wiring Setup

The system uses an **Arduino Uno** with a stacked **Robotis DYNAMIXEL Shield**[cite: 1, 2].

### Pin Connections

| Arduino Pin | Variable in `config.h` | Connection / Device |
|---|---|---|
| **Pin 2** | `DXL_DIR_PIN` | Dynamixel Shield Direction Pin[cite: 1, 6] |
| **Pin 4** | `BTN_PIN` | Grip toggle button (connect between Pin 4 and GND)[cite: 1, 6] |
| **Pin 5** | `ABC_LOAD_CELL_CLK` | HX711 Clock (optional test rig)[cite: 1, 6] |
| **Pin 6** | `C_LOAD_CELL_DT` | HX711 Data (optional test rig)[cite: 1, 6] |
| **Pin 7** | `DXL_SHIELD_UART_RX` | SoftwareSerial RX (for computer debug printing)[cite: 1, 6] |
| **Pin 8** | `DXL_SHIELD_UART_TX` | SoftwareSerial TX (for computer debug printing)[cite: 1, 6] |
| **Pin 9** | `SCOPE_PIN_A` | Timing test pin[cite: 1, 6] |
| **Pin 10** | `SCOPE_PIN_B` | Timing test pin[cite: 1, 6] |

### Power Supply Warning
> **Warning:** You **must** provide external 5V power to the screw terminals on the DYNAMIXEL Shield[cite: 1, 2]. The USB cable alone only powers the Arduino board; the motors will not respond, and the initialization code will hang in a ping loop until external power is supplied[cite: 1, 4].

---

## 3. Assembly References

For detailed mechanical assembly steps, see the PDFs included in this folder:
- **`assembly_instructions_compressed.pdf`**: Exploded-view diagrams showing how to press-fit the bearings, assemble the drums, route the cords, and attach the unit to the robot arm[cite: 1, 2].
- **`bill_of_materials_compressed.pdf`**: Itemized list of fasteners, bearings, wire, tubing, and electronics with part numbers and supplier links[cite: 1, 2].
