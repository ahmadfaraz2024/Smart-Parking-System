#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

char auth[] = "6b3d4964ca674955a77b1e44509a3e5d";
char ssid[] = "Faraz";
char pass[] = "farazahmad";

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

SoftwareSerial EspSerial(10, 11); // RX, TX
ESP8266 wifi(&EspSerial);

#define ESP8266_BAUD 9600

unsigned long slotParkedTime1 = 0;
unsigned long slotParkedTime2 = 0;
unsigned long slotParkedTime3 = 0;

float slot1Charge;
float slot2Charge;
float slot3Charge;

#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2

#define RATE 0.05 //rate per secs

void setup(){
  Serial.begin(115200);
  delay(10);
  lcd.begin(16, 2);
  
  lcd.print("Smart Parking!");

  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass);
  lcd.setCursor(0, 1);
  lcd.print("!! ALL OK !!");
}

void toStringTime(String& buff, unsigned long temp){
int sec = (temp % 60);temp /= 60;
int min = (temp % 60);temp /= 60;
int hour = temp;
    //buff += hour; buff += ':';
    if(min < 10)buff += '0';buff += min; buff += ':';
    if(sec < 10)buff += '0';buff += sec;
}

unsigned long previousMillis = 0;

void loop(){
String buff1 = "";
String buff2 = "";
String buff3 = "";

int hour, min, sec;
unsigned long temp;

  unsigned long currentTime = millis();
  
  if (currentTime - previousMillis >= 5000) {
    previousMillis = currentTime;
    currentTime /= 1000;
    buff1 = "L1:";
    if(!digitalRead(SENSOR1)){
      if(!slotParkedTime1){
        slotParkedTime1 = currentTime;
      }else{
        toStringTime(buff1, currentTime - slotParkedTime1);
      }
    }else{
      if(slotParkedTime1){
        slot1Charge = ((float)(currentTime - slotParkedTime1) * RATE);
        slotParkedTime1 = 0;
      }else{
        //buff1 = "Free";
      }
      buff1 += slot1Charge;
    }

    buff2 = "L2:";
    if(!digitalRead(SENSOR2)){
      if(!slotParkedTime2){
        slotParkedTime2 = currentTime;
      }else{
        toStringTime(buff2, currentTime - slotParkedTime2);
      }
    }else{
      if(slotParkedTime2){
        slot2Charge = ((float)(currentTime - slotParkedTime2) * RATE);
        slotParkedTime2 = 0;
      }else{
        //buff2 = "Free";
      }
      buff2 += slot2Charge;
    }

    buff3 = "L3:";
    if(!digitalRead(SENSOR3)){
      if(!slotParkedTime3){
        slotParkedTime3 = currentTime;
      }else{
        toStringTime(buff3, currentTime - slotParkedTime3);
      }
    }else{
      if(slotParkedTime3){
        slot3Charge = ((float)(currentTime - slotParkedTime3) * RATE);
        slotParkedTime3 = 0;
      }else{
        //buff3 = "Free";
      }
      buff3 += slot3Charge;
    }

    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print(buff1.c_str());

    lcd.setCursor(8, 0);
    lcd.print(buff2.c_str());

    lcd.setCursor(0, 1);
    lcd.print(buff3.c_str());

    Serial.println(buff1);
    Serial.println(buff2);
    Serial.println(buff3);

    Blynk.virtualWrite(V0, buff1.c_str());
    Blynk.virtualWrite(V1, buff2.c_str());
    Blynk.virtualWrite(V2, buff3.c_str());
  }

  Blynk.run();
}
