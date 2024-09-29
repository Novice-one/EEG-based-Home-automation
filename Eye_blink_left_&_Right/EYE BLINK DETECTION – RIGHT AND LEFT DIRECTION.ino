#define BAUDRATE 57600
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 
int lightstatus = 0;
int fanstatus = 0;
int data;
#define RX_PIN 10
#define TX_PIN 12
SoftwareSerial esp32serial(RX_PIN,TX_PIN);
boolean DEBUGOUTPUT  = false,piekDetected = false;
int Data[512] = {0};
int i = 0, n = 0,e=0,k=0;
int envi=0;
int PiekP, PiekM;
long piekTime = 0;
unsigned long int  start=0;
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 200;
const int rwheel=11;
const int lwheel=8;

void setup()
{
    
  Serial.begin(57600);
  Serial.begin(BAUDRATE);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  lcd.begin(16,2);
  lcd.backlight();
  pinMode(rwheel,OUTPUT);
  pinMode(lwheel,OUTPUT);
  digitalWrite(rwheel,LOW);  
  digitalWrite(lwheel,LOW);
  Serial.print("start");
}
byte ReadOneByte()
{
  int ByteRead;
  while (!Serial.available());
  ByteRead = Serial.read();
  return ByteRead;
}

void loop()
{
  long Hilf = 0;
  if (i >= 512)
    i = 0;
 if (ReadOneByte() == 170 && ReadOneByte() == 170)
  {
    payloadLength = ReadOneByte();
    if (payloadLength == 4)
    {
      if (ReadOneByte() == 128 && ReadOneByte() == 2)
      {
          Hilf =  ((long)ReadOneByte() * 256 + (long)ReadOneByte());
        if (Hilf > 32767)
          Hilf -= (long)(65535);
        Data[i] = (int)Hilf;

        ReadOneByte(); 
        PiekP += Data[(512 + i - 71) % 512];
        PiekP -= Data[(512 + i - 50 - 71) % 512];
        if ((PiekP > 3000) && (Data[(512 + i  - 70) % 512] < 0) && (PiekP < 13000))
        { 
          if (millis() - piekTime >100) 
          { 
             PiekM = 0;
            for (int j = 1; j <= 70; j++)
              PiekM +=  (int)(Data[(512 + i  + j - 70) % 512]);
            if (PiekM < -3000 && PiekM > -11000) 
			{  Serial.println("I-Blink detected!");
              if(e==0)
              {
                start=millis();
                Serial.print("Initail time :\n");
                Serial.print(start);
                delay(500);
               }
              Serial.print(millis()-start);
              delay(100);
              if(millis()-start<4000)
              {
                e++;
                lcd.setCursor(0,0);
                lcd.println(e);
              }
              if(millis()-start>4000)
              {
                Serial.print("\nEye blink count");
                Serial.println(e);
                lcd.setCursor(8,0);
                lcd.print("EYE :");
                lcd.println(e);
                k=e;
                delay(1000); 
                e=0;
                start=0;
              }
              if(k==3)
              {
                lcd.setCursor(0,1);
                lcd.print("RIGHT");
                Serial.print("Right");
                digitalWrite(rwheel,LOW);
                digitalWrite(lwheel,HIGH);
                delay(8000);
              }
              if(k==5)
              {
                lcd.setCursor(0,1);
                lcd.print("left");
                Serial.print("LEFT");
                digitalWrite(rwheel,HIGH);
                digitalWrite(lwheel,LOW);
                delay(8000);
              }
             if(k==4)
              {
                if (lightstatus == 0 && k==4) {
                  data=1;
                  Serial.println("lighton");
                  esp32serial.println(data);
                  lcd.setCursor(0,1);
                  lcd.print("LON");
                  lightstatus = 1;
                  k=0;
                  delay(3000);
                }
                if (lightstatus == 1 && k==4)
                 {
                  data=2;
                  Serial.println("lightoff");
                  esp32serial.println(data);
                  lcd.setCursor(0,1);
                  lcd.print("LOFF");
                  lightstatus = 0;
                  k=0;
                  delay(3000);
                }
              }
              if(k==6)
              {
                if (fanstatus == 0 && k==6) {
                  data=3;
                  Serial.println("fanon");
                  esp32serial.println(data);
                  lcd.setCursor(0,1);
                  lcd.print("FON");
                  fanstatus = 1;
                  k=0;
                  delay(3000);
                }
                if (fanstatus  == 1 && k==6)
                 {
                  data=4;
                  Serial.println("fanoff");
                  esp32serial.println(data);
                  lcd.setCursor(0,1);
                  lcd.print("FOFF");
                  fanstatus  = 0;
                  k=0;
                  delay(3000);
                }
              }
              else{
                lcd.setCursor(0,1);
                lcd.print("NO    OPERATION");
               digitalWrite(rwheel,HIGH);  
               digitalWrite(lwheel,HIGH);
              }
              k=0;
              }
              if ((millis() - piekTime) < 600)n++; else n = 1;
              if(poorQuality == 0)digitalWrite(6,HIGH);else digitalWrite(6,LOW);
              piekTime = millis();
              piekDetected = true;
            }
          }
        } 
        i++;
        if (DEBUGOUTPUT && i == 512)
        {
          for (int j = 0; j < 512; j++)
          {
            Serial.print(Data[j]);
            Serial.print(";");
          }
          Serial.println("");
        }
      }
    }
    else if(payloadLength < 170)
   {
      for (int k = 1; k < payloadLength; k++)
      if(ReadOneByte()== 2)poorQuality = ReadOneByte();
    }
  } 
