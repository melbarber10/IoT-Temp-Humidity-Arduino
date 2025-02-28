# IoT Based Temperature & Humidity Monitor Using Arduino & ESP8266

## Overview
This project is a straightforward IoT solution that uses an Arduino, a DHT11 sensor, and an ESP8266 Wi-Fi module to measure and report temperature and humidity in real time. Data is sent to ThingSpeak, where you can monitor and analyze it online. The setup is designed to be as simple as it is effective—ideal for beginners and scalable for more advanced applications.

## Features
- **Real-Time Monitoring:** Continuously reads temperature and humidity.
- **IoT Connectivity:** Transmits data to ThingSpeak over Wi-Fi.
- **Customizable:** Easily update your Wi-Fi credentials and ThingSpeak API key.
- **Expandable:** Use as a base for more complex IoT projects.

## Hardware Requirements
- Arduino board (Uno, Mega, etc.)
- ESP8266 Wi-Fi module
- DHT11 sensor (temperature and humidity)
- Jumper wires and breadboard
- (Optional) Additional components based on your project design

## Software Requirements
- Arduino IDE
- Libraries:
  - [SoftwareSerial](https://www.arduino.cc/en/Reference/SoftwareSerial) (usually pre-installed)
  - [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) (install via Arduino Library Manager)

## Circuit Setup
Connect the components as follows:
- **DHT11 Sensor:** Connect the data pin to Arduino digital pin 11.
- **ESP8266 Module:** 
  - Connect RX to Arduino digital pin 2.
  - Connect TX to Arduino digital pin 3.
  
*(Tip: Ensure your ESP8266 is powered properly since it can be power-hungry.)*

## Code Walkthrough
The main sketch (e.g., `main.ino`) does the following:
1. **Initialization:**  
   - Sets up serial communication.
   - Initializes the DHT11 sensor on pin 11.
   - Configures the ESP8266 using AT commands.
2. **Wi-Fi Setup:**  
   - Switches the ESP8266 to station mode.
   - Connects to your specified Wi-Fi network.
3. **Data Transmission:**  
   - Reads temperature and humidity values.
   - Formats a GET request to ThingSpeak using your API key.
   - Sends the data over the network.
4. **Command Handling:**  
   - Uses a custom `sendCommand()` function to issue AT commands and verify responses.

### Code Snippet
Below is the core code used in this project:

```cpp
#include <SoftwareSerial.h>
#include "DHT.h"

#define RX 2
#define TX 3
#define dht_apin 11 // Pin sensor is connected to

// Initialize the DHT object with the pin and sensor type
DHT dhtObject(dht_apin, DHT11);

String AP = "";        // AP NAME
String PASS = "";      // AP PASSWORD
String API = "";       // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

SoftwareSerial esp8266(RX, TX);

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);

  // Initialize the DHT sensor
  dhtObject.begin();

  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop() {
  String getData = "GET /update?api_key=" + API + "&field1=" + getTemperatureValue() + "&field2=" + getHumidityValue();
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}

String getTemperatureValue() {
  float temperature = dhtObject.readTemperature();
  Serial.print(" Temperature(C)= ");
  Serial.println(temperature);
  return String(temperature);
}

String getHumidityValue() {
  float humidity = dhtObject.readHumidity();
  Serial.print(" Humidity in %= ");
  Serial.println(humidity);
  return String(humidity);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1)) {
    esp8266.println(command);
    if (esp8266.find(readReplay)) {
      found = true;
      break;
    }
    countTimeCommand++;
  }

  if (found == true) {
    Serial.println("DONE");
    countTrueCommand++;
    countTimeCommand = 0;
  } else {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}
```

## Getting Started
1. **Clone or Download the Repository:**
   ```bash
   git clone https://github.com/yourusername/IoT-Temp-Humidity-Arduino.git
   ```
2. **Open the Project in Arduino IDE:**
   - Open the main sketch file (e.g., `main.ino`).
3. **Install Required Libraries:**
   - Use the Library Manager to install the DHT sensor library.
4. **Configure Your Credentials:**
   - Update the `AP`, `PASS`, and `API` variables with your Wi-Fi SSID, Wi-Fi password, and ThingSpeak API key, respectively.
5. **Upload the Code:**
   - Connect your Arduino board and upload the code.

## Running the Project
Once the code is uploaded, power your Arduino setup. The ESP8266 will connect to your Wi-Fi network and start transmitting sensor data to ThingSpeak. Log into your ThingSpeak account to view and analyze the data in real time.

## Project Report
For a deeper dive into the project design, methodology, and analysis, refer to the attached `Project Report.pdf`. It contains comprehensive documentation of the project and insights into future enhancements.

## Troubleshooting
- **Wi-Fi Connection Failures:**
  - Double-check your SSID and password.
  - Ensure the ESP8266 module is correctly powered and wired.
- **Inaccurate Sensor Readings:**
  - Verify that the DHT11 sensor is securely connected.
  - Consider calibrating the sensor if necessary.
- **Communication Issues:**
  - Confirm that the SoftwareSerial pins are correctly assigned.
  - Check the baud rate settings for consistency.

## Contributing
If you have ideas to make this project even more lit, fork the repository and send a pull request. Feedback and improvements are always welcome.

---

