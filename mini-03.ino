#include <WiFi.h>
#include <ESP32Servo.h>

#include "FirebaseESP32.h"

#define FIREBASE_HOST "https://iniiotboss-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "rmdN0oS6zC2S47XFr2SDb0S4kd5QU9rts8amrDt2"
#define WIFI_SSID "IniWifi"
#define WIFI_PASSWORD "iniWifi123"

FirebaseData firebaseData;

int ldr = 34, batas, val, valAkhir, i, valueBatas;
// char stringLux[10], valBatas[10];
Servo servo23;
bool terbuka = false;

void initWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
                                                                 
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void firebaseNih() {
  Firebase.setInt(firebaseData, "/iniiotboss-default-rtdb/node1/ldr", valAkhir);
  delay(1000);
  Firebase.getInt(firebaseData, "/iniiotboss-default-rtdb/node1/batas");
  batas = firebaseData.intData();

  if(!terbuka){
      if(valAkhir >= batas){
        Serial.println("Terbuka");
        for(i=0; i<=180; i++){
          servo23.write(i);
          if(i%3==0)Serial.print(".");
            delay(25);
        }
        terbuka = true;
      }
    }else{
      if(valAkhir <= batas){          
        Serial.println("Tertutup");
        for(i=180; i>0; i--){
          servo23.write(i);
          if(i%3==0)Serial.print(".");
            delay(25);
        }
        terbuka = false;
      }
    }
}

void lux(){
  val = analogRead(ldr);
  valAkhir = 0.009768*val+10;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initWifi();
  
  servo23.attach(23);
}

void loop() {
  lux();
  Serial.println(valAkhir);
  Serial.println(batas);
  firebaseNih();
}
