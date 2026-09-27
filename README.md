# IoT-based Health Monitoring System 🔒

A DIY cloud-based portable health monitoring system that can detect someone's oxygen level, blood presssure and temperature using MAX3010x and DS18B20, integrated with IoT via NodeMCU ESP8266 ESP-01 for online monitoring..

---

## 📘 Project Overview

- **Course:** Microprocessor & Microcontroller Lab (CSE 3108)
- **Institution:** Notre Dame University Bangladesh
- **Batch:** CSE 22
- **Project Timeline:** August 26, 2025 – November 17, 2025

### 👥 Team Members

- [Durjoy Das](https://github.com/dasdurjoy0) (ID: 0692320005101033)
- [Koshiq Chowdhury](https://github.com/Koushiq-Sourav) (ID: 0692320005101006)
- Shantashree Bhattacherjee (ID: 0692320005101017)

---

## 📌 Objective

To build a low-cost, efficient, and remotely monitored cloud-based health monitoring system using Arduini Uno, ESP-01 and basic electronic componants that can measure basic vitals.

---

## 🎯 Features

- **SPO2 + DS18B20** for measuring temp and Oxygen level.
- **Passive Buzzer** for sound alarm.
- **LCD Display (16x2)** to display real-time status messages.
- **ESP8266 ESP-01 Web Interface** for remote monitoring.
- **Portable Power Supply** using a power bank.

---

## 🔧 Hardware Components

| Component              | Quantity   | Cost (BDT) |
|------------------------|------------|------------|
| Arduino UNO R3         | 1          | 1050/-     |
| ESP8266 ESP-01         | 1          | 420/-      |
| DS18B20 Module         | 1          | 200/-      |
| Active Buzzer          | 1          | 15/-       |
| LCD Display (16x2)     | 1          | 230/-      |
| Breadboard (830 point) | 1          | 150/-      |
| SPO2 MAX3010x          | 1          | 60/-       |
| 10k Potentiometer      | 1          | 10/-       |
| Jumper Wires           | As req.    | 100/-      |
| Misc Wires + Power     | As req.    | 150/-      |
| LED lights             | 2          | 10/-       |

---

## 🧠 Software and Libraries

### Tools Used:
- **Arduino IDE** for microcontroller programming.
- **Tinkercad** for 3D simulation and circuit design.

### Programming Languages:
- C / C++

### Arduino Libraries:
- `LiquidCrystal_I2C`
- `Wire`
- `OneWire`
- `MAX30105`
- `DallasTemperature`

### ESP-01 Libraries:
- `ESP8266WiFi`
- `ThingSpeak`

---

## 🧬 System Architecture

1. **Arduino Uno** controls the MAX3010x, DS18B20 and buzzer.
2. **MAX3010x** detects IR interruption.
3. **ESP-01** acts as a data server and send data to ThingSpeak dashboard.
4. **LCD** shows status messages.
5. **Power Bank** supplies portable power.

---

## 💻 Code Highlights

### Arduino Sketch (Main Logic)
- Reads MAX3010x, and DS18B20 values.
- Activates buzzer on abnormal values.
- Displays "SpO2/BPM" and "Temperature" on LCD.
- Forwards values to ESP-01.

### NodeMCU Sketch
- Connects to Wi-Fi.
- Hosts a web server with vital values.
- Sends vital values through ThingSpeak API.

---

## 🖼️ Project Output (Visual Summary)

- LCD showing real-time status.
- Circuit shows back/front view.
- Working model simulated in Tinkercad.

---

## ⚠️ Challenges Faced

- Getting legit values from MAX3010x as it is not an medical certified component
- Sensor calibration to avoid false positives.
- Wi-Fi and serial communication instability.
- Power fluctuations during operation.

---

## 🚀 Future Enhancements

- Mobile app with remote control.
- Certified sensor for better reaading accuracy.
- Push notifications and cloud logging.
- Integration with smart home ecosystems.

---

## 📅 Timeline

- **Start Date:** 26th August 2025
- **End Date:** 17th November 2025

---

## 📚 References

- Arduino Tutorials & Sketches
- Tinkercad Circuit Simulation

---

## 📸 Screenshots 
## Font View
![Front View](Images/20241109_174059.jpg)

## Circuit Diagram
![Circuit Diagram](https://github.com/user-attachments/assets/fc315aa3-4d5e-4b53-97bf-2a72522916df)


---

## 🏁 Conclusion

The **IoT-based Health Monitoring System** successfully showcases a responsive and real-time human body vitals with an cloud integration. Whith a room for scalable and future enhancement, we can use more portable sensor in orfer to get extra vitals. It is a very good start for a portable DIY cloud-based health monitoring.
