#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ===== AP DETAILS =====
const char* ap_ssid = "ESP32-CAR";
const char* ap_password = "12345678"; // min 8 chars

AsyncWebServer server(80);

// ===== MOTOR PINS (L298N) =====
#define MOTOR1_PIN1 27
#define MOTOR1_PIN2 26
#define ENABLE1_PIN 14
#define MOTOR2_PIN1 33
#define MOTOR2_PIN2 25
#define ENABLE2_PIN 32

// ===== MOTOR FUNCTIONS =====
void moveForward() {
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
  analogWrite(ENABLE1_PIN, 200);
  analogWrite(ENABLE2_PIN, 200);
}

void moveBackward() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
  analogWrite(ENABLE1_PIN, 200);
  analogWrite(ENABLE2_PIN, 200);
}

void turnLeft() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
}

void turnRight() {
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

void stopMotor() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
  analogWrite(ENABLE1_PIN, 0);
  analogWrite(ENABLE2_PIN, 0);
}

// ===== WEB PAGE =====
const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 CAR</title>
<style>
button {
  width: 130px;
  height: 60px;
  font-size: 20px;
  margin: 6px;
}
</style>
</head>
<body>
<center>
<h2>ESP32 WiFi Car</h2>
<button onclick="fetch('/forward')">Forward</button><br>
<button onclick="fetch('/left')">Left</button>
<button onclick="fetch('/stop')">Stop</button>
<button onclick="fetch('/right')">Right</button><br>
<button onclick="fetch('/backward')">Backward</button>
</center>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(ENABLE1_PIN, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  pinMode(ENABLE2_PIN, OUTPUT);

  stopMotor();

  // ===== START AP MODE =====
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP()); // 192.168.4.1

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){
    r->send_P(200, "text/html", html);
  });

  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *r){ moveForward(); r->send(200); });
  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *r){ moveBackward(); r->send(200); });
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *r){ turnLeft(); r->send(200); });
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *r){ turnRight(); r->send(200); });
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *r){ stopMotor(); r->send(200); });

  server.begin();
}

void loop() {}
