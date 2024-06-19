# ACCINET-Accident-Detection-GPS-Twilio-Firebase
# ACCINET: Accident Intervention Network

## Overview

ACCINET is an IoT-based system designed to detect accidents and promptly send alerts to predefined contacts. It utilizes an MPU6050 accelerometer for accident detection, a NEO-6M GPS module for dynamic location tracking, an OLED display for real-time feedback, and WiFi for connectivity. The system sends SMS alerts using the Twilio API and logs data to Firebase, ensuring timely intervention and support.

## Features

- **Accident Detection**: Monitors sudden changes in acceleration using an MPU6050 accelerometer.
- **Dynamic GPS Tracking**: Uses a NEO-6M GPS module to provide real-time location data.
- **OLED Display**: Displays system status, accelerometer data, and alert messages.
- **WiFi Connectivity**: Connects to the internet to send alerts and update Firebase.
- **Twilio API**: Sends SMS alerts to predefined emergency contacts.
- **Firebase Integration**: Logs accident details and status updates in real-time.
- **NTP Server**: Ensures accurate timestamping for logged data.

## Components

- **MPU6050 Accelerometer**: Detects sudden changes in motion.
- **Adafruit SSD1306 OLED Display**: Shows real-time feedback and system status.
- **NEO-6M GPS Module**: Provides dynamic location tracking.
- **ESP32 Board**: Central processing unit with WiFi capabilities.
- **Buzzer**: Alerts the user audibly in case of an accident.
- **Push Button**: Allows manual intervention to cancel alerts.
- **WiFi Module**: Enables internet connectivity for real-time data transmission.

## Getting Started

### Prerequisites

- ESP32 Development Board
- MPU6050 Accelerometer
- Adafruit SSD1306 OLED Display
- NEO-6M GPS Module
- Buzzer and Push Button
- Arduino IDE with necessary libraries installed

### Libraries Required

- Wire
- Adafruit_GFX
- Adafruit_SSD1306
- MPU6050
- WiFi
- HTTPClient
- base64
- TinyGPS++
- SoftwareSerial

### Installation

1. **Clone the Repository**:
    ```sh
    git clone https://github.com/yourusername/ACCINET-Accident-Detection-GPS-Twilio-Firebase.git
    cd ACCINET-Accident-Detection-GPS-Twilio-Firebase
    ```

2. **Install Required Libraries**:
   Install the necessary libraries via the Arduino Library Manager or manually from the given sources.

3. **Upload the Code**:
   Open the provided code in Arduino IDE, configure your WiFi credentials and Twilio/Firebase settings, and upload the code to your ESP32 board.

## Usage

1. **Power On the Device**: Ensure all components are properly connected and power on your ESP32 board.
2. **Connect to WiFi**: The device will attempt to connect to the configured WiFi network.
3. **Accident Detection**: The MPU6050 will monitor for sudden changes in acceleration.
4. **Alert Generation**: Upon detecting an accident, the system will fetch the location from the GPS module and send SMS alerts via Twilio.
5. **Data Logging**: Accident details and status updates will be logged to Firebase in real-time.

## Contributing

1. Fork the repository.
2. Create your feature branch: `git checkout -b feature/AmazingFeature`
3. Commit your changes: `git commit -m 'Add some AmazingFeature'`
4. Push to the branch: `git push origin feature/AmazingFeature`
5. Open a pull request.

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Acknowledgements

- [TinyGPS++ Library](https://github.com/mikalhart/TinyGPSPlus)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Twilio API](https://www.twilio.com/docs/usage/api)
- [Firebase Realtime Database](https://firebase.google.com/docs/database)

