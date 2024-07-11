#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Gal";
const char* pswd = "0544933268";

StaticJsonDocument<500> doc;
JsonArray arr;

void AddObjectToJson(float temperature, float humidity, int lightIntensity, int UV_radiation, int soilMoisture){
      JsonObject object = arr.createNestedObject();
      object["temperature"] = temperature;
      object["humidity"] = humidity;
      object["lightIntensity"] = lightIntensity;
      object["UV_radiation"] = UV_radiation;
      object["soilMoisture"] = soilMoisture;
}


WiFiClient client;

void connectToWiFi() {
    Serial.println("connectToWiFi");
    WiFi.begin(ssid, pswd);
        
    while (WiFi.status() != WL_CONNECTED) 
    {
       Serial.print(".");
       delay(200);
    }
    Serial.println("Connected to network");
}

void disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected");
}

void SendData() {
      char jsonOutPut[500];
      arr = doc.to<JsonArray>();
      HTTPClient http;
      
      http.begin(client,"http://192.168.1.83:3000/esp");
      http.addHeader("Content-Type","application/json");
      
      JsonObject object = arr.createNestedObject();
      object["id"] = Device_id;
      object["Status"] = "Running";

      AddObjectToJson(position1.temperature, position1.humidity, position1.lightIntensity, position1.UV_radiation, position1.soilMoisture);
      AddObjectToJson(position2.temperature, position2.humidity, position2.lightIntensity, position2.UV_radiation, position2.soilMoisture);
      AddObjectToJson(position3.temperature, position3.humidity, position3.lightIntensity, position3.UV_radiation, position3.soilMoisture);
      
      serializeJson(doc, jsonOutPut);
      
      int httpCode = http.POST(String(jsonOutPut));
      Serial.println(httpCode);
      http.end();
}
