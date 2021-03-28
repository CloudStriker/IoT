#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
// diatas itu library ya :)


#define DHTPIN 23   // menerima data dht sensor
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); // inisialisasi

int tempS, luxS, btn;
int tempLed = 22, luxLed = 21;
int ldr = 39, val; //val menampung data ldr

WidgetLED vTempLed(V7); //virtual7 = temperature
WidgetLED vLuxLed(V6);  // virtual6 = Cahaya

//blynk_write untuk menerima data dari app ke mesin

BLYNK_WRITE(V4){ 
  luxS = param.asInt();
}

BLYNK_WRITE(V5){
  tempS = param.asInt(); 
}

BLYNK_WRITE(V8){
  btn = param.asInt();
}

int temp(){
  int t = dht.readTemperature();
  if(isnan(t)){
    vTempLed.on();
    digitalWrite(tempLed, HIGH);
    delay(500);
    vTempLed.off();
    digitalWrite(tempLed, LOW);
    delay(500);
    return 0;
  }
  return t;
}

int lux(){
  val = analogRead(ldr);
  return 0.009768*val+10;
}

void th(){
  if(luxS <= lux()){  // LuxS lihat baris 30, lux() lihat baris 48
    vLuxLed.on();
    digitalWrite(luxLed, HIGH);
  }else{
    vLuxLed.off();
    digitalWrite(luxLed, LOW);
  }

  if(tempS <= temp()){  // tempS lihat 26, temp() lihat baris 34
    vTempLed.on();
    digitalWrite(tempLed, HIGH);
  }else{
    vTempLed.off();
    digitalWrite(tempLed, LOW);
  }
}

char auth[] = "3MVsJhxg6nZwzdkwClGKvidI_GL6pht8";
char ssid[] = "OKE";
char pass[] = "bergizitingg1";

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(72,44,45,151), 8080);
  dht.begin();
  pinMode(tempLed, OUTPUT);
  pinMode(luxLed, OUTPUT);
}

void loop() {
  Blynk.run();
  mainFunc();
}

void mainFunc(){
  if(btn == 1){ 
    Blynk.virtualWrite(V0, lux());
    Blynk.virtualWrite(V1, temp());
    Blynk.virtualWrite(V2, lux());
    Blynk.virtualWrite(V3, temp());

    th();
  }else{
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V3, 0);
    vTempLed.on();
    vLuxLed.on();
    delay(500);
    vTempLed.off();
    vLuxLed.off();
    delay(500);
  }
}
