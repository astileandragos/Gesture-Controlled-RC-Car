# Gesture-Controlled RC Car

A wireless RC vehicle controlled via hand gestures, mapping IMU tilt data to motor control commands over RF.

## Hardware & Tech Stack
- **Microcontrollers:** 1x Arduino Nano (Glove), 1x Arduino Uno (Car)
- **Sensors & Modules:** ICM-20948 IMU, nRF24L01 (Wireless Transceiver), L298N (Motor Driver)
- **Language:** Arduino C++

## Key Features
- **Dynamic Calibration & Filtering:** The transmitter calibrates the IMU on startup and uses a low-pass filter to smooth the data.
- **Threshold-Based Commands:** Uses activation thresholds to prevent jitter and send stable directional commands.
- **Failsafe Mechanism:** The receiver implements a timeout system; if the radio signal is lost, the motors stop automatically.
- **H-Bridge Control:** The L298N driver uses PWM signals to manage motor speed and direction.

## Setup & Run
1. Wire the **Transmitter (Glove)**: Connect the ICM-20948 (I2C) and nRF24L01 (SPI) to the Arduino Nano.
2. Wire the **Receiver (Car)**: Connect the nRF24L01 (SPI) and L298N to the Arduino Uno.
3. Upload `emitter_glove.ino` to the Arduino Nano. Keep it still during startup for calibration.
4. Upload `receiver_car.ino` to the Arduino Uno.
