#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "iot";
const char* password = "mrfunkfunk";

String openWeatherMapApiKey = "8df7b9a7be070e91bae0e9b4515308a2";
String city = "Aarhus";
String countryCode = "DK";

unsigned long timerDelay = 10000;

String jsonBuffer;


//Definering af lamper + magnetsensor
#define MAGNECTIC_SWITCH 33
#define RED 22
#define YLW 21
#define BLU 17



void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  //Pins. Øverste er input til magnetdetektoren, de tre andre er LED
  pinMode(MAGNECTIC_SWITCH, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(BLU, OUTPUT);

  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}
JsonDocument doc;

void loop() {
  int sensorValue = digitalRead(MAGNECTIC_SWITCH);
    if(sensorValue == HIGH)//if the sensor value is HIGH?
    {
        
        digitalWrite(BLU, LOW);
        digitalWrite(RED, LOW);
        digitalWrite(YLW, LOW);
    }
    else
    {
        getTemperature()
    }
}

void getTemperature() {
  if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());

      DeserializationError error = deserializeJson(doc, jsonBuffer);
      if (error) {
        Serial.println("Parsing input failed!");
        return;
      }


float temperatureKelvin = doc["main"]["temp"]; // Temperature in Kelvin
      float temperatureCelsius = temperatureKelvin - 273.15; // Convert to Celsius
      Serial.print("Temperature (Celsius): ");
      Serial.println(temperatureCelsius);
      Serial.print("Temperature:");

float temperature = doc["main"]["temp"]; // 280.33

      Serial.println(temperature);

      // Play song based on temperature
    playSongBasedOnTemperature(temperature);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  
  int httpResponseCode = http.GET();
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

void playSongBasedOnTemperature(float temperature) {
  // Define temperature thresholds and associated responses
  if (temperature < 273.15) { // Below 0°C
    digitalWrite(BLU, HIGH);
  } else if (temperature < 283.15) { // 0°C to 10°C
    digitalWrite(YLW, HIGH);
  } else { // Above 10°C
    digitalWrite(RED, HIGH);
  }
}



  
