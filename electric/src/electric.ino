
int motorPin = 5;

int analogPin = 3;

int val = 0;



void setup()

{
  pinMode(motorPin, OUTPUT);
  // pinMode(ledPin, OUTPUT);   // sets the pin as output
  Serial.begin(115200);
  Serial.println('Initialized...');
}



void loop()

{

  // val = analogRead(analogPin);   // read the input pin

  analogWrite(motorPin, 100);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255

  delay(50);
}
