#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
            
#include <ESP8266HTTPClient.h>

const char* ssid = "gal";
const char* pswd = "12345678";

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
      String dataURL = "";
      dataURL += "Device_id="+String(Device_id);
      dataURL += "&status=Running";
      dataURL += "&temperature="+String(temperature);
      dataURL += "&lightIntensity="+String(lightIntensity);
      dataURL += "&humidity="+String(humidity);
      dataURL += "&UV_radiation="+String(UV_radiation);
      dataURL += "&soilMoisture="+String(soilMoisture);
      http.begin(client,"http://192.168.135.75:3000/?" + dataURL);
      int httpCode = http.GET();
      Serial.println(httpCode);
      http.end();
}
