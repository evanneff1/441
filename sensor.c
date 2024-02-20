#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

int blinkFreq = 2000;
unsigned long blinkTimer = 0;
int lightState = 0;  // Declare at the top of your sketch



// Your WiFi credentials
// const char* ssid = "AirPhone";
// const char* password = "BeatsFarm25!";
// volatile bool cycling = false;

const char* ssid = "HelloThere";
const char* password = "GeneralKenob1";


ESP8266WebServer server(80);  // Create a web server on port 80

// Ultrasonic Sensor pins
#define echoPin D2                 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin D3                 //attach pin D3 Arduino to pin Trig of HC-SR04



IPAddress serverIp(192, 168, 86, 24); // IP of the Stoplight Arduino

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
}

void loop() {
  static unsigned long lastMeasurementTime = 0;
  long duration, distance;

  if (millis() - lastMeasurementTime >= 50) { // Non-blocking delay of 50ms
    lastMeasurementTime = millis();
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1;

    if (distance < 5) {
      httpRequest("/flashred");
      Serial.println("BACK UP");
    } else if (distance < 10) {
      httpRequest("/red");
      Serial.println("Too close! Turn on red");
    } else if (distance < 20) {
      httpRequest("/yellow");
      Serial.println("Start to slow down. Turn on yellow");
    } else {
      httpRequest("/green");
      Serial.println("Keep going. Turn on green");
    }
  }
}

void httpRequest(const char* path) {
  WiFiClient client;
  if (client.connect(serverIp, 80)) {
    client.println(String("GET ") + path + " HTTP/1.1");
    client.println("Host: " + serverIp.toString());  // Convert IPAddress to String
    client.println("Connection: close");
    client.println();
  }
  while(client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\r');
      // Serial.print(line);
    }
  }
  client.stop();
}
