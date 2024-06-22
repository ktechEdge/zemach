int Device_id = 1;
int temperature = 0;
int humidity = 0;
int lightIntensity = 0;
int UV_radiation = 0;
int soilMoisture = 0;

unsigned long TimeForSendData;

void setup() {
  Serial.begin(9600);
  wifi_Setup();
  TimeForSendData = millis();
}

void loop() {
  if((TimeForSendData - millis()) > 2000)
  {
    SendData();
    TimeForSendData = millis();  
  }
}
