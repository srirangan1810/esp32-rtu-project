# ESP32 Remote Telemetry Unit (RTU)

## 📌 Overview
This project demonstrates a simple IoT-based Remote Telemetry Unit using ESP32.  
The device reads temperature and humidity data from a DHT11 sensor and sends it to the cloud.

If the internet connection is lost, the data is stored locally in a buffer and sent later when the connection is restored.

---

## 🚀 Features
- Temperature & Humidity monitoring (DHT11)
- Data upload to cloud (ThingSpeak)
- Offline buffer storage
- Automatic resend after reconnect
- Reliable data handling (no data loss)

---

## 🛠 Hardware Used
- ESP32
- DHT11 Sensor

---

## ⚙️ Working Principle

1. Sensor reads temperature and humidity
2. If WiFi is available → Data sent to cloud
3. If WiFi is not available → Data stored in buffer
4. When WiFi reconnects → Buffered data is sent

---

## 📊 Architecture

Sensor → ESP32 → Cloud  
      ↓  
    Buffer (offline storage)

---

## 📽 Demo Video
https://youtu.be/hI97HibXyDI?feature=shared

---

## 📡 Cloud Platform
ThingSpeak is used for storing and visualizing sensor data.

---

## ⚠️ Limitations
- ThingSpeak has 15 seconds update limit
- Limited buffer size (RAM based)

---

## 🔮 Future Improvements
- Use MQTT for real-time communication
- Add SD card for large data storage
- Add more sensors

---

## 👨‍💻 Author
SwathiSri
