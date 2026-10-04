#  3DOF Robotic Work Lamp

An Arduino-controlled 3-DOF robotic arm converted into an articulated work lamp.

Instead of using the arm as a traditional pick-and-place robot with a gripper, I wanted to explore a different use for its movement: **positioning a light in places that are difficult for a normal desk lamp to reach.**

The current prototype uses three MG996R servo motors for the waist, shoulder and elbow. A joystick moves the arm, while three physical buttons select which joint is currently being controlled.

> 🚧 This project is currently a working prototype and is still being calibrated and improved.

---

##  Why a robotic lamp?

Traditional desk lamps are limited by their hinges and where their base can be positioned.

A robotic arm already provides several independently controlled joints, so I experimented with replacing the gripper with a light.

This gives the lamp three controllable axes:

- **Waist** — rotates the lamp left/right
- **Shoulder** — raises and lowers the main arm
- **Elbow** — changes the reach and angle of the lamp

The goal is to create a work light that can be manually positioned into unusual or difficult-to-reach areas.

---

##  Controller

The lamp uses one analog joystick and three push buttons.

| Control | Function |
|---|---|
| 🔴 Red button | Select waist |
| 🔵 Blue button | Select shoulder |
| 🟡 Yellow button | Select elbow |
| 🕹️ Joystick left/right | Move selected joint |
| 🕹️ Joystick click | Return arm to home position |

Once a joint is selected, the joystick controls that joint until another button is pressed.

The amount the joystick is moved also affects the movement speed.

---

##  Hardware

- Arduino UNO R4 Minima
- 3 × MG996R servo motors
- 3D-printed robotic arm
- Analog joystick module
- 3 × push buttons
- Breadboard
- Jumper wires
- External 5V / 5A DC power supply
- DC barrel-jack screw-terminal adapter
- Lamp/light mounted at the end of the arm

---

##  Pin Mapping

### Servo motors

| Joint | Arduino Pin |
|---|---:|
| Waist | D5 |
| Shoulder | D6 |
| Elbow | D7 |

### Controller

| Input | Arduino Pin |
|---|---:|
| Waist button | D2 |
| Shoulder button | D3 |
| Elbow button | D4 |
| Joystick X | A0 |
| Joystick button | D8 |

---

##  Power

The MG996R servos are **not powered from the Arduino's 5V pin**.

They use a separate 5V / 5A supply.

```text
5V / 5A Supply
      │
      ├── +5V ──→ Servo red wires
      │
      └── GND ──→ Servo brown wires
                     │
                     └── Arduino GND
```

The Arduino and external servo supply share a **common ground** so that the servo control signals have the same electrical reference.

The Arduino itself is powered through USB during development.

---

##  Software

The Arduino keeps track of:

- which joint is currently selected
- the current angle of each servo
- safe software limits for each joint
- joystick direction
- joystick movement magnitude

A dead zone around the joystick center prevents small analog fluctuations from causing unwanted movement.

The current firmware also includes a HOME function that gradually returns all three joints toward their neutral positions.

The Arduino firmware can be found here:

[`firmware/work_lamp_controller.ino`](firmware/work_lamp_controller.ino)

---

##  Current Software Limits

These limits are intentionally conservative while the mechanical range of the prototype is being calibrated.

| Joint | Minimum | Maximum |
|---|---:|---:|
| Waist | 30° | 150° |
| Shoulder | 70° | 110° |
| Elbow | 60° | 120° |

These are software safety limits and may change as the mechanical design is tested further.

---

##  Current Status

### Working

- [x] 3-axis mechanical arm assembled
- [x] External servo power system
- [x] MG996R servo testing
- [x] Analog joystick input
- [x] Three joint-selection buttons
- [x] Waist joystick control
- [x] Lamp mounted to the arm
- [x] Software joint limits
- [x] HOME function implemented

### In progress

- [ ] Finish calibration of shoulder control
- [ ] Finish calibration of elbow control
- [ ] Determine final mechanical angle limits
- [ ] Improve cable management
- [ ] Build a cleaner controller enclosure
- [ ] Refine smooth movement

---

##  Possible Future Improvements

Some ideas I want to explore:

- smoother coordinated movement between joints
- programmable lamp positions
- improved physical controller
- ESP32 wireless control
- Bluetooth controller support
- automatic positioning
- additional sensors
- cleaner electronics enclosure

---

##  What I Learned

This project has been a practical introduction to:

- servo motor control
- external power supplies
- common-ground wiring
- breadboard power distribution
- analog joystick input
- digital button input
- Arduino `INPUT_PULLUP`
- servo angle limits
- state-based controls
- debugging electronics and mechanical systems together

One of the biggest lessons was that building the mechanical arm was only part of the problem. Power delivery, wiring, control logic and mechanical limits all have to work together.

---

##  Credits

The 3D-printed robotic arm mechanism is based on the open-source **Bench Robotics 3D-Printed Arm** project, which itself was inspired by the robotic arm design from **How To Mechatronics**.

I modified the project by simplifying it to three main joints, developing a physical joystick/button control system, and converting the end effector into a work lamp.

---

##  Safety

High-torque servos can draw significant current and can move unexpectedly.

- Do not power the MG996R servos directly from the Arduino 5V pin.
- Use an appropriate external 5V power supply.
- Always connect the Arduino and servo supply grounds together.
- Keep fingers clear of moving joints.
- Establish safe mechanical limits before commanding large servo movements.

---

##  License

License information will be added as the project develops.
