// Serial servo pantilt interface
// fetter aka phetter aka bangdollar aka yourmomma 2(g)16

#include <Arduino.h>
#include <Servo.h>

#define SERVO_TILT_PIN 9
#define SERVO_PAN_PIN 8
#define SERVO_FIRE_PIN 3

#define SERVO_MOVE_WAIT_TIME 250
#define SERVO_FIRE_WAIT_TIME 100

#define LAUNCH_MOTOR_PIN 5

boolean launch_state = 0;


int servo_panPosition = 0;
int servo_tiltPosition = 0;

Servo servo_pan;
Servo servo_tilt;

String inputStr = "";             // a string to hold incoming data
boolean serialCmdReady = false;   // is a serial command ready to process?

String argStr;



void setup() {
  inputStr.reserve(256);
  argStr.reserve(4);

  servo_pan.attach(SERVO_PAN_PIN); //pin 9 on arduino
  servo_tilt.attach(SERVO_TILT_PIN); //pin 9 on arduino

  pinMode(LAUNCH_MOTOR_PIN, OUTPUT);
  pinMode(SERVO_FIRE_PIN, OUTPUT);

  Serial.begin(115200);  // Activate Serial protocol
  Serial.println("Listening for serial commands...");
}


void loop() {
  if (serialCmdReady) {
    // <pan>_<tilt>
    // <000-180>_<000-180>'\0'
    if (inputStr.length() == 8) {
      argStr = inputStr.substring(0,3);
      servo_panPosition = (int) strtol(&argStr[0], NULL, 10);
      // Serial.println(argStr);

      argStr = inputStr.substring(4,7);
      servo_tiltPosition = (int) strtol(&argStr[0], NULL, 10);
      // Serial.println(argStr);

      Serial.println("MOVING");

      setPan(servo_panPosition);
      setTilt(servo_tiltPosition);

      Serial.print("Pan: ");
      Serial.println(servo_panPosition);
      Serial.print("Tilt: ");
      Serial.println(servo_tiltPosition);
    }
    else if (inputStr.length() == 1) {
      fire();
    } else if (inputStr.length() == 2) {
      Serial.print("Launch Motors:");
      if (launch_state) {
        launch_state = 0;
        digitalWrite(LAUNCH_MOTOR_PIN, LOW);
      } else {
        launch_state = 1;
        digitalWrite(LAUNCH_MOTOR_PIN, HIGH);
      }
      Serial.println(launch_state);
    }
    else {
      Serial.print("READ SYNTAX ERROR: ");
      Serial.println(inputStr);
    }

    inputStr = "";
    serialCmdReady = false;
  }
}


// set tilt servo position in degrees
void setTilt(int deg)
{
  servo_tilt.write(deg);
  delay(SERVO_MOVE_WAIT_TIME);
}


// set pan servo position in degrees
void setPan(int deg)
{
  servo_pan.write(deg);
  delay(SERVO_MOVE_WAIT_TIME);
}


void fire()
{
  Serial.println("FIRING");
  digitalWrite(SERVO_FIRE_PIN, HIGH);
  delay(SERVO_FIRE_WAIT_TIME);
  digitalWrite(SERVO_FIRE_PIN, LOW);
  delay(SERVO_FIRE_WAIT_TIME);
}


// serialEvent() gets called implicitly every loop
// see: https://www.arduino.cc/en/Tutorial/SerialEvent
void serialEvent() {
  if (Serial) {
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char) Serial.read();
      // add it to the inputString:

      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (inChar == '\n') {
        serialCmdReady = true;
        inputStr += '\0';
      }
      else {
        inputStr += inChar;
      }
    }
  }
  else {
    Serial.begin(115200);
    Serial.println("Listening for serial commands...");
  }
}
