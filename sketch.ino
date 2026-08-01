#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ---------- CONFIG ----------
const char* AP_SSID     = "GateControl";
const char* AP_PASSWORD = "12345678";

#define SERVO_PIN   23
#define GREEN_PIN   19

int OPEN_ANGLE   = 90;
int CLOSED_ANGLE = 0;

// ---------------------------

Servo gateServo;
WebServer server(80);

void setLED(bool isOpen) {
  digitalWrite(GREEN_PIN, isOpen ? HIGH : LOW); // ON when open, OFF when closed
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Gate Control</title>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;margin-top:60px;background:#f2f2f2;}";
  html += "button{font-size:24px;padding:20px 40px;margin:15px;border:none;border-radius:10px;color:white;cursor:pointer;}";
  html += ".open{background-color:#2ecc71;}";
  html += ".close{background-color:#e74c3c;}";
  html += "</style></head><body>";
  html += "<h1>Gate Control</h1>";
  html += "<button class='open' onclick=\"fetch('/open')\">OPEN</button><br>";
  html += "<button class='close' onclick=\"fetch('/close')\">CLOSE</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleOpen() {
  gateServo.write(OPEN_ANGLE);
  setLED(true);
  server.send(200, "text/plain", "Opened");
}

void handleClose() {
  gateServo.write(CLOSED_ANGLE);
  setLED(false);
  server.send(200, "text/plain", "Closed");
}

void handleOpenTest() {
  gateServo.write(OPEN_ANGLE);
  setLED(true);
  Serial.println("TEST: Opened");
}

void handleCloseTest() {
  gateServo.write(CLOSED_ANGLE);
  setLED(false);
  Serial.println("TEST: Closed");
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_PIN, OUTPUT);

  gateServo.setPeriodHertz(50);
  gateServo.attach(SERVO_PIN, 500, 2400);
  gateServo.write(CLOSED_ANGLE);
  setLED(false);

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.on("/close", handleClose);
  server.begin();
}

void loop() {
  server.handleClient();

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'o') handleOpenTest();
    if (c == 'c') handleCloseTest();
  }
} 
