# Autonomous Sumo Robot

**Final Project – Automation Systems Course**  
An autonomous sumo robot designed and implemented as a final project for the Automation Systems course, focusing on **automatic control, sensor integration, and embedded system design**.

---

## Project Overview

This project involves the development of an **autonomous sumo robot** capable of navigating the arena, detecting opponents, avoiding boundary lines, and executing attack strategies without human intervention.

The robot operates using a **Finite State Machine (FSM)**–based control approach to ensure structured decision-making and reliable autonomous behavior in real-time conditions.

---

## System Architecture

The system is composed of the following main subsystems:

### 1. Sensing Subsystem
- **HC-SR04 Ultrasonic Sensor**  
  Used to detect the presence and distance of opponents.
- **Infrared Line Sensors**  
  Used to detect arena boundaries and prevent the robot from exiting the ring.

### 2. Control Subsystem
- **Microcontroller (ESP32)**  
  Acts as the main controller executing FSM-based logic for navigation, detection, and attack behavior.

### 3. Actuation Subsystem
- **DC Motors**  
  Provide robot movement and pushing force.
- **Motor Driver (L298N)**  
  Controls motor direction and speed using PWM signals.

---

## Control Logic

The robot behavior is implemented using a **Finite State Machine (FSM)** with the following states:
- **Idle / Search** – Robot scans the arena to locate an opponent  
- **Approach** – Robot moves toward the detected opponent  
- **Attack** – Robot pushes the opponent out of the arena  
- **Boundary Avoidance** – Robot retreats when the arena edge is detected  

This structured control logic improves system reliability and simplifies behavior expansion.

---

## Workflow

1. Initialize sensors, motors, and control parameters  
2. Continuously read ultrasonic and IR sensor data  
3. Determine robot state based on sensor inputs  
4. Execute movement commands according to FSM rules  
5. Adjust motor speed and direction using PWM control  

---

## Technologies Used

- **Microcontroller:** ESP32  
- **Sensors:** HC-SR04 Ultrasonic Sensor, Infrared Line Sensors  
- **Actuators:** DC Motors  
- **Motor Driver:** L298N  
- **Programming Language:** C/C++ (Arduino Framework)  
- **Concepts:** Automation Systems, Finite State Machine (FSM), Embedded Control, Sensor Integration  

---

## Hardware Requirements

- ESP32 Development Board  
- HC-SR04 Ultrasonic Sensor  
- Infrared Line Sensors  
- DC Motors  
- L298N Motor Driver  
- Battery Pack  
- Jumper Wires  

---

## Notes

- Sensor placement significantly affects detection accuracy  
- Motor speed tuning is critical for stability and maneuverability  
- The system is designed for **educational and experimental purposes**

---

## License

This project was developed for academic purposes as part of the  
**Automation Systems** course.
