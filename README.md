# The Hyphen - Hardware Demo V1

**The Hyphen** is a physical computing project exploring the tension of existing between two states or identities. This prototype translates abstract emotional tension into a synchronized physical and digital experience.

## Project Concept
The core interaction is based on **time in a state**. Tiling the device initiates a "tension" value that grows exponentially the longer it is held. Returning to a neutral state allows the tension to decay gracefully.

## Features
- **Physical Input:** Tilt switch (NC) detected via Arduino internal pull-ups for maximum stability.
- **Physical Output:** Active buzzer modulated with ultra-short duty cycles to produce a quiet, organic "low hum" vibration rather than a sharp beep.
- **Digital Twin:** A high-performance 3D visualizer built with **Three.js** and **Web Serial API**.
- **Visual Language:** Two volumetric 3D orbs conjoined by an elastic, organic bridge that thins and vibrates as tension pulls them diagonally apart.
- **Precision Logic:** High-speed 115200 baud communication with lag-proof buffer management and liquid smoothing (lerp).

## Hardware Setup
- **Arduino Uno R3**
- **Tilt Switch:** Connected between **GND** and **Digital Pin 2** (No physical resistor required).
- **Active Buzzer:** Driven via a **PN2222 NPN Transistor**:
  - **Emitter:** GND
  - **Base:** Pin 9 (via 220Ω resistor)
  - **Collector:** Buzzer (-)
  - **Buzzer (+):** 5V

## Installation & Usage

### 1. Hardware (Arduino/PlatformIO)
1. Open the project in VS Code with the **PlatformIO** extension.
2. Connect your Arduino Uno.
3. Click the **Upload (→)** button to flash the firmware.

### 2. Software (Digital Visualizer)
1. Open `index.html` in **Google Chrome** or **Microsoft Edge**.
2. Click **"CONNECT_HARDWARE_INTERFACE"**.
3. Select your Arduino's serial port.
4. Physically tilt your breadboard to see and hear the tension build.

## Technical Details
- **Baud Rate:** 115200
- **Logic:** Non-blocking timing (`millis()` / `micros()`)
- **Visuals:** Three.js with UnrealBloom post-processing and cubic bezier deformation.
- **Stability:** 50ms hardware debouncing and 33ms Serial update frequency.
