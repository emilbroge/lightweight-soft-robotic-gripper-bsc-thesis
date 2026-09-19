# Bio-Inspired, Lightweight Soft Robotic Gripper

A lightweight, tendon-driven soft robotic gripper developed as a Bachelor’s thesis at DTU (Neuro Robotics Technology Lab) by Mads August Claussen and Emil Broge Johansen[cite: 2].

The goal of this project was to design a compliant gripper that weighs almost nothing (only 37 g at the wrist!) so it can be mounted onto soft, low-payload continuum robotic arms without weighing them down[cite: 2]. Instead of rigid fingers with lots of gears, it uses flexible 3D-printed fingers shaped like an octopus tentacle that wrap gently around objects of different shapes and sizes[cite: 2].

https://github.com/user-attachments/assets/gripper-showcase-vid.mp4

*(If you are viewing this locally, check out `documentation/gripper-showcase-vid.mp4` and `documentation/gripper_being_operated_and_powered_off.mp4` to see it in action!)*[cite: 1, 2]

---

## 1. System Architecture & Mechanism

To keep the hand as light as possible, we separated the "muscles" from the "fingers"[cite: 2]:

<!-- Insert your system architecture diagram here -->
![System Architecture Diagram](documentation/architecture_diagram.png)

- **External Actuator Box:** Two smart servo motors sit away from the moving arm[cite: 2]. One pulls the "flexor" tendon to close the grip, and the other pulls an "extensor" tendon to open it and add stiffness[cite: 1, 2].
- **Bowden Cable Transmission:** The pulling force travels through slick PTFE tubes via strong polyethylene cords (Dyneema)[cite: 2].
- **Cable Drum Wrist:** Inside the mount, small spools turn linear cable pulling into rotation without needing a bulky mechanism[cite: 2].
- **Octopus-Inspired Fingers:** The fingers are printed out of flexible TPU with built-in bending joints[cite: 2]. They follow a logarithmic spiral shape so they naturally wrap around objects instead of pinching at a stiff angle[cite: 2].
- **Silicone Skin:** A soft silicone layer gives the fingers the grip they need so objects don't slip out[cite: 2].

---

## 2. Hardware Build Guide

The entire physical build uses affordable 3D-printed parts, standard hobbyist bearings, and two smart servos[cite: 2]. The brain of the setup is an Arduino Uno fitted with a motor shield[cite: 1, 2].

All step-by-step guides and CAD drawings are organized in the `documentation/` folder:
- **Parts List & Costs:** See the [Bill of Materials (PDF)](documentation/bill_of_materials_compressed.pdf)[cite: 1, 2]
- **Step-by-Step Assembly:** See the [Assembly Guide (PDF)](documentation/assembly_instructions_compressed.pdf)[cite: 1, 2]
- **Fabrication & Print Specs:** See the [Hardware & Fabrication Guide](documentation/README.md#1-3d-printing--fabrication)
- **Full Thesis Report:** Read [Full BSc Report (PDF)](documentation/Full_BSc_Report_compressed_prepress.pdf) for the academic background, math, and design decisions[cite: 1, 2].

---

## 3. Code & Electronics Build Guide

The microcontroller firmware is set up in `code/bsc-gripper/`[cite: 1]. It handles motor communication, runs an automatic grip sequence, and streams diagnostic data[cite: 1, 3, 4].

- We use **PlatformIO** (via the VS Code extension) to compile and upload code automatically[cite: 1].
- **Wiring & Pinout:** See the [Wiring Setup in documentation](documentation/README.md#2-electronics--wiring-setup).
- **Firmware Architecture & Motor Tuning:** See the [Firmware Deep-Dive](code/bsc-gripper/README.md) for pinouts, state machine explanations, and tuning constants in `config.h`[cite: 1].

---

## 4. Experimental Data (`raw_data/`)

All raw test data from our validation trials are available in `raw_data/`[cite: 1]:
- `transmission_test/`: Tendon transmission efficiency measurements and `transmission_test.py` to calculate friction coefficients ($\mu$)[cite: 1, 2].
- `motor_performance_test/`: Current and force measurements for XL330 and XL430 servos[cite: 1, 2].
- `payload_test/`: Pull-force test data across 11 test shapes (`test_objects.csv` describes dimensions and weights)[cite: 1, 2].
- `success_rate/`: Grasp trials (`data_succes.csv`) on real everyday objects[cite: 1, 2].

---

## 5. Potential Improvements

If you're picking up this project for a semester project or thesis, here are several clear areas where you can take it further:

### Hardware Improvements
- **Miniaturization & Cleaner Enclosure:** Redesign the motor box to be smaller and more self-contained[cite: 2]. Adding buttons directly to the enclosure would make manual testing much smoother (for example, triggering test routines or toggling live serial output)[cite: 3].
- **Easier Finger Assembly:** Threading the tendons and securing the fingers to the mount currently requires a very specific touch and patience. A more modular, snap-in joint design would save a lot of headaches.
- **Self-Contained Power:** The prototype currently relies on a 5V benchtop power supply and a tethered laptop[cite: 1, 2]. Integrating a small internal regulator or battery pack would make the setup truly portable[cite: 2].
- **Tendon Terminations:** Finding a cleaner mechanical method to clamp the tendon ends (rather than manual knots) would make string tensioning and length adjustment much more repeatable[cite: 2].

### Control Improvements
- **Scientifically Grounded Control:** The current firmware uses a tuned open-loop profile[cite: 2]. A great next step would be modeling the tendon-sheath friction analytically—for instance, using the approach from:
  > Z. Wang and N. M. Freris, *"Exploiting Frictional Effects to Reproduce Octopus-Like Reaching Movements with a Cable-Driven Spiral Robot,"* 2024 IEEE 7th International Conference on Soft Robotics (RoboSoft), San Diego, CA, USA, 2024, pp. 537-542[cite: 2].
- **Grasp Detection:** Use live motor current readings to detect when the fingers touch an object and stop pulling automatically[cite: 2].

---

## Notes for the Next Student

Don't hesitate to reach out! If you run into weird quirks with the build, need tips on 3D printing TPU, or want to bounce ideas about extending the design, feel free to contact us:

- **Mads August Claussen**
- **Emil Broge Johansen**[cite: 2]
