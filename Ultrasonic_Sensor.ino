#include <dummy.h>
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>

#define WIFI_AP "Akirachix"
#define WIFI_PASSWORD "Akirachix2021"

#define TOKEN "ESP8266_DEMO_TOKEN"
const int trigPin = 6;
const int echoPin = 5;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

char thingsboardServer[] = "XeRSRXLKmLzBct5aFdqp";

WiFiClient wifiClient;

ThingsBoard tb(wifiClient);

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  InitWifi();
  lastSend = 0;
}

void loop() {  
    if ( !tb.connected() ) {
      reconnect();
  }

  if ( distance() - lastSend > 100 ) { // Update and send only after 1 seconds
    getAndSendDistance();
    lastSend = distance();
  }

  tb.loop();
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  
  delay(1000);
}

void getAndSendDistance(){
  Serial.println("Collecting distance data: ");

  float distanceCm = readDistanceCm();

  float distanceInch = readDistanceInch();

  // Check if any reads failed and exit early (to try again).
  if (isnan(distanceCm) || isnan(distanceInch)) {
    Serial.println("Failed to read from Ultrasonic sensor!");
    return;
  }

  void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
    }
