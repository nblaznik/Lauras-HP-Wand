# ✨ Lauras HP Wand

### A WiFi-enabled magic wand system with LED spells, IR control, and a motorized unlocking box

------------------------------------------------------------------------

## 🔮 Overview

This project recreates a Harry-Potter-style magic wand system using an
**ESP8266**, an **IR receiver**, a **WS2811 LED strip**, and a
**28BYJ-48 stepper motor**.\
The wand triggers spell-themed lighting effects and physically **unlocks
a hidden box** by rotating a stepper mechanism. Everything is fully
programmable and can be updated over WiFi using **ArduinoOTA**.

Inside the repo you'll find: - The complete Arduino code that controls
spells, LEDs, and the unlocking mechanism\
- 3D-printed designs for the controller and box mechanism\
- Images, videos, and build documentation (not yet, ill eventually upload it) inside the `images/` folder\
- Test sketches and development notes from the build process

------------------------------------------------------------------------

## 🧙 Spellbook: What the Wand Can Do

The IR remote acts as a wand, sending codes that trigger spells:

  -----------------------------------------------------------------------
  Spell                Effect                  IR Code
  -------------------- ----------------------- --------------------------
  **Expecto Patronum** Blue meteor rain        `0xF30CFF00`

  **Expelliarmus**     Red meteor streak       `0xE718FF00`

  **Lumos**            Soft blue fade-in       `0xF609FF00`

  **Nox**              Fade-out darkness       `0xF807FF00`

  **Lumos Maxima**     Full-brightness flash   `0xB847FF00`

  **Alohomora**        Unlocks the box (90°    `0xF20DFF00`
                       stepper rotation)       

  **Incendio**         Fire effect with        `0xA55AFF00`
                       particle heat           
                       simulation              

  **Rainbow Wave**     Full palette sweep      `0xF708FF00`

  **Dynamic Ocean      Rolling sine-wave color `0xE31CFF00`
  Wave**               motion                  

  **Spark Cascade**    White sparkles          `0xA15EFF00`
  -----------------------------------------------------------------------

------------------------------------------------------------------------

## 🎥 Demo Videos
  <img src="./images/vid1.gif" width="48">
  <img src="./images/vid2.gif" width="48">

------------------------------------------------------------------------

## 🖼️ Images from the Build

  ------------------------------------------------------------------------------------
  Box Mechanism            LED Controller                  Wand + IR Receiver
  ------------------------ ------------------------------- ---------------------------
  <img src="./images/img1.jpg" width="48">
  <img src="./images/img2.jpg" width="48">
  <img src="./images/img3.jpg" width="48">

  ------------------------------------------------------------------------------------

------------------------------------------------------------------------

## 📦 Hardware

**Microcontroller:** ESP8266 NodeMCU\
**LEDs:** WS2811 strip, 59 LEDs\
**Motor:** 28BYJ-48 stepper + ULN2003 driver\
**IR Receiver:** 38 kHz module on pin 12\
**Power:** 5V\
**Enclosure:** 3D-printed, STL files included

------------------------------------------------------------------------

## 📐 Wiring Overview

    ESP8266
    │
    ├── PIN 5  → LED strip (WS2811 DIN)
    ├── PIN 12 → IR receiver OUT
    ├── PINS 0,2,4,5 → Stepper motor inputs
    │
    └── 5V supply → LEDs + Stepper + ESP8266 (regulated)

------------------------------------------------------------------------

## 🛠️ Folder Structure

    FinalCode/         
    ARDUINO/          
    images/            
    onlinetest/        
    README.md          

------------------------------------------------------------------------

## 🚀 Getting Started

1.  Flash `FinalCode` to an ESP8266\
2.  Connect LEDs, IR module, and stepper driver\
3.  Power with 5V\
4.  Cast spells using the IR remote\
5.  Enjoy the animations and magic unlock mechanism

------------------------------------------------------------------------

## 📚 Credits

Built using ESP8266, FastLED, IRremote, and lots of tinkering.
