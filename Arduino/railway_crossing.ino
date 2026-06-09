#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gate;

// HC-SR04
const int trigPin = 9;
const int echoPin = 10;

// LEDs
const int greenLED = 2;
const int yellowLED = 3;
const int redLED = 4;

// Buzzer
const int buzzer = 5;

// Servo
const int servoPin = 6;

long duration;
float distance;

int trainCount = 0;

float getDistance()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);

digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH);

return duration * 0.034 / 2;
}

void setup()
{
Serial.begin(9600);

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

pinMode(greenLED, OUTPUT);
pinMode(yellowLED, OUTPUT);
pinMode(redLED, OUTPUT);

pinMode(buzzer, OUTPUT);

gate.attach(servoPin);

lcd.init();
lcd.backlight();

gate.write(0);

digitalWrite(greenLED, HIGH);
digitalWrite(yellowLED, LOW);
digitalWrite(redLED, LOW);

lcd.setCursor(0,0);
lcd.print("TRACK CLEAR");

lcd.setCursor(0,1);
lcd.print("GATE OPEN");

Serial.println("STATUS=TRACK_CLEAR");
Serial.print("COUNT=");
Serial.println(trainCount);
}

void loop()
{
distance = getDistance();

Serial.print("DIST=");
Serial.println(distance);

if(distance > 0 && distance < 15)
{
trainCount++;

```
Serial.println("STATUS=TRAIN_DETECTED");

Serial.print("COUNT=");
Serial.println(trainCount);

// Green -> Yellow
digitalWrite(greenLED, LOW);
digitalWrite(yellowLED, HIGH);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("TRAIN");
lcd.setCursor(0,1);
lcd.print("APPROACHING");

delay(1000);

// Buzzer Warning
for(int i = 0; i < 2; i++)
{
  digitalWrite(buzzer, HIGH);
  delay(200);

  digitalWrite(buzzer, LOW);
  delay(200);
}

// Yellow -> Red
digitalWrite(yellowLED, LOW);
digitalWrite(redLED, HIGH);

// Close Gate
gate.write(90);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("TRAIN PASSING");
lcd.setCursor(0,1);
lcd.print("GATE CLOSED");

Serial.println("STATUS=GATE_CLOSED");

delay(2000);

// Red -> Yellow
digitalWrite(redLED, LOW);
digitalWrite(yellowLED, HIGH);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("TRACK CLEAR");
lcd.setCursor(0,1);
lcd.print("PREPARING");

delay(1000);

// Open Gate
gate.write(0);

// Yellow -> Green
digitalWrite(yellowLED, LOW);
digitalWrite(greenLED, HIGH);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("TRACK CLEAR");
lcd.setCursor(0,1);
lcd.print("GATE OPEN");

Serial.println("STATUS=TRACK_CLEAR");

do
{
  distance = getDistance();
  delay(100);
}
while(distance < 15);
```

}

delay(100);
}
