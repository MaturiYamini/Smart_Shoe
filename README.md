# Smart_Shoe

## Smart Shoe – IoT-Based Assistive Wearable System
An intelligent wearable system built using ESP32 that provides real-time step tracking, obstacle detection, and Telegram alerts.
This project integrates embedded systems, sensor processing, and secure cloud communication to create a smart assistive navigation device.

 ## Project Highlights
- Real-time Step Detection using ADXL345
- Obstacle Detection using Ultrasonic Sensor
- Instant Telegram Alerts 
- WiFi-enabled IoT Communication
- Secure Bot Authentication
- Automatic Step Counter Reset

## Hardware Components
- ESP32 Development Board
- ADXL345 Accelerometer
- HC-SR04 Ultrasonic Sensor
- Active Buzzer
- Power Supply
## Software & Libraries Used
- Arduino IDE
- ESP32 WiFi Library
- Adafruit_ADXL345_U
- Adafruit_Sensor
- UniversalTelegramBot
- WiFiClientSecure

## System Flow
1. Initialize sensors and WiFi.
2. Read accelerometer data.
3. Detect steps.
4. Measure obstacle distance.
5. Trigger buzzer or LED.
6. Send Telegram notification if condition met.
7. Repeat loop continuously.

## Smart Shoe - Connections
🟢 1️⃣ ADXL345 Accelerometer → ESP32 (I2C)
| ADXL345 Pin | ESP32 Pin                 |
| ----------- | ------------------------- |
| VCC         | 3.3V                      |
| GND         | GND                       |
| SDA         | GPIO 21                   |
| SCL         | GPIO 22                   |
| CS          | 3.3V (to enable I2C mode) |
| SDO         | GND                       |

🔵 2️⃣ Ultrasonic Sensor (HC-SR04)
| HC-SR04 Pin | ESP32 Pin |
| ----------- | --------- |
| VCC         | 5V        |
| GND         | GND       |
| TRIG        | GPIO 5    |
| ECHO        | GPIO 18 ⚠ |

🔴 3️⃣ Buzzer
| Buzzer Pin | ESP32 Pin |
| ---------- | --------- |
| +          | GPIO 33   |
| –          | GND       |

4️⃣ LED
| LED Pin     | ESP32 Pin                   |
| ----------- | --------------------------- |
| Anode (+)   | GPIO 25 (via 220Ω resistor) |
| Cathode (-) | GND                         |

5️⃣ GPS Module (Using Serial2)
| GPS Pin | ESP32 Pin     |
| ------- | ------------- |
| VCC     | 3.3V          |
| GND     | GND           |
| TX      | GPIO 16 (RX2) |
| RX      | GPIO 17 (TX2) |

## Author
Yamini Maturi
B.Tech Computer Science & Engineering



