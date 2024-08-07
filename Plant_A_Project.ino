#include <DHT.h>
#include <Preferences.h>
#define DHTTYPE DHT11

Preferences preferences;

// ----- POSITION 1 -----
#define UV_PIN_POSITION1 4
#define LIGHT_PIN_POSITION1 0
#define SOIL_PIN_POSITION1 2
#define DHT_PIN_POSITION1 16
DHT dht1(DHT_PIN_POSITION1, DHTTYPE);

// ----- POSITION 2 -----
#define UV_PIN_POSITION2 35
#define LIGHT_PIN_POSITION2 32
#define SOIL_PIN_POSITION2 34
#define DHT_PIN_POSITION2 33
DHT dht2(DHT_PIN_POSITION2, DHTTYPE);

// ----- POSITION 3 -----
#define UV_PIN_POSITION3 27
#define LIGHT_PIN_POSITION3 14
#define SOIL_PIN_POSITION3 12
#define DHT_PIN_POSITION3 26
DHT dht3(DHT_PIN_POSITION3, DHTTYPE);

// ----- Global variables -----
#define ledForWiFi 15
int Device_id = 2;
unsigned long TimeForProcess;
unsigned long versionCheckTime;

struct Position{
  float temperature;
  float humidity;
  int lightIntensity;
  int UV_radiation;
  int soilMoisture;
}position1, position2, position3;

void collectionOfData(Position &stance, DHT dht, int pinLight, int pinUV, int pinSoil){
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  while(isnan(temperature) || isnan(humidity)){
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
  }

  stance.temperature = temperature;
  stance.humidity = humidity;
  stance.lightIntensity = analogRead(pinLight);
  stance.UV_radiation = analogRead(pinUV);
  stance.soilMoisture = analogRead(pinSoil);
}

/*void goSleep(byte minutes){
  esp_sleep_enable_timer_wakeup(minutes * (60 * 1000000));
  esp_light_sleep_start();
}*/

void setup() {
  Serial.begin(115200);
  pinMode(ledForWiFi, OUTPUT);
  dht1.begin();
  dht2.begin();
  dht3.begin();
  disconnectWiFi();
  preferences.begin("firmware", false);
}

void loop() {
  if((millis() - versionCheckTime) > ((60 * 1000)*60)*24){
    connectToWiFi();
    if(checkingNewVersion()){
      FirmwareUpdate();
    }
    disconnectWiFi();
    versionCheckTime = millis();
  }
  
  if((millis() - TimeForProcess) > (59 * 1000)){
    //Serial.println("Start Process");
    collectionOfData(position1, dht1, LIGHT_PIN_POSITION1, UV_PIN_POSITION1, SOIL_PIN_POSITION1);
    delay(300);
    collectionOfData(position2, dht2, LIGHT_PIN_POSITION2, UV_PIN_POSITION2, SOIL_PIN_POSITION2);
    delay(300);
    collectionOfData(position3, dht3, LIGHT_PIN_POSITION3, UV_PIN_POSITION3, SOIL_PIN_POSITION3);
    delay(500);

    connectToWiFi();
    SendData();
    disconnectWiFi();
    TimeForProcess = millis();
   // Serial.println("End Process");
  }
}
