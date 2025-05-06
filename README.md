# ACCINET: Accident Intervention Network (C++ + IoT Hardware Project)

## Project Overview

ACCINET is a hardware-integrated IoT system programmed in **C++**, designed to detect road accidents and immediately notify emergency contacts. It brings together multiple hardware components—sensors, GPS, display, and wireless modules—controlled via efficient and responsive **C++ code running on an ESP32 microcontroller**.

This project demonstrates how **C++ enables direct control over sensors, real-time processing, and internet communication** in embedded systems, making it ideal for safety-critical IoT applications.

##  Core Concept

ACCINET uses hardware components interfaced through **C++** code to:

1. Detect accidents via motion sensors (MPU6050).
2. Get the exact GPS location (NEO-6M).
3. Show system status on an OLED screen.
4. Trigger a buzzer alert.
5. Allow manual alert cancellation via a push button.
6. Send SMS alerts through the **Twilio API** using HTTP calls in C++.
7. Log data to **Firebase Realtime Database** for cloud-based monitoring.


##  Hardware Components

| Component                   | Description                                                        |
| --------------------------- | ------------------------------------------------------------------ |
| **ESP32 Development Board** | Main microcontroller running C++ code for sensor control and logic |
| **MPU6050 Accelerometer**   | Detects sudden acceleration/deceleration to identify accidents     |
| **NEO-6M GPS Module**       | Provides real-time latitude and longitude                          |
| **Adafruit SSD1306 OLED**   | Displays sensor data, system status, and alerts                    |
| **Buzzer**                  | Emits sound when accident is detected                              |
| **Push Button**             | Used to manually cancel alerts                                     |
| **WiFi (built into ESP32)** | Enables cloud integration and SMS communication via HTTP           |

---

## How C++ Powers the Hardware

Each hardware module is programmed and controlled using **C++**, leveraging its ability to work closely with low-level operations, memory management, and real-time performance.

### What C++ Does in This Project:

* **Sensor Interfacing**: Reads acceleration data from the MPU6050 using I2C protocols via C++ `Wire` library.
* **GPS Parsing**: Receives and parses GPS NMEA data using the `TinyGPS++` library written in C++.
* **Display Control**: Draws text and graphics on OLED using `Adafruit_GFX` and `Adafruit_SSD1306` C++ libraries.
* **Network Communication**: Uses `WiFi` and `HTTPClient` libraries in C++ to connect to the internet.
* **Alerting via Twilio**: C++ code constructs HTTP requests, encodes credentials with `base64`, and sends SMS.
* **Data Logging to Firebase**: Sends structured JSON data to the Firebase Realtime Database via HTTP POST using C++.
* **Interrupt and State Handling**: Uses C++ condition checks and ISR (Interrupt Service Routines) for button presses.


## Software Requirements

* **Arduino IDE** (for writing, compiling, and uploading C++ code)
* **Installed C++ Libraries**:

  * `Wire`
  * `MPU6050`
  * `Adafruit_GFX`, `Adafruit_SSD1306`
  * `TinyGPS++`
  * `WiFi`, `HTTPClient`
  * `base64`

---

## Getting Started (Hardware + C++)

### 1. Connect the Hardware

Connect components to the ESP32 according to the following:

* **MPU6050** → I2C Pins (SCL/SDA)
* **GPS** → Serial RX/TX
* **OLED** → I2C
* **Buzzer** → GPIO with transistor (if needed)
* **Push Button** → GPIO with pull-down resistor

### 2. Flash the C++ Code

Open the code in Arduino IDE:

* Configure WiFi SSID/PASSWORD, Twilio credentials, and Firebase URL.
* Compile and upload the C++ code to the ESP32.

### 3. Run the System

* On boot, the system connects to WiFi.
* If an accident is detected, the C++ program collects GPS data, displays it, triggers the buzzer, and sends alerts.
* Data is logged to Firebase with timestamps using NTP.

---

##  Workflow (All in C++)

```cpp
void setup() {
  initSensors();
  connectWiFi();
  initializeDisplay();
  syncTimeWithNTP();
}

void loop() {
  if (accidentDetected()) {
    getGPSLocation();
    triggerBuzzer();
    sendSMSviaTwilio();
    logToFirebase();
  }
  checkManualCancel();
}
```

---

## 📤 Output Example

 **SMS Alert**:

  ```
  Accident Detected!
  Location: https://maps.google.com/?q=12.9716,77.5946
  Time: 10:42:31 AM
  ```

* **Firebase Log**:

  ```json
  {
    "latitude": 12.9716,
    "longitude": 77.5946,
    "timestamp": "2025-05-06T10:42:31Z",
    "status": "Accident Detected"
  }
  ```

## Contributing

1. Fork the repo and add your feature using C++.
2. Test with actual hardware setup.
3. Submit a pull request with description and photos/videos of hardware test.

## License

Distributed under the MIT License. See LICENSE for more information.

## Acknowledgements

- [TinyGPS++ Library](https://github.com/mikalhart/TinyGPSPlus)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Twilio API](https://www.twilio.com/docs/usage/api)
- [Firebase Realtime Database](https://firebase.google.com/docs/database)
