# RC And Autodrive Car
## Project Member
Tuan Kiet Le - Electrical Engineering
## Project Objective
This project is dedicated to study about how **ESP 32 WROOM microcontroller** works. More specific, this project aims to study how to use the WiFi mode of the ESP 32, how they communicate to each other through that module. Furthermore, this project also improves my PCB design skill. The expected outcomes of this project are:
* understanding how ESP 32 works to communicate,
* being able to combine electrical components such as motor driver, motors, ultrasonic sensors, joysticks, switches,
* designing PCB board for the car's controller,
## Introduction
In the realm of modern robotics and automation, the fusion of microcontroller technology with intelligent control systems has opened up new possibilities for hobbyists and engineers alike. This project explores the development of a versatile remote-controlled (RC) car utilizing the powerful ESP32 microcontroller. The ESP32, known for its robust computational capabilities and built-in Wi-Fi and Bluetooth support, serves as the brain of the car, enabling seamless communication and control.

The RC car operates in two distinct modes: manual and autonomous. In manual mode, the car responds to user commands transmitted via a wireless controller, offering precision and agility for direct operation. When switched to autonomous mode, the car transitions into a self-driving vehicle, leveraging sensor inputs and programmed algorithms to navigate its environment intelligently. This dual-mode functionality showcases the potential of integrating microcontrollers with automation to create a dynamic and interactive robotic system.

The project aims to demonstrate the capabilities of the ESP32 in handling real-time tasks such as motor control, wireless communication, and autonomous decision-making. By incorporating features like obstacle detection, path planning, and environment awareness, the autonomous driving mode highlights practical applications of concepts like IoT, machine learning, and robotics in a compact and affordable design. Whether for educational purposes, prototyping, or just for fun, this RC car project serves as a stepping stone into the exciting world of embedded systems and intelligent automation.
## Project Result
The RC car project utilizing the ESP32 microcontroller successfully met its objectives, delivering a system capable of operating in both manual and autonomous driving modes. Through meticulous design and implementation, the project demonstrated the ESP32's ability to manage complex tasks, including wireless communication, motor control, and real-time decision-making.

In manual mode, the car showcased precise responsiveness to user inputs transmitted via a Bluetooth controller. The seamless communication between the controller and the ESP32 ensured smooth control of the car’s movements, including acceleration, decceleration, turn left, right, and go forward. This mode provided a user-friendly interface for remote operation, proving the robustness and reliability of the system.

In autonomous mode, the car successfully transitioned into self-driving operations. Using sensor inputs from components such as ultrasonic sensors, the car effectively detected and avoided obstacles in its path. Real-time adjustments ensured the car could handle dynamic scenarios, demonstrating the potential of intelligent automation in compact robotics.

## Materials Used
### Controller
* 1 ESP 32 WROOM 38 pins.
* 2 Thumb Joysticks.
* 2 EG 1218 switches.
* 1 3.3V Li-Po Battery.
* 1 0.1uF Electrolytic Capacitor.
* 1 10uF Electrolytic Capacitor.
### Receiver
* 1 ESP 32 WROOM 38 pins.
* 3 HC-SR04 Ultrasonic Sensor.
* 3 330 Ohms Resistors.
* 3 100 Ohms Resistors.
* 1 Chasis Car Kit.
* 1 L298N Motor Driver.
* 1 9V Battery.
* 1 3.3V Li-Po Battery.
## Works Cited
* Get ESP32/ESP8266 MAC Address and Change It
  https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/.
* Getting Started with ESP - NOW
  https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/.
