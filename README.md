# Dual-Mode-IoT-Hazard-Alert-System
Overview
The IoT Hazard Alert System is designed to enhance vehicle safety through real-time hazard monitoring and Vehicle-to-Vehicle (V2V) communication. This system uses NodeMCU, NRF24L01 transceivers, and MQTT protocol to detect and share hazard information between vehicles, ensuring timely alerts and improved situational awareness.
Features
•	Real-time Hazard Detection: Monitors vehicle conditions and detects hazards such as sudden braking, road obstructions, or adverse weather.
•	Immediate Alerts: Transmits hazard alerts to nearby vehicles, enabling drivers to take evasive action promptly.
•	Early Warning System: Provides real-time alerts about potential collisions or dangerous road conditions.
•	Efficient Traffic Management: Shares information about traffic congestion, accidents, or road closures to optimize traffic flow.
•	Dynamic Route Planning: Suggests alternate routes based on real-time conditions.
Technologies Used
•	NodeMCU (ESP8266)
•	NRF24L01 Transceiver Modules
•	MQTT Protocol
•	Arduino IDE
•	C/C++ Programming Language
•	WiFi Connectivity
•	JSON Data Serialization
How It Works
1.	Setup: Configure the NodeMCU to connect to a WiFi network and set up the MQTT broker.
2.	Detection: Use sensors to monitor vehicle conditions and detect hazards.
3.	Communication: Use NRF24L01 modules for V2V communication and MQTT protocol for remote monitoring and control.
4.	Alerts: Transmit hazard alerts to nearby vehicles and update the system through MQTT.
Installation
1.	Clone the Repository:
sh
Copy code
https://github.com/KodurShrikar/Dual-Mode-IoT-Hazard-Alert-System
2.	Install Arduino IDE: Download and install the Arduino IDE.
3.	Install Required Libraries:
o	Install the ESP8266WiFi library
o	Install the PubSubClient library
o	Install the ArduinoJson library
4.	Upload the Code:
o	Open the Arduino IDE.
o	Load the provided code files.
o	Configure the ssid and password for your WiFi network.
o	Upload the code to the NodeMCU.
Connections
•	NodeMCU:
o	D1: Switch
o	D6: LED
o	D7: Buzzer
•	NRF24L01:
o	VCC: 3.3V
o	GND: GND
o	CE: D2
o	CSN: D8
o	SCK: D5
o	MOSI: D7
o	MISO: D6
Usage
1.	Power the NodeMCU and connect it to the WiFi network.
2.	Monitor the Serial output for system status and alerts.
3.	Use the switch to toggle between transmitter and receiver modes.
4.	In transmitter mode, send hazard alerts when necessary.
5.	In receiver mode, listen for incoming alerts and respond accordingly.
Conclusion
The IoT Hazard Alert System enhances vehicle safety with real-time hazard monitoring and V2V communication, improving situational awareness and reducing collision risk. It optimizes traffic flow and enables dynamic route planning, making road travel more efficient and safer.

FOR VIDEO REFER THE BELOW LINK
https://www.loom.com/share/c00e4c85b79a4822807196d8e5085e79
