#define BLYNK_TEMPLATE_ID "TMPL3B6fEOIXL"
#define BLYNK_TEMPLATE_NAME "Remote"
#define BLYNK_AUTH_TOKEN "ADKiIbAjhNcG8STFTKBPOew9lXB891Ni"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// WiFi Credentials
char ssid[] = "RedmiD";
char pass[] = "9399097725";

// Motor Pin Definitions
// Motor 1
const int RPWM1 = 18;  
const int LPWM1 = 19;
const int REN1 = 21;  // Shared by Motor 1 & 2
const int LEN1 = 22;

// Motor 2
const int RPWM2 = 23;
const int LPWM2 = 25;

// Motor 3
const int RPWM3 = 26;
const int LPWM3 = 27;
const int REN2 = 32;  // Shared by Motor 3 & 4
const int LEN2 = 33;

// Motor 4
const int RPWM4 = 14;
const int LPWM4 = 15;

// LEDC PWM Configuration
const int PWM_FREQ = 10000;  // 10 kHz for BTS7960
const int PWM_RES = 8;       // 8-bit resolution

// LEDC Channel Assignments
#define CH_RPWM1 0
#define CH_LPWM1 1
#define CH_RPWM2 2
#define CH_LPWM2 3
#define CH_RPWM3 4
#define CH_LPWM3 5
#define CH_RPWM4 6
#define CH_LPWM4 7

// Joystick Values
int x = 0;
int y = 0;

bool frontEnabled = false;  // Controls motors 1 & 2
bool rearEnabled = false;   // Controls motors 3 & 4

String connectionStatus = "Disconnected";
unsigned long lastUpdate = 0;
const long updateInterval = 2000; // Update every 2 seconds

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Enable Pin Setup
  pinMode(REN1, OUTPUT);
  pinMode(LEN1, OUTPUT);
  pinMode(REN2, OUTPUT);
  pinMode(LEN2, OUTPUT);

  // LEDC PWM Setup
  // Motor 1
  ledcSetup(CH_RPWM1, PWM_FREQ, PWM_RES);
  ledcAttachPin(RPWM1, CH_RPWM1);
  ledcSetup(CH_LPWM1, PWM_FREQ, PWM_RES);
  ledcAttachPin(LPWM1, CH_LPWM1);

  // Motor 2
  ledcSetup(CH_RPWM2, PWM_FREQ, PWM_RES);
  ledcAttachPin(RPWM2, CH_RPWM2);
  ledcSetup(CH_LPWM2, PWM_FREQ, PWM_RES);
  ledcAttachPin(LPWM2, CH_LPWM2);

  // Motor 3
  ledcSetup(CH_RPWM3, PWM_FREQ, PWM_RES);
  ledcAttachPin(RPWM3, CH_RPWM3);
  ledcSetup(CH_LPWM3, PWM_FREQ, PWM_RES);
  ledcAttachPin(LPWM3, CH_LPWM3);

  // Motor 4
  ledcSetup(CH_RPWM4, PWM_FREQ, PWM_RES);
  ledcAttachPin(RPWM4, CH_RPWM4);
  ledcSetup(CH_LPWM4, PWM_FREQ, PWM_RES);
  ledcAttachPin(LPWM4, CH_LPWM4);

  // Initial status
  Blynk.virtualWrite(V4, "Connecting...");

}

// Update status on Blynk connection/disconnection
BLYNK_CONNECTED() {
  connectionStatus = "Wi-Fi: Connected | Blynk: Connected | Signal: " + String(WiFi.RSSI()) + "dBm";
  Blynk.virtualWrite(V4, connectionStatus);
}

BLYNK_DISCONNECTED() {
  connectionStatus = "Wi-Fi: Connected | Blynk: Disconnected | Signal: " + String(WiFi.RSSI()) + "dBm";
  Blynk.virtualWrite(V4, connectionStatus);
}


BLYNK_WRITE(V0) { x = param.asInt(); }
BLYNK_WRITE(V1) { y = param.asInt(); }

BLYNK_WRITE(V2) { 
  frontEnabled = param.asInt(); // Toggle front motors
}

BLYNK_WRITE(V3) { 
  rearEnabled = param.asInt(); // Toggle rear motors
}







// Add this function to update signal strength
void updateSignal() {
  if (WiFi.status() == WL_CONNECTED) {
    int rssi = WiFi.RSSI(); // Get signal strength
    Blynk.virtualWrite(V4, "Wi-Fi: Connected | Signal: " + String(rssi) + "dBm");
  } else {
    Blynk.virtualWrite(V4, "Wi-Fi: Disconnected");
  }
}


