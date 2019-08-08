/*
 PretzelBoard-Ultraschall-SN-SR04T
 Beispiel für PretzelBoard - Arduino Board mit WLAN
 Beschreibung des Boards und Beispiels auf der Webseite:
 (c) https://www.mikrocontroller-elektronik.de/
 Aufgabe:
 Entfernung Messen und auf seriellen Schnittstelle im Sekundentakt in cm ausgeben
 */

// #include <SoftwareSerial.h>
#include <ros.h>
#include <sensor_msgs/Range.h>

#define trigger 3  // Arduino Pin an HC-SR04 Trig
#define echo 2     // Arduino Pin an HC-SR04 Echo
#define trigger_rear 5  // Arduino Pin an HC-SR04 Trig
#define echo_rear 4     // Arduino Pin an HC-SR04 Echo

constexpr float fov = 0.2617993877991494;
constexpr float range_min = 0.02;
constexpr float range_max = 3.0;

ros::NodeHandle nh;
sensor_msgs::Range range_msg;
ros::Publisher range_finder_front_pub("/front_ultrasonic", &range_msg);
ros::Publisher range_finder_rear_pub("/rear_ultrasonic", &range_msg);

// SoftwareSerial esp8266(11, 12);

void setup()
{
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, HIGH);  // Signal abschalten

  pinMode(trigger_rear, OUTPUT);
  pinMode(echo_rear, INPUT);
  digitalWrite(trigger_rear, HIGH);  // Signal abschalten

  // Open serial communications and wait for port to open:
  // Serial.begin(19200);

  nh.initNode();
  nh.advertise(range_finder_front_pub);
  nh.advertise(range_finder_rear_pub);
}

void loop()  // Arduino Hauptschleife
{
  int distance = getDistance(0);
  // int mean = getDistanceMA();

  range_msg.header.stamp = nh.now();
  range_msg.header.frame_id = "front_sonar";
  range_msg.radiation_type = range_msg.ULTRASOUND;
  range_msg.min_range = range_min;
  range_msg.max_range = range_max;
  range_msg.field_of_view = fov;
  range_msg.range = static_cast<float>(distance) / 100.;

  range_finder_front_pub.publish(&range_msg);

  distance = getDistance(1);
  range_msg.header.stamp = nh.now();
  range_msg.header.frame_id = "rear_sonar";
  range_msg.range = static_cast<float>(distance) / 100.;
  range_finder_rear_pub.publish(&range_msg);

  nh.spinOnce();
  delay(10);
}

// Entfernung in cm über gewöhnlichen Ultraschallsensor mit Echo und Trigger messen
int getDistance(int sensor)
{
  long distance = 0;
  long time = 0;

  if (sensor == 0)
    digitalWrite(trigger, LOW);
  else
    digitalWrite(trigger_rear, LOW);
  delayMicroseconds(3);
  noInterrupts();

  if (sensor == 0)
    digitalWrite(trigger, HIGH);  // Trigger Impuls 10 us
  else
    digitalWrite(trigger_rear, HIGH);
  delayMicroseconds(10);
  
  if (sensor == 0)
    digitalWrite(trigger, LOW);
  else
    digitalWrite(trigger_rear, LOW);

  time = sensor == 0 ? pulseIn(echo, HIGH) : pulseIn(echo_rear, HIGH);  // Echo-Zeit messen
  interrupts();

  time = (time / 2);       // Zeit halbieren
  distance = time / 29.1;  // Zeit in Zentimeter umrechnen
  return distance;
}

// Entfernung Gleitender Durchschnitt (gewichtet)
/* int getDistanceMA()
{
  int old = 0;
  int avg, dist;

  delay(10);
  old = getDistance(0);
  delay(10);
  for (int i = 0; i < 10; i++)
  {
    dist = getDistance(0);
    avg = (0.8 * old) + (0.2 * dist);
    old = avg;
    delay(10);
  }
  return avg;
} */
