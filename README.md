# ESP32 Remote Telemetry Unit (RTU)

## Overview
This project demonstrates an IoT-based RTU using ESP32.

## Features
- Temperature & Humidity monitoring (DHT11)
- Cloud data upload (ThingSpeak)
- Offline buffer storage
- Automatic resend after reconnect
- Remote command (sampling rate change)

## Hardware Used
- ESP32
- DHT11 Sensor

## Working
Sensor reads data → sends to cloud  
If WiFi fails → stores in buffer  
When WiFi returns → sends stored data  
Also receives command from cloud

## Demo
(You will add video link here)
