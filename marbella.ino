// Marbella is a hacked RC car platform for autonomous driving research.

#include <Arduino.h>
#include <VescUart.h>
#include <SoftwareSerial.h>

#include "signals/low_pass_filter.hpp"
#include "control/steering_motor.hpp"

const int knobPin = A0;
const int steerPin = A1;
int motorPin = 5;

SteeringMotor steeringMotor(4);
SoftwareSerial bluetoothSerial(9, 10); // RX (interupt pin), TX

VescUart vesc;

unsigned long previousMillis = 0;
double previousError = 0.0;
unsigned long previousVescMillis = 0;

LowPassFilter throttleLowPassFilter(0.8);
LowPassFilter steeringLowPassFilter(0.8);

void setup() {
  // Inputs.
  pinMode(knobPin, INPUT);
  pinMode(steerPin, INPUT);
  // Outputs.
  pinMode(motorPin, OUTPUT);
  steeringMotor.init();

  // vesc setup
  Serial1.begin(115200);
  while (!Serial1) {;}
  vesc.setSerialPort(&Serial1);
  previousMillis = millis();
  previousVescMillis = millis();

  // bluetooth setup
  // bluetoothSerial.begin(9600);
}

void loop() {
  // read knob value
  int knobValue = analogRead(knobPin);
  // knobValue = throttleLowPassFilter.read(knobValue);
  // bluetoothSerial.print("knobValue: ");
  // bluetoothSerial.println(knobValue);

  // Throttle

  // Speed Controller
  double rpm = 0;
  double rpmDeadzone = 1000.0;
  double minRpm = 3000.0;
  double maxRpm = 14000.0;
  // double startCurrent = 5.0;
  // double deadzoneRampRate = 0.5;


  int rpmRequest = map(knobValue, 0, 1023, 0, maxRpm);
  rpmRequest = throttleLowPassFilter.read(rpmRequest);
  double dt = (millis() - previousMillis) / 1000.0;  // seconds

  if( (millis() - previousVescMillis) > 100 ) {
    if( vesc.getVescValues()) {
      rpm = vesc.data.rpm;
    }
    previousVescMillis = millis();
  }

  if( abs(rpmRequest) < rpmDeadzone ) {
    rpmRequest = 0;
  }

  if( abs(rpmRequest) <= minRpm && dt >= 0.001 ) {  // 1 ms control loop
    // low speed regime - pid torque control.
    double kp = 0.0025;
    double ki = 0.1;//0.08;
    double kd = 0.000004; // improves the smooth start up response.
    double error = rpmRequest - rpm;
    double integral = error * dt;
    double derivative = (error - previousError) / dt;
    double targetCurrent = kp * error + ki * integral + kd * derivative;
    vesc.setCurrent( targetCurrent );
    // bluetoothSerial.print("RPM Desired: ");
    // bluetoothSerial.print(rpmRequest);
    // bluetoothSerial.print(" RPM Actual: ");
    // bluetoothSerial.println(rpm);
    previousError = error;
    previousMillis = millis();
  }
  else if ( abs(rpmRequest) > minRpm ) {
    // high speed regime - vesc rpm control.
    vesc.setRPM(rpmRequest);
    // bluetoothSerial.print("RPM Desired: ");
    // bluetoothSerial.print(rpmRequest);
    // bluetoothSerial.print(" RPM Actual: ");
    // bluetoothSerial.println(rpm);
    double error = rpmRequest - rpm;
    previousError = error;
    previousMillis = millis();
  }
  // throttle feedback led
  analogWrite(motorPin, map(abs(rpmRequest), 0, maxRpm, 0, 255));

  // Steering
  int steerValue = analogRead(steerPin);
  steerValue = steeringLowPassFilter.read(steerValue);
  double angle = map(steerValue, 0, 1023, -90, 90);
  steeringMotor.setAngle(angle);
  // bluetoothSerial.print("Steering angle: ");
  // bluetoothSerial.println(steeringMotor.getAngle());
}
