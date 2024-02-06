#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "iot";
const char* password = "mrfunkfunk";

String openWeatherMapApiKey = "8df7b9a7be070e91bae0e9b4515308a2";
String city = "Aarhus";
String countryCode = "DK";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

String jsonBuffer;

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

  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}
JsonDocument doc;

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      //Serial.println(jsonBuffer);
      //JSONVar myObject = JSON.parse(jsonBuffer);

      DeserializationError error = deserializeJson(doc, jsonBuffer);
      if (error) {
        Serial.println("Parsing input failed!");
        return;
      }


float temperatureKelvin = doc["main"]["temp"]; // Temperature in Kelvin
      float temperatureCelsius = temperatureKelvin - 273.15; // Convert to Celsius
      Serial.print("Temperature (Celsius): ");
      Serial.println(temperatureCelsius);

     // Serial.print("JSON object ");
    //  Serial.println(doc);
      Serial.print("Temperature:");

      //JsonObject main = doc["main"];
//      float temperature = main["temp"]; // 280.33
float temperature = doc["main"]["temp"]; // 280.33


      // Extract temperature as a string and then convert it to float
//      String tempStr = String((float)myObject["main"]["temp"]);
  //    float temperature = tempStr.toFloat();
     // float temperature = (float)(myObject["main"]["temp"]);
      Serial.println(temperature);


     // Serial.print("Pressure: ");
     // Serial.println(myObject["main"]["pressure"]);
     // Serial.print("Humidity: ");
     // Serial.println(myObject["main"]["humidity"]);
     // Serial.print("Wind Speed: ");
     // Serial.println(myObject["wind"]["speed"]);

      // Play song based on temperature
    playSongBasedOnTemperature(temperature);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
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
  // Define temperature thresholds and associated songs
  if (temperature < 273.15) { // Below 0°C
    Serial.println("Playing song A");
    // Code to play song A
  } else if (temperature < 283.15) { // 0°C to 10°C
    Serial.println("Playing song B");
    // Code to play song B
  } else if (temperature < 293.15) { // 10°C to 20°C
    Serial.println("Playing song C");
    // Code to play song C
  } else { // Above 20°C
    Serial.println("Playing song D");
    // Code to play song D
  }
}



  