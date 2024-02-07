#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const int ledGreen = D8;
const int ledYellow = D7;
const int ledRed = D6;
int blinkFreq = 2000;
unsigned long blinkTimer = 0;
int lightState = 0;  // Declare at the top of your sketch



// Your WiFi credentials
// const char* ssid = "AirPhone";
// const char* password = "BeatsFarm25!";
volatile bool cycling = false;

const char* ssid = "HelloThere";
const char* password = "GeneralKenob1";

// const char* ssid = "CenturyLink3023";
// const char* password = "7483eec364aa76";

ESP8266WebServer server(80);  // Create a web server on port 80

const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
<head><title>LED Control</title></head>
<body>
  <h1>LED Control</h1>
  <p>
    <a href="/led/green/on"><button>Green ON</button></a>
    <a href="/led/green/off"><button>Green OFF</button></a>
  </p>
  <p>
    <a href="/led/yellow/on"><button>Yellow ON</button></a>
    <a href="/led/yellow/off"><button>Yellow OFF</button></a>
  </p>
  <p>
    <a href="/led/red/on"><button>Red ON</button></a>
    <a href="/led/red/off"><button>Red OFF</button></a>
  </p>
  <p>
    <a href="/cycle/start"><button>Start Cycling</button></a>
    <a href="/cycle/stop"><button>Stop Cycling</button></a>
  </p>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlContent);
}

void cycleLights() {
  if (!cycling) return;  // Check before starting the cycle

    Serial.println("Green Start");
    Serial.println(blinkTimer);
    Serial.println(millis());
switch(lightState){
      case 0: // Green
      if (blinkTimer < millis()) {
        if (!cycling) return;  // Check before changing state
        digitalWrite(ledGreen, HIGH);
        lightState++;
        blinkTimer = millis() + blinkFreq;  // Set for 3 seconds later
        if (!cycling) return;                    // Check before changing state
      }
      break;
      case 1:
  Serial.println("Yellow Start");
   Serial.println(blinkTimer);
    Serial.println(millis());
 // Turn off Green, turn on Yellow
      if (blinkTimer < millis()) {
        if (!cycling) return;  // Check before changing state
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, HIGH);
        lightState++;
        blinkTimer = millis() + blinkFreq;  // Set for 3 seconds later
        if (!cycling) return;                    // Check before changing state
      }
      break;
      case 2:
        Serial.println("Red Start");
   Serial.println(blinkTimer);
    Serial.println(millis());
    // Turn off Yellow, turn on Red
      if (blinkTimer < millis()) {
        if (!cycling) return;  // Check before changing state
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledRed, HIGH);
        lightState++;
        blinkTimer = millis() + blinkFreq;  // Set for 3 seconds later
        if (!cycling) return;                    // Check before changing state
      }
      break;
      case 3:
  // Turn off Red and reset state
      if (blinkTimer < millis()) {
        if (!cycling) return;  // Check before changing state
        digitalWrite(ledRed, LOW);
        lightState = 0;
        blinkTimer = millis() + blinkFreq;  // Set for 3 seconds later
        if (!cycling) return;        // Check before changing state
      }
      break;
  }
}



void handleLedControl() {
  String led = server.arg("led");
  String state = server.arg("state");

  int ledPin;
  if (led == "green") ledPin = ledGreen;
  else if (led == "yellow") ledPin = ledYellow;
  else if (led == "red") ledPin = ledRed;
  else {
    server.send(404, "text/plain", "LED not found");
    return;
  }

  if (state == "on") digitalWrite(ledPin, HIGH);
  else if (state == "off") digitalWrite(ledPin, LOW);
  else {
    server.send(404, "text/plain", "Invalid state");
    return;
  }

  server.send(200, "text/html", htmlContent);
}

void setup() {
  Serial.begin(115200);        // Start the Serial communication
  WiFi.begin(ssid, password);  // Connect to Wi-Fi

  // Initialize LED pins as outputs
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/cycle/start", []() {
    cycling = true;
    lightState = 0;
    server.send(200, "text/html", htmlContent);
  });
  server.on("/cycle/stop", []() {
    cycling = false;
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    server.send(200, "text/html", htmlContent);
  });
  server.on("/led/green/on", []() {
    digitalWrite(ledGreen, HIGH);
    server.send(200, "text/html", htmlContent);
  });
  server.on("/led/green/off", []() {
    digitalWrite(ledGreen, LOW);
    server.send(200, "text/html", htmlContent);
  });
  server.on("/led/yellow/on", []() {
    digitalWrite(ledYellow, HIGH);
    server.send(200, "text/html", htmlContent);
  });
  server.on("/led/yellow/off", []() {
    digitalWrite(ledYellow, LOW);
    server.send(200, "text/html", htmlContent);
  });
  server.on("/led/red/on", []() {
    digitalWrite(ledRed, HIGH);
    server.send(200, "text/html", htmlContent);
  });
  server.on("/led/red/off", []() {
    digitalWrite(ledRed, LOW);
    server.send(200, "text/html", htmlContent);
  });

  server.begin();  // Start the server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Handle client requests

  if (cycling) {
    cycleLights();  // Cycle through lights if cycling mode is active
  }

}