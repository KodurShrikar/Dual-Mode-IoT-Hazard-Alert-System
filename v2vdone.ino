#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <RF24.h>

const char* ssid = "vivo T1 5G";
const char* password = "RushitJani";
const char* mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);
RF24 radio(D2, D8); // CE, CSN pins (D2 = GPIO4, D8 = GPIO15 on NodeMCU)
const byte address[6] = "00001";

const int switchPin = D1; // Digital pin connected to the switch
const int ledPin = D6;    // Digital pin connected to LED
const int buzPin = D7;    // Digital pin connected to Buzzer

bool isTransmitter = false;
bool useMQTT = false;
unsigned long lastMQTTAttempt = 0;
const unsigned long mqttTimeout = 20000; // 20 seconds

const char* mqttClientId = "mqttx_34001bc4";
const char* mqttTopic = "testtopic/sender";

const float nearbyThreshold = 0.01; // Define a threshold to consider as nearby

void setup() {
  Serial.begin(9600);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzPin, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  radio.begin();
  radio.openReadingPipe(1, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);  // Set power level to low as NodeMCU operates at 3.3V

  Serial.println("Device setup complete");
}

void loop() {
  unsigned long currentMillis = millis();

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected() && currentMillis - lastMQTTAttempt >= mqttTimeout) {
      lastMQTTAttempt = currentMillis;
      if (client.connect(mqttClientId)) {
        useMQTT = true;
        client.subscribe(mqttTopic);
        Serial.println("MQTT connected and subscribed to topic");
      }
    }
  }

  if (useMQTT && !client.connected()) {
    useMQTT = false;
    Serial.println("MQTT connection lost, switching to NRF");
    radio.startListening();
  }

  if (useMQTT) {
    client.loop();
  } else {
    radioCommunication();
  }

  bool switchState = digitalRead(switchPin);

  if (switchState == LOW) {
    sendAlert();
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendAlert() {
  float latitude, longitude;
  getCoordinates(latitude, longitude);

  if (useMQTT) {
    StaticJsonDocument<200> doc;
    doc["alert"] = true;
    doc["latitude"] = latitude;
    doc["longitude"] = longitude;

    char message[200];
    serializeJson(doc, message);
    client.publish(mqttTopic, message);

    Serial.print("Sent alert: ");
    Serial.println(message);

    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
  } else {
    bool hazardAlert = true;
    radio.stopListening(); // Switch to transmitter mode
    radio.write(&hazardAlert, sizeof(hazardAlert));
    radio.startListening(); // Switch back to receiver mode

    Serial.print("Transmitted Hazard Alert via NRF: ");
    Serial.println(hazardAlert);
  }
}

void getCoordinates(float &latitude, float &longitude) {
  latitude = 40.0;  // Placeholder for actual GPS coordinates
  longitude = -75.0; // Placeholder for actual GPS coordinates
}

void callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, message);
  bool alert = doc["alert"];
  float latitude = doc["latitude"];
  float longitude = doc["longitude"];

  if (alert && isNearby(latitude, longitude)) {
    triggerAlert();
  } else {
    clearAlert();
  }
}

bool isNearby(float latitude, float longitude) {
  float deviceLatitude = 40.0;
  float deviceLongitude = -75.0;

  float latDiff = abs(deviceLatitude - latitude);
  float lonDiff = abs(deviceLongitude - longitude);

  return (latDiff <= nearbyThreshold && lonDiff <= nearbyThreshold);
}

void triggerAlert() {
  digitalWrite(buzPin, HIGH);
}

void clearAlert() {
  digitalWrite(buzPin, LOW);
}

void radioCommunication() {
  if (radio.available()) {
    bool receivedHazardAlert;
    radio.read(&receivedHazardAlert, sizeof(receivedHazardAlert));

    // Trigger alerts based on hazard alert status
    if (receivedHazardAlert) {
      triggerAlert();
    } else {
      clearAlert();
    }
  }
}
