#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "workstationWifi"; 
const char* password = "Zorp7825!"; 

// MQTT Broker settings
const char* mqtt_server = "192.168.137.199"; 
const int mqtt_port = 1883;
const char* mqtt_user = "evan"; 
const char* mqtt_password = "791313"; 

const char* topicDoor = "home/door";

const int doorSensorPin = D5; 

WiFiClient espClient;
PubSubClient client(espClient);

void setupWiFi() {
  Serial.begin(115200);
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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("DoorSensorClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  setupWiFi();
  client.setServer(mqtt_server, mqtt_port);

  pinMode(doorSensorPin, INPUT_PULLUP); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static bool lastDoorState = !digitalRead(doorSensorPin); 
  bool doorState = !digitalRead(doorSensorPin);

  if (doorState != lastDoorState) { 
    lastDoorState = doorState;

    if (doorState) {
      Serial.println("Door Closed");
      client.publish(topicDoor, "closed");
    } else {
      Serial.println("Door Opened");
      client.publish(topicDoor, "opened");
    }
