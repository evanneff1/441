#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "workstationWifi";
const char* password = "Zorp7825!"; 
const char* mqtt_server = "192.168.137.199";

WiFiClient espClient;
PubSubClient client(espClient);

// Ultrasonic Sensor pins
#define echoPin D2
#define trigPin D3

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

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static unsigned long lastMeasurementTime = 0;
  long duration, distance;

  if (millis() - lastMeasurementTime >= 50) {
    lastMeasurementTime = millis();
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1;

    if (distance < 5) {
      publishMessage("flashred");
      Serial.println("BACK UP");
    } else if (distance < 10) {
      publishMessage("red");
      Serial.println("Too close! Turn on red");
    } else if (distance < 20) {
      publishMessage("yellow");
      Serial.println("Start to slow down. Turn on yellow");
    } else {
      publishMessage("green");
      Serial.println("Keep going. Turn on green");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishMessage(const char* message) {
  client.publish("distance/status", message);
}
