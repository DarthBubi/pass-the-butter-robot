/*
 PretzelBoard-Ultraschall-SN-SR04T
 Beispiel für PretzelBoard - Arduino Board mit WLAN
 Beschreibung des Boards und Beispiels auf der Webseite:
 (c) https://www.mikrocontroller-elektronik.de/
 Aufgabe:
 Entfernung Messen und auf seriellen Schnittstelle im Sekundentakt in cm ausgeben
 */

#define __AVR_ATmega328P__
#include <ros.h>
#include <sensor_msgs/Range.h>

const uint8_t trigger = 3;       // Arduino Pin an HC-SR04 Trig
const uint8_t echo = 2;          // Arduino Pin an HC-SR04 Echo
const uint8_t trigger_rear = 5;  // Arduino Pin an HC-SR04 Trig
const uint8_t echo_rear = 4;     // Arduino Pin an HC-SR04 Echo

const float fov = 0.2617993877991494;
const float range_min = 0.02;
const float range_max = 3.0;

ros::NodeHandle nh;
sensor_msgs::Range range_msg;
ros::Publisher range_finder_front_pub("/front_ultrasonic", &range_msg);
ros::Publisher range_finder_rear_pub("/rear_ultrasonic", &range_msg);

void setup()
{
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, HIGH);

  pinMode(trigger_rear, OUTPUT);
  pinMode(echo_rear, INPUT);
  digitalWrite(trigger_rear, HIGH);

  nh.initNode();
  nh.advertise(range_finder_front_pub);
  nh.advertise(range_finder_rear_pub);

  range_msg.radiation_type = range_msg.ULTRASOUND;
  range_msg.min_range = range_min;
  range_msg.max_range = range_max;
  range_msg.field_of_view = fov;
}

void loop()
{
  int distance = getDistance(0);

  range_msg.header.stamp = nh.now();
  range_msg.header.frame_id = "front_sonar";
  range_msg.range = static_cast<float>(distance) / 100.;
  range_finder_front_pub.publish(&range_msg);

  distance = getDistance(1);

  range_msg.header.stamp = nh.now();
  range_msg.header.frame_id = "rear_sonar";
  range_msg.range = static_cast<float>(distance) / 100.;
  range_finder_rear_pub.publish(&range_msg);

  nh.spinOnce();
  delay(50);
}

int getDistance(int sensor)
{
  long distance = 0;
  long time = 0;

  if (sensor == 0)
    digitalWrite(trigger, LOW);
  else if (sensor == 1)
    digitalWrite(trigger_rear, LOW);
  delayMicroseconds(5);
  noInterrupts();

  if (sensor == 0)
    digitalWrite(trigger, HIGH);  // Trigger Impuls 50 us
  else if (sensor == 1)
    digitalWrite(trigger_rear, HIGH);
  delayMicroseconds(50);

  if (sensor == 0)
    time = pulseIn(echo, HIGH);
  else if (sensor == 1)
    time = pulseIn(echo_rear, HIGH);

  // time = sensor == 0 ? pulseIn(echo, HIGH) : pulseIn(echo_rear, HIGH);  // Echo-Zeit messen
  interrupts();

  time = (time / 2);       // Zeit halbieren
  distance = time / 29.1;  // Zeit in Zentimeter umrechnen
  return distance;
}