void loop() {
  Blynk.run();

  // Update connectivity and signal strength every 2 seconds
    static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();
    updateSignal();
  }

    if(x==0 && y==0){
          stop();
     }
   // Forward
     else if(x >= -106 && x <= 106 && y >= 0 && y <= 255){
          forward();
     }
          
  // Backward
     else if(x >= -106 && x <= 106 && y >= -255 && y <= 0){
          backward();
     }
     
  // Left
     else if(x >= -255 && x <= 0 && y >= -106 && y <= 106){
          left();
     }
     
  // Right
     else if(x >= 0 && x <= 255 && y >= -106 && y <= 106){
          right();
     }

  // Forward Right
     else if(x > 0 && x < 255 && y > 106 && y < 255){
          forwardRight();
     }

  // Forward Left
     else if(x > -255 && x < 0 && y > 106 && y < 255){
          forwardLeft();
     }

  // Backward Right
     else if(x > 0 && x < 255 && y > -255 && y < -106){
          backwardRight();
     }

  // Backward Left
     else if(x >= -255 && x <= 0 && y >= -255 && y <= -106){
          backwardLeft();
     }
}

// Motor Control Functions
void forward() {
  Serial.println("Forward");

  // Front motors (1 & 2)
  if (frontEnabled) {
    digitalWrite(REN1, HIGH);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, y);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, y);
    ledcWrite(CH_LPWM2, 0);
    Serial.println("Front Wheel Drive");
  }
   else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, HIGH);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, y);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, y);
    ledcWrite(CH_LPWM4, 0);
    Serial.println("Rear Wheel Drive");
  }
   else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

void backward() {
  //Backward
  Serial.println("Backward");
  
  if (frontEnabled) {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, HIGH);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, -y);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, -y);
    Serial.println("Front Wheel Drive");
  }
   else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, HIGH);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, -y);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, -y);
    Serial.println("Rear Wheel Drive");
  }
   else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

void stop() {
  //Stop
  Serial.println("Stop");
  
  if (frontEnabled) {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
    Serial.println("Front Wheel Drive");
  }
   else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
    Serial.println("Rear Wheel Drive");
  }
   else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

void left() {
  Serial.println("Left");

  // Front motors (1 & 2)
  if (frontEnabled) {
    digitalWrite(REN1, HIGH);
    digitalWrite(LEN1, HIGH);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 255);
    ledcWrite(CH_RPWM2, 255);
    ledcWrite(CH_LPWM2, 0);
    Serial.println("Front Wheel Drive");
  } else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, HIGH);
    digitalWrite(LEN2, HIGH);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 255);
    ledcWrite(CH_RPWM4, 255);
    ledcWrite(CH_LPWM4, 0);
    Serial.println("Rear Wheel Drive");
  } else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

void right() {
  //Right
  Serial.println("Right");

  if (frontEnabled) {
    digitalWrite(REN1, HIGH);
    digitalWrite(LEN1, HIGH);
    ledcWrite(CH_RPWM1, 255);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 255);
    Serial.println("Front Wheel Drive");
  } else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, HIGH);
    digitalWrite(LEN2, HIGH);
    ledcWrite(CH_RPWM3, 255);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 255);
    Serial.println("Rear Wheel Drive");
  } else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

void forwardRight() {
  //Forward Right
  Serial.println("Forward_Right");

  // Front motors (1 & 2)
  if (frontEnabled) {
    digitalWrite(REN1, HIGH);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, y+50);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, x+40);
    ledcWrite(CH_LPWM2, 0);
    Serial.println("Front Wheel Drive");
  }
   else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, HIGH);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, y+50);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, x+40);
    ledcWrite(CH_LPWM4, 0);
    Serial.println("Rear Wheel Drive");
  }
   else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

  void forwardLeft() {
  //Forward Left
  Serial.println("Forward_Left");
 
  // Front motors (1 & 2)
  if (frontEnabled) {
    digitalWrite(REN1, HIGH);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, ((-x)+40));
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, y+50);
    ledcWrite(CH_LPWM2, 0);
    Serial.println("Front Wheel Drive");
  }
   else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, HIGH);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, ((-x)+40));
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, y+50);
    ledcWrite(CH_LPWM4, 0);
    Serial.println("Rear Wheel Drive");
  }
   else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

void backwardRight() {
  //Backward Right
  Serial.println("BackwardRight");
 
  if (frontEnabled) {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, HIGH);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, ((-y)+50));
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, x+40);
    Serial.println("Front Wheel Drive");
  }
   else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, HIGH);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, ((-y)+50));
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, x+40);
    Serial.println("Rear Wheel Drive");
  }
   else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

void backwardLeft() {
  //Backward Left
  Serial.println("Backward_left");

  if (frontEnabled) {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, HIGH);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, ((-x)+40));
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, ((-y)+50));
    Serial.println("Front Wheel Drive");
  }
   else {
    digitalWrite(REN1, LOW);
    digitalWrite(LEN1, LOW);
    ledcWrite(CH_RPWM1, 0);
    ledcWrite(CH_LPWM1, 0);
    ledcWrite(CH_RPWM2, 0);
    ledcWrite(CH_LPWM2, 0);
  }

  // Rear motors (3 & 4)
  if (rearEnabled) {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, HIGH);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, ((-x)+40));
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, ((-y)+50));
    Serial.println("Rear Wheel Drive");
  }
   else {
    digitalWrite(REN2, LOW);
    digitalWrite(LEN2, LOW);
    ledcWrite(CH_RPWM3, 0);
    ledcWrite(CH_LPWM3, 0);
    ledcWrite(CH_RPWM4, 0);
    ledcWrite(CH_LPWM4, 0);
  }
}

