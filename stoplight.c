#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const int greenLed = D8;
const int yellowLed = D7;
const int redLed = D6;\
int blinkFreq = 500;
unsigned long blinkTimer = 0;


volatile bool flashRedLed = false;

// Your WiFi credentials
// const char* ssid = "AirPhone";
// const char* password = "BeatsFarm25!";
// volatile bool cycling = false;

const char* ssid = "HelloThere";
const char* password = "GeneralKenob1";


ESP8266WebServer server(80);  // Create a web server on port 80

void setup() {
  Serial.begin(115200);
  
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  server.on("/red", []() {
  flashRedLed = false;
  setLights(HIGH, LOW, LOW);
  server.send(200, "text/html", "<html><body>LED set to Red</body></html>");
});

server.on("/yellow", []() {
  flashRedLed = false;
  setLights(LOW, HIGH, LOW);
  server.send(200, "text/html", "<html><body>LED set to Yellow</body></html>");
});

server.on("/green", []() {
  flashRedLed = false;
  setLights(LOW, LOW, HIGH);
  server.send(200, "text/html", "<html><body>LED set to Green</body></html>");
});

server.on("/flashred", []() {
  setLights(HIGH, LOW, LOW);
  flashRedLed = true;  // Start flashing
  server.send(200, "text/html", "<html><body>Flashing Red LED</body></html>");
});

server.on("/stopflash", []() {
  flashRedLed = false;  // Stop flashing
  // setLights(LOW, LOW, LOW);
  server.send(200, "text/html", "<html><body>Stopped Flashing</body></html>");
});


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  server.handleClient();
  if (flashRedLed) {
    unsigned long currentMillis = millis();
    if (currentMillis - blinkTimer >= blinkFreq) {
      blinkTimer = currentMillis;  // Reset the timer

      // Toggle the red LED
      int ledState = digitalRead(redLed);  // Get the current state of the red LED
      digitalWrite(redLed, !ledState);  // Set the LED to the opposite state
    }
  }
}

void setLights(bool red, bool yellow, bool green) {
  digitalWrite(redLed, red);
  digitalWrite(yellowLed, yellow);
  digitalWrite(greenLed, green);
}
