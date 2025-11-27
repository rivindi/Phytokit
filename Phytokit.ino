#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 9, 8);
SoftwareSerial s(5, 6);

int spd = 1000;
int sign = 1;
int ldr;
int x = 0;
int data1 = 0, data2 = 0;

void setup() {
  Serial.begin(115200);
  s.begin(115200);
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(1000);

  pinMode(12, OUTPUT);
  pinMode(10, OUTPUT);

  datarecv();  // Initial JSON data read
}

void loop() {
  ldr = analogRead(A0);
  Serial.println(ldr);

  if (ldr < 300) {
    stepper.setSpeed(0);
    stepper.runSpeed();
    watering();
  } else {
    x = 0;
    stepper.setSpeed(40);
    stepper.runSpeed();
  }
}

void watering() {
  digitalWrite(12, LOW);  // Activate pump
  delay(data2);
  digitalWrite(12, HIGH);  // Deactivate pump
  delay(100);
  stepper.setSpeed(100);
  stepper.runSpeed();
}

void datarecv() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);

  if (!root.success()) {
    Serial.println("JSON parse failed");
    return;
  }

  data1 = root["data1"];
  Serial.println(data1);

  data2 = root["data2"];
  Serial.println(data2);
}

