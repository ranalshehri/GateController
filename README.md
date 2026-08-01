# GateControl — ESP32 WiFi Servo + LED Controller

A Wemos D1 Mini ESP32 project that hosts its own WiFi access point with a simple webpage to open/close a servo-driven gate, with a green LED indicating status.

## What it does

- ESP32 creates its own WiFi network (no router/internet needed)
- Hosts a webpage with two buttons: **OPEN** and **CLOSE**
- **OPEN** → servo moves to a set angle, green LED turns ON
- **CLOSE** → servo returns to resting angle, green LED turns OFF

## Hardware

- Wemos D1 Mini ESP32
- 1x Servo motor (SG90 or similar)
- 1x Green LED
- 1x Resistor (220–330Ω)
- Breadboard + jumper wires

## Wiring

| Component | Pin | Connects to |
|---|---|---|
| Servo | Signal (orange/yellow) | GPIO23 |
| Servo | Power (red) | 5V / VIN rail |
| Servo | Ground (brown/black) | GND rail |
| LED | Anode (long leg) | → resistor → GPIO19 |
| LED | Cathode (short leg) | GND rail |
| ESP32 | 5V/VIN | Breadboard + rail |
| ESP32 | GND | Breadboard – rail |

**Notes:**
- Confirm your board's power pin is actually 5V/VIN (not 3.3V) before wiring the servo to it — 3.3V will make it move weakly or not at all, but won't damage anything.
- Keep all grounds (ESP32, servo, LED) tied to the same rail — shared ground is required for the circuit to work reliably.
- One small servo + one LED draws well within safe limits for USB power. Don't power multiple servos off the same USB-fed rail — use a separate 5V supply for that.

## Setup (Arduino IDE)

1. Install the **ESP32 board package** (Boards Manager → search "esp32" by Espressif Systems)
2. Install the **ESP32Servo** library (Library Manager → search "ESP32Servo" by Kevin Harrington / madhephaestus)
3. Select your board and port under **Tools**
4. Paste in `sketch.ino` (below), adjust config values if needed
5. Upload

## Config values to check/change

```cpp
const char* AP_SSID     = "GateControl";   // WiFi network name
const char* AP_PASSWORD = "12345678";      // WiFi password (8+ chars)

#define SERVO_PIN   23   // servo signal pin
#define GREEN_PIN   19   // LED pin

int OPEN_ANGLE   = 90;   // servo angle when "open"
int CLOSED_ANGLE = 0;    // servo angle when "closed" / resting
```

## How to use it

1. Upload the sketch, then open **Serial Monitor** (115200 baud) — you should see:
   ```
   AP IP address: 192.168.4.1
   ```
2. On your phone or laptop, connect to WiFi network **`GateControl`** (in my own example), (password `12345678`). It's normal for your device to warn "no internet" — stay connected anyway.
3. Open a browser and go to `192.168.4.1`
4. Tap **OPEN** or **CLOSE** to control the servo + LED

## Built-in serial test mode

You can bypass the webpage entirely to test the servo/LED directly:

1. Open Serial Monitor (115200 baud)
2. Type `o` and hit Enter → servo opens, LED on
3. Type `c` and hit Enter → servo closes, LED off

Useful for confirming wiring/hardware is working independent of WiFi/browser issues.

## Troubleshooting

- **Buttons don't respond / nothing moves**: power-cycle the ESP32 (unplug/replug or hit EN/RST). WiFi AP + web server occasionally need a clean boot after upload.
- **`ESP32Servo.h: No such file or directory`**: install the ESP32Servo library via Library Manager.
- **Servo moves weakly or not at all**: check that its power wire is on 5V.
- **Phone shows "no internet" on GateControl network**: expected — this is a standalone AP with no internet, not your home WiFi.
