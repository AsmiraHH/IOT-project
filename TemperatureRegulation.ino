#include <DHT.h>

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Amina"
#define WIFI_PASSWORD "IspitIOT"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCGj-Obwg-UTdPaqYYSebJCYpdd4HTk7y8"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://temperature-regulation-default-rtdb.europe-west1.firebasedatabase.app"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
#include <DHT.h>

DHT dht;
int senzor = D4;
int dioda = D1;
int granicnaVrijednost;
bool jelUpaljeno = false;
int ponavljanje = 0;
int parametarTemperatura=0;
int komanda=0;
int upaliParametar;
int parametarBlinkaj;

void setup() {
  // put your setup code here, to run once:
  pinMode(senzor, INPUT);
  dht.setup(senzor);
  Serial.begin(115200);
  pinMode(dioda, OUTPUT);
  // digitalWrite(dioda,LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 3000 || sendDataPrevMillis == 0)) {
    // put your main code here, to run repeatedly:
    float temperatura = dht.getTemperature();

    delay(3000);
    if (Firebase.RTDB.setFloat(&fbdo, "parametri/temperatura", temperatura)) {
      String porukaTemp = "Poslata vrijednost temperature " + String(temperatura);
      Serial.println(porukaTemp);
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (temperatura > 28) {
      digitalWrite(dioda, HIGH);
      delay(3000);
      digitalWrite(dioda, LOW);
    }

    Firebase.RTDB.getInt(&fbdo, "parametri/parametarTemperatura");
      parametarTemperatura=fbdo.intData();
    

    if(parametarTemperatura > 30){
      Firebase.RTDB.setInt(&fbdo, "parametri/komanda", 1);
    }
    else{
      Firebase.RTDB.setInt(&fbdo, "parametri/komanda", 0);
    }

    if( Firebase.RTDB.getInt(&fbdo, "parametri/komanda")){
      komanda=fbdo.intData();
      if(komanda==1){
           digitalWrite(dioda, HIGH);
           delay(4000);
           digitalWrite(dioda, LOW);
           delay(1000);
      }
     
      else
       digitalWrite(dioda, LOW);
    }
    }
}
