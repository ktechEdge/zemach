#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Update.h>

const char* ssid = "4G-UFI-A89";
const char* pswd = "1234567890";

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

void ledBlinking(){
  digitalWrite(ledForWiFi, HIGH);
  delay(200);
  digitalWrite(ledForWiFi, LOW);
  delay(200);
}

void connectToWiFi() {
    //Serial.println("connectToWiFi");
    WiFi.begin(ssid, pswd);
        
    while (WiFi.status() != WL_CONNECTED) 
    {
      // Serial.print(".");
       ledBlinking();
    }
    digitalWrite(ledForWiFi, HIGH);
    //Serial.println("Connected to network");
}

void disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  digitalWrite(ledForWiFi, LOW);
  //Serial.println("WiFi disconnected");
}

void SendData() {
      char jsonOutPut[500];
      arr = doc.to<JsonArray>();
      HTTPClient http;
      
      http.begin(client,"http://zemach.ktec-edge.com/esp");
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

bool checkingNewVersion(){
  String currentVersion = preferences.getString("version", "unknown");
  String newVersion;
  HTTPClient http;
  http.begin("http://zemach.ktec-edge.com/esp/version");
  int httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode == HTTP_CODE_OK) {
    newVersion = http.getString();
   }
   http.end();
   if(currentVersion != newVersion){
    preferences.putString("version", newVersion);
    return true;
   }
   
   return false;
}

void FirmwareUpdate(){
    HTTPClient http;
    http.begin("http://zemach.ktec-edge.com/esp/firmware");
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        WiFiClient* client = http.getStreamPtr();

        if (contentLength > 0) {
            bool canBegin = Update.begin(contentLength);
            if (canBegin) {
                Serial.println("Begin OTA update...");
                Update.writeStream(*client);
                if (Update.end()) {
                    if (Update.isFinished()) {
                        Serial.println("OTA update has successfully finished");
                        ESP.restart();
                    } 
                }
            } else {
                Serial.println("Not enough space to begin OTA");
                client->flush();
            }
        }
    }
    http.end();
}
