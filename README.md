# Glove-controlled Robot
A robot powered by two ESP32s that can navigate using the motions of your hand with built-in obstacle avoidance.

![robot](https://github.com/user-attachments/assets/882f129f-3bcb-4b50-8b26-9ed6bcec5720)


## Parts required
- 2 ESP32s
- Car Chassis Kit
- 3 Rechargeable Lithium Ion Batteries
- Battery Charger
- 4.5 inch Flex Sensor
- Ultrasonic Sensor
- Wires

## Step 1: Assembly
Create the robot car using any tutorial online.
For the Motor Driver L289n pinout:
![circuit1](https://github.com/user-attachments/assets/706bb0cb-d84e-46ec-83e2-0674a53f0bb1)

For the flex sensor pinout:
![circuit2](https://github.com/user-attachments/assets/8c488486-a092-4100-a89f-9b6306502109)

For the ultrasonic sensor pinout:
![circuit3](https://github.com/user-attachments/assets/017cb57c-8ab9-427d-8cc6-ee4250f624d6)

Credits to Random Nerd Tutorials for the circuit diagrams.


## Step 2: Run The Code
Lastly, it is time to test the code. Download flex.ino and upload on the ESP32 connected to the glove. Download robot.ino and upload on the ESP32 connected to the robot. Wear the glove, power up both ESP32s and navigate using hand motions.

## License
GPL-3.0

## Author
[Zoha Ahmed](https://github.com/zoha-a)
