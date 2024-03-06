#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "workstationWifi";
const char* password = "Zorp7825!"; 

// MQTT Broker settings
const char* mqtt_server = "192.168.137.199"; 
const int mqtt_port = 1883;
const char* mqtt_user = "evan"; 
const char* mqtt_password = "791313"; 

// LED pins
const int greenLed = D8;
const int yellowLed = D7;
const int redLed = D6;

WiFiClient espClient;
PubSubClient client(espClient);

bool doorIsOpen = false; 

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

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == topicDoor) {
    doorIsOpen = (message == "opened");
  }

  if (doorIsOpen) {
    if (String(topic) == topicRed) {
      setLights(HIGH, LOW, LOW);
    } else if (String(topic) == topicYellow) {
      setLights(LOW, HIGH, LOW);
    } else if (String(topic) == topicGreen) {
      setLights(LOW, LOW, HIGH);
    } else if (String(topic) == topicFlashRed) {
      setLights(LOW, LOW, LOW);
      flashRedLed = true;
    }
  } else {
    setLights(LOW, LOW, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("StoplightClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("distance/status");
      client.subscribe("home/door"); 
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
  client.setCallback(callback);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setLights(bool red, bool yellow, bool green) {
  digitalWrite(redLed, red);
  digitalWrite(yellowLed, yellow);
  digitalWrite(greenLed, green);
}
