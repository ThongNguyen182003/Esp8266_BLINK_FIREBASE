#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "VIETTEL_NguyenHuuThong"
#define WIFI_PASSWORD "01082003"

#define API_KEY "AIzaSyDW8WZjl27_RNBq7qr4KZkxFcGkEKwVh1U"
#define DTBU "https://esp8266demo-34e4a-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int led = 2;

bool signUp = false;

void setup()
{
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println();
    Serial.println(WiFi.localIP());
    Serial.println();

    config.api_key = API_KEY;
    config.database_url = DTBU;

    if (Firebase.signUp(&config, &auth, "", ""))
    {
        Serial.println("Sign up successfull.");
        signUp = true;
    }
    else
    {
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}
void loop()
{
    if (Firebase.ready() && signUp == true)
    {
        if (Firebase.RTDB.getInt(&fbdo, "/LedStatus"))
        {

            if (fbdo.dataType() == "int")
            {
                int state = fbdo.intData();
                Serial.println(state);
                if (state == 1)
                {
                    digitalWrite(led, HIGH);
                }
                else
                    digitalWrite(led, LOW);
            }
        }
    }
    else
    {
        Serial.print("Error in getInt, ");
    }
}