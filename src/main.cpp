#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NewPing.h>


const String SSID = "I.Carlsson_1";
const String password = "92ac437921";
const String host = "http://143.42.27.128:1303/SendDepth";

float duration;
float distance;

const int trigPin = D6;
const int echoPin = D5;

long interval = 1000*60*10;
long prevMillis = 0;

WiFiClient wifiClient;
NewPing sonar(trigPin,echoPin,400);

void sendHttpToServer() {//ändra till skicka en request med alla tre sensordata
  if (WiFi.status() == WL_CONNECTED) {  //Check WiFi connection status

    String sendMsg = String(distance);
    sendMsg.concat(" ");
    sendMsg.concat("4");
    sendMsg.concat(" ");
    sendMsg.concat("7");

    HTTPClient http;                                       //Declare object of class HTTPClient
    http.begin(wifiClient,host);
    http.POST(sendMsg);  //Send the request
    http.end();                      //Close connection
  } else {
  }
}

void connectWifi(){
  WiFi.begin(SSID,password);
  
  delay(0);
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.print(".");
  }
  Serial.println("CONNECTED TO WIFI");
}

void setup() {
  Serial.begin(9600);
  Serial.print("Serial init");
  connectWifi();
  
}

void loop() {

  if (millis() - prevMillis >= interval){
    duration = sonar.ping_median(10); //in microseconds
    distance = (duration/10000*343)/2; //in cm
    Serial.println(distance);
    sendHttpToServer();
    prevMillis = millis();

  }
  
}