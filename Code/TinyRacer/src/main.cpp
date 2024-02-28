/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL4Z7XRzbdL"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "4J1YscQA4Vjn7QKOr3KS_XXGzDo_atoA"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//#include <Servo.h>
#include <ESP32Servo.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CASELAB";
char pass[] = "CaseLocalNet";

BlynkTimer timer;
int throttle = 0;
int steering = 0;
int motor1 = 18;
int motor2 = 26;
int motor1rev = 23;
int motor2rev = 25;
int adc_max = 4095;
int trim = 0;

Servo steering_servo;

// This function is called every time the Virtual Pin 1 state changes
BLYNK_WRITE(V1)
{
  // Set incoming value from pin V1 to a variable
  steering = param.asInt();
}

BLYNK_WRITE(V4)
{
  // Set incoming value from pin V4 to a variable
 throttle = param.asInt();
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  //Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  steering_servo.setPeriodHertz(50);    // standard 50 hz servo
  steering_servo.attach(13, 500, 2400); // attaches the servo on pin 13 to the servo object

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor1rev, OUTPUT);
  pinMode(motor2rev, OUTPUT);
  pinMode(14, OUTPUT);
  //steering_servo.attach(13);


  digitalWrite(14, HIGH); //power to motor driver

  // Stop the motor
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor1rev, LOW);
  digitalWrite(motor2rev, LOW);
}

void loop()
{
  Blynk.run();
  //Blynk.virtualWrite(V4, throttle);
  steering_servo.write(steering);
  //Adjust throttle if input is greater than 4
  if (throttle > 4){
    trim = round(throttle * 1.2);
    analogWrite(motor1, trim);
    analogWrite(motor2rev, throttle);
    digitalWrite(motor2, LOW);
    digitalWrite(motor1rev, LOW);
  }
  //Reverse throttle if input is less than -4
  else if (throttle < -4){
    digitalWrite(motor1, LOW);
    analogWrite(motor2, abs(throttle));
    analogWrite(motor1rev, abs(throttle));
    digitalWrite(motor2rev, LOW);
  }
  //Zero throttle if input is less than 4 and greater than -4
  else if (throttle < 4 && throttle > -4){
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
    digitalWrite(motor1rev, LOW);
    digitalWrite(motor2rev, LOW);
  }
  
  //Adjust steering if input is greater than 4
  if (steering > 4 || steering < -4){
    steering_servo.write(steering);
  }
  //Zero steering if input is less than 4
  else{
    steering_servo.write(90);
  }
  /*Serial.println("Throttle");
  Serial.println(throttle);
  Serial.println("\n");
  Serial.println("Steering");
  Serial.println(steering);*/
  //delay(1000);
}