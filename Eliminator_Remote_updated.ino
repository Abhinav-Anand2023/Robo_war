#define BLYNK_TEMPLATE_ID "TMPL3B6fEOIXL"
#define BLYNK_TEMPLATE_NAME "Remote"
#define BLYNK_AUTH_TOKEN "ADKiIbAjhNcG8STFTKBPOew9lXB891Ni"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "RedmiD";
char pass[] = "9399097725";

// Pin for BTS to ESP32

// Motor 1 Pins
const int RPWM1 = 18;  
const int LPWM1 = 19;
const int REN1 = 21;        // REN1 used for motor 1 and motor 2 both 
const int LEN1 = 22;        // LEN1 used for motor 1 and motor 2 both 

// Motor 2 Pins
const int RPWM2 = 23;
const int LPWM2 = 25;

// Motor 3 Pins
const int RPWM3 = 26;
const int LPWM3 = 27;
const int REN2 = 32;        // REN2 used for motor 1 and motor 2 both 
const int LEN2 = 33;        // LEN2 used for motor 1 and motor 2 both 

// Motor 4 Pins
const int RPWM4 = 14;
const int LPWM4 = 15;

   
    int x = 0; // Replace with the value from V0
    int y = 0; // Replace with the value from V1





void setup() {
    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Set Motor 1 & 2 Enable Pins
    pinMode(REN1, OUTPUT);
    pinMode(LEN1, OUTPUT);

    // Set Motor 3 & 4 Enable Pins
    pinMode(REN2, OUTPUT);
    pinMode(LEN2, OUTPUT);

    // Set Motor PWM Pins
    pinMode(RPWM1, OUTPUT);
    pinMode(LPWM1, OUTPUT);
    pinMode(RPWM2, OUTPUT);
    pinMode(LPWM2, OUTPUT);
    pinMode(RPWM3, OUTPUT);
    pinMode(LPWM3, OUTPUT);
    pinMode(RPWM4, OUTPUT);
    pinMode(LPWM4, OUTPUT);
 }




BLYNK_WRITE(V0) {
    int X = param.asInt();
    // Serial.print("X-axis: ");
    // Serial.println(X);
    x=X;
}

BLYNK_WRITE(V1) {
    int Y = param.asInt();
    // Serial.print("Y-axis: ");
    // Serial.println(Y);
    y=Y;
}

   
void loop() {
    Blynk.run();

    // Stop
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


void forward() {
  //forward
  Serial.println("Forward");
  
  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, LOW);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, LOW);

  digitalWrite(RPWM1, y);
  digitalWrite(LPWM1, 0);
  digitalWrite(RPWM2, y);
  digitalWrite(LPWM2, 0);
  digitalWrite(RPWM3, y);
  digitalWrite(LPWM3, 0);
  digitalWrite(RPWM4, y);
  digitalWrite(LPWM4, 0);
}


void backward() {
  //Backward
  Serial.println("Backward");
  
  digitalWrite(REN1, LOW);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, LOW);
  digitalWrite(LEN2, HIGH);

  digitalWrite(RPWM1, 0);
  digitalWrite(LPWM1, -y);
  digitalWrite(RPWM2, 0);
  digitalWrite(LPWM2, -y);
  digitalWrite(RPWM3, 0);
  digitalWrite(LPWM3, -y);
  digitalWrite(RPWM4, 0);
  digitalWrite(LPWM4, -y);
}


void stop() {
  //Stop
  Serial.println("Stop");
  
  digitalWrite(REN1, LOW);
  digitalWrite(LEN1, LOW);
  digitalWrite(REN2, LOW);
  digitalWrite(LEN2, LOW);

  digitalWrite(RPWM1, 0);
  digitalWrite(LPWM1, 0);
  digitalWrite(RPWM2, 0);
  digitalWrite(LPWM2, 0);
  digitalWrite(RPWM3, 0);
  digitalWrite(LPWM3, 0);
  digitalWrite(RPWM4, 0);
  digitalWrite(LPWM4, 0);
}


void left() {
  //Left
  Serial.println("Left"); 
  
  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  digitalWrite(RPWM1, 0);
  digitalWrite(LPWM1, 255);
  digitalWrite(RPWM2, 255);
  digitalWrite(LPWM2, 0);
  digitalWrite(RPWM3, 0);
  digitalWrite(LPWM3, 255);
  digitalWrite(RPWM4, 255);
  digitalWrite(LPWM4, 0);
}


void right() {
  //Right
  Serial.println("Right");

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  digitalWrite(RPWM1, 255);
  digitalWrite(LPWM1, 0);
  digitalWrite(RPWM2, 0);
  digitalWrite(LPWM2, 255);
  digitalWrite(RPWM3, 255);
  digitalWrite(LPWM3, 0);
  digitalWrite(RPWM4, 0);
  digitalWrite(LPWM4, 255);
}


void forwardRight() {
  //Forward Right
  Serial.println("Forward_Right");
 
  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, LOW);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, LOW);

  digitalWrite(RPWM1, y+50);
  digitalWrite(LPWM1, 0);
  digitalWrite(RPWM2, x+40);
  digitalWrite(LPWM2, 0);
  digitalWrite(RPWM3, y+50);
  digitalWrite(LPWM3, 0);
  digitalWrite(RPWM4, x+40);
  digitalWrite(LPWM4, 0);
}


void forwardLeft() {
  //Forward Left
  Serial.println("Forward_Left");
 
  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, LOW);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, LOW);

  digitalWrite(RPWM1, ((-x)+40));
  digitalWrite(LPWM1, 0);
  digitalWrite(RPWM2, y+50);
  digitalWrite(LPWM2, 0);
  digitalWrite(RPWM3, ((-x)+40));
  digitalWrite(LPWM3, 0);
  digitalWrite(RPWM4, y+50);
  digitalWrite(LPWM4, 0);
}


void backwardRight() {
  //Backward Right
  Serial.println("BackwardRight");
 
  digitalWrite(REN1, LOW);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, LOW);
  digitalWrite(LEN2, HIGH);

  digitalWrite(RPWM1, 0);
  digitalWrite(LPWM1, ((-y)+50));
  digitalWrite(RPWM2, 0);
  digitalWrite(LPWM2, x+40);
  digitalWrite(RPWM3, 0);
  digitalWrite(LPWM3, ((-y)+50));
  digitalWrite(RPWM4, 0);
  digitalWrite(LPWM4, x+40);
}


void backwardLeft() {
  //Backward Left
  Serial.println("Backward_left");

  digitalWrite(REN1, LOW);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, LOW);
  digitalWrite(LEN2, HIGH);

  digitalWrite(RPWM1, 0);
  digitalWrite(LPWM1, ((-x)+40));
  digitalWrite(RPWM2, 0);
  digitalWrite(LPWM2, ((-y)+50));
  digitalWrite(RPWM3, 0);
  digitalWrite(LPWM3, ((-x)+40));
  digitalWrite(RPWM4, 0);
  digitalWrite(LPWM4, ((-y)+50));
}


