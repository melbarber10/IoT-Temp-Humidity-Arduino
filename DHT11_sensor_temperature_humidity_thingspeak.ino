#include <SoftwareSerial.h>
#include "DHT.h"

#define RX 2
#define TX 3
#define dht_apin 11 // Pin sensor is connected to

// Initialize the DHT object with the pin and sensor type
DHT dhtObject(dht_apin, DHT11);

String AP = "";        // AP NAME
String PASS = "";     // AP PASSWORD
String API = "";      // Write API KEY
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
