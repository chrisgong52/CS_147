#include <Arduino.h>
#include <Servo.h>
Servo myservo; // create servo object to control a servo
int k_max = 0;
int k_min = 1000000;
double k_base_unit = 1;
void setup()
{
  Serial.begin(9600);
  sleep(1);
  myservo.attach(27); // attaches the servo on port 27 to the servo object
  for (int i = 0; i < 10; i++)
  {
    analogWrite(17,1);
    int x = analogRead(36);
    if (x > k_max)
    {
      k_max = x;
    }
    if (x < k_min)
    {
      k_min = x;
    }
    Serial.print(k_max);
    Serial.print(' ');
    Serial.print(k_min);
    Serial.print(' ');
    Serial.print(i);
    Serial.print('\n');
    usleep(500000);
    analogWrite(17,0);
    usleep(500000);
  }
  k_base_unit = (k_max - k_min) / 180;
}

void loop()
{
  int cur_read = analogRead(36);
  if (cur_read > k_max) {
    cur_read = k_max;
  }
  if (cur_read < k_min) {
    cur_read = k_min;
  }
  double cur_deg = (cur_read - k_min) / k_base_unit;
  // move servo to degree
  myservo.write(cur_deg);
  sleep(1);
  }