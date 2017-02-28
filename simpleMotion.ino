#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Simplepush.h>

//program constants

// constants for simplepush
char* SIMPLE_KEY   = "XXXXXX";
char* SIMPLE_TITLE = "Simple-Motion";
char* SIMPLE_EVENT = "sm01";

char* SIMPLE_MSGS[] = {
  "SimpleMotion has been started",
  "Motion has been detected"
};

//constants for wifi
const char* WIFI_SSID   = "XXXXXX";
const char* WIFI_PASSWD = "XXXXXX";

// constants for misc
const int     LED_PIN       = D1;
const int     NOTIFICATION_PIN = D2;
const int     POLLING_CYCLE = 5000;
// constants for sensors
const int SENSOR_PIN         = D6;
const int SENSOR_OPEN        = 0;
const int SENSOR_CLOSED      = 1; // Motion detected
const boolean SENSOR_USEPULL = false;
//end program constants


// data types
enum SimpleMessageType {
  MotionStart = 0,
  MotionDetected  
};

// function definitions
void blinkLED(int ledPin, int count,int duration);

// main global variables

Simplepush       simple;
int          lastSensorStatus;    // true  = open, false = close
int          currentSensorStatus; // true = open, false = close

void setup() {
 
  if(SENSOR_USEPULL){
   pinMode(SENSOR_PIN,INPUT_PULLUP);
  }else{
   pinMode(SENSOR_PIN,INPUT);
  }
  pinMode(LED_PIN,OUTPUT);
  pinMode(D2,OUTPUT);
  
  // Connect ESP8266 to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  
  simple.send(SIMPLE_KEY,SIMPLE_TITLE,SIMPLE_MSGS[MotionStart],SIMPLE_EVENT);
  blinkLED(LED_PIN,3,1000);

  lastSensorStatus = digitalRead(SENSOR_PIN);
  digitalWrite(LED_PIN,HIGH);
}

void loop(){
  currentSensorStatus = digitalRead(SENSOR_PIN);
  if(currentSensorStatus != lastSensorStatus) {
    if(currentSensorStatus == SENSOR_CLOSED) {
      simple.send(SIMPLE_KEY,SIMPLE_TITLE,SIMPLE_MSGS[MotionDetected],SIMPLE_EVENT);
      blinkLED(NOTIFICATION_PIN,1,1000);
    }
  }
  lastSensorStatus = currentSensorStatus;
  delay(POLLING_CYCLE);
}

// strobe ledPin high/low count number of times
// with each high/low lasting for duration
void blinkLED(int ledPin, int count,int duration){
 for(int i=0;i<count;i++) {
  digitalWrite(ledPin,HIGH);
  delay(duration/2);
  digitalWrite(ledPin,LOW);
  delay(duration/2);
 }
}

