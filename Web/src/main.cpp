#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <DHT.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define SSID "thong"
#define PASSWORD "hoangyenhi"
#define API_KEY "AIzaSyDQByhCdBp5WKBf7AcRweg5sXnrmOnUYfo"
#define USER_EMAIL "nguyenthongtdna@gmail.com"
#define USER_PASSWORD "182003"
#define DATABASE_URL "https://thong-77659-default-rtdb.europe-west1.firebasedatabase.app/"

// Define firebase project 
FirebaseData fbdo ; 
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID 

String uid;

//define DHT pin 
#define DHTPIN 5 
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

// Variable to save database paths 

String databasePath ;
String tempPath;
String humPath;

// variable DHT sensor

float t = 0.0;
float h = 0.0;

// timer 
unsigned long sendDataPrevMillis = 0 ; 
unsigned long timerDelay = 180000 ; 

void initWiFi(){
  WiFi.begin(SSID,PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    /* code */
    Serial.print('.');
    delay(500);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
  
}

void initDHT()
{
  dht.begin();
}

//Write float values to the database 

void sendFloat(String path, float value){
  if(Firebase.RTDB.setFloat(&fbdo,path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print("on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: "+ fbdo.errorReason());
  }
}

void setup(){

  Serial.begin(115200);
  initWiFi();
  initDHT();

  config.api_key = API_KEY;

  // Assign the user sign credentials 
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTBD URL (required)

  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  config.token_status_callback = tokenStatusCallback ;
  config.max_token_generation_retry = 5 ;

  Firebase.begin(&config, &auth);

  Serial.println("Getting User UID");
  while ((auth.token.uid) == "")
  {
    Serial.print('.');
    delay(1000);
  }

  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database path for sensor readings
  tempPath = databasePath + "/temperature"; // --> UsersData/<user_uid>/temperature
  humPath = databasePath + "/humidity" ;//--> UsersData/<user_uid>/humidity
}

void loop(){
  if(Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    t = dht.readTemperature();
    h = dht.readHumidity();

    sendFloat(tempPath,t);
    sendFloat(humPath,h);
    // Get lastest sensor readings
  }
}