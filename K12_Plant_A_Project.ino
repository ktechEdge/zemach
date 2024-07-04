#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Gal";
const char* pswd = "0544933268";
char jsonOutPut[128];

struct Testing{
  int id;
  String Status;
}test1;

WiFiClient client;

void wifi_Setup() {
    Serial.println("wifiSetup");
    WiFi.begin(ssid, pswd);
        
    while (WiFi.status() != WL_CONNECTED) 
    {
       Serial.print(".");
       delay(200);
    }
    Serial.println("Connected to network");
}

void SendData() {
      HTTPClient http;
     /* String dataURL = "";
      dataURL += "Device_id="+String(Device_id);
      dataURL += "&status=Running";
      dataURL += "&temperature="+String(temperature);
      dataURL += "&lightIntensity="+String(lightIntensity);
      dataURL += "&humidity="+String(humidity);
      dataURL += "&UV_radiation="+String(UV_radiation);
      dataURL += "&soilMoisture="+String(soilMoisture);*/

      test1.id = Device_id;
      test1.Status = "Running";
      
      http.begin(client,"http://192.168.1.83:3000/esp");
      http.addHeader("Content-Type","application/json");

      //const size_t CAPACITY = JSON_OBJECT_SIZE(3);
      StaticJsonDocument<256> doc;
      JsonArray arr = doc.to<JsonArray>();
      JsonObject object = arr.createNestedObject();
      object["id"] = 1;
      object["Status"] = test1.Status;
      JsonObject object2 = arr.createNestedObject();
      object2["id"] = 2;
      object2["Status"] = test1.Status;
      JsonObject object3 = arr.createNestedObject();
      object3["id"] = 3;
      object3["Status"] = test1.Status;

      serializeJson(doc, jsonOutPut);
      
      int httpCode = http.POST(String(jsonOutPut));
      Serial.println(httpCode);
      http.end();
}
