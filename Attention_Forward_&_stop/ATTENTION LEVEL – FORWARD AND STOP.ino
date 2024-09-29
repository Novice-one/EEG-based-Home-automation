#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int motorRightA = 11; //left Motor-clockwise in1
int motorLeftA = 8;  //right Motor-clockwise in3
int LED = 8;
#define BAUDRATE 57600
// checksum variables
byte payloadChecksum = 0;
byte CalculatedChecksum;
byte checksum = 0;
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;
boolean brainwave = false;

void setup() 
{
pinMode(motorRightA, OUTPUT);
pinMode(motorLeftA, OUTPUT);
lcd.init();
lcd.backlight();
Serial.begin(57600); 
delay(500);  
Serial.print("Communicating... ");
Serial.println();
digitalWrite(motorRightA, LOW);
digitalWrite(motorLeftA, LOW);
 }
byte ReadOneByte() 
{
   int ByteRead;
  while(!Serial.available());
  ByteRead = Serial.read();
  return ByteRead; // read incoming serial data
}
unsigned int delta_wave = 0;
unsigned int theta_wave = 0;
unsigned int low_alpha_wave = 0;
unsigned int high_alpha_wave = 0;
unsigned int low_beta_wave = 0;
unsigned int high_beta_wave = 0;
unsigned int low_gamma_wave = 0;
unsigned int mid_gamma_wave = 0;
void read_waves(int i) 
{
  delta_wave = read_3byte_int(i);
  i+=3;
  theta_wave = read_3byte_int(i);
  i+=3;
  low_alpha_wave = read_3byte_int(i);
  i+=3;
  high_alpha_wave = read_3byte_int(i);
  i+=3;
  low_beta_wave = read_3byte_int(i);
  i+=3;
  high_beta_wave = read_3byte_int(i);
  i+=3;
  low_gamma_wave = read_3byte_int(i);
  i+=3;
  mid_gamma_wave = read_3byte_int(i);
}
int read_3byte_int(int i) 
{
  return ((payloadData[i] << 16) + (payloadData[i+1] << 8) +payloadData[i+2]);
}

void loop() 
{
f(ReadOneByte() == 0xAA) 
   {
      if(ReadOneByte() == 0xAA) 
        {
            payloadLength = ReadOneByte();

	if(payloadLength > 169) 
	return;
	payloadChecksum = 0;
      		for(int i = 0; i < payloadLength; i++)
      		{      
       		//loop until payload length is complete
        		payloadData[i] = ReadOneByte();             
       		 payloadChecksum += payloadData[i];
      		}
      checksum = ReadOneByte();                   
      payloadChecksum = 255 – payloadChecksum; 
      of generated checksum
      if(checksum == payloadChecksum) 
      {
        poorQuality = 200;
        attention = 0;
        meditation = 0;
      }
     brainwave = false;
     for(int i = 0; i < payloadLength; i++) { // Parse the payload
          switch (payloadData[i]) 
          {
          case 02:
            i++;
            poorQuality = payloadData[i];
            bigPacket = true;
            break;
          case 04:
            i++;
            attention = payloadData[i];
            break;
          case 05:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:                         
            i++;
            brainwave = true;
            byte vlen = payloadData[i];
            //mySerial.print(vlen, DEC);
            //mySerial.println();
            read_waves(i+1);
            i += vlen; // i = i + vlen
            break;
          }                                 
        }                                   
       if(bigPacket) {
          if(poorQuality == 0){
          }
          else{  }
         }
 if(brainwave && attention > 0 && attention < 100) 
     {
            Serial.print("Attention: ");
            Serial.print(attention, DEC);
            Serial.println();
            lcd.setCursor(0,0);
            lcd.print("Attention: ");
            lcd.print(attention, DEC);
         }
        if(attention >= 70)
          {
            Serial.print("go");
            Serial.println();
            lcd.setCursor(0,0);
            lcd.print("Attention: ");
            lcd.print(attention, DEC);
            lcd.setCursor(0,1);
            lcd.print("GO");
            digitalWrite(motorRightA, HIGH);
            digitalWrite(motorLeftA,  HIGH);
            delay(3000);
          }
          if(attention <70)
          {
            Serial.print("stop");
           Serial.println();
           lcd.setCursor(0,0);
            lcd.print("Attention: ");
            lcd.print(attention, DEC);
            lcd.setCursor(0,1);
            lcd.print("STOP");
            digitalWrite(motorRightA, LOW);
            digitalWrite(motorLeftA, LOW);
            }
      }
   }
}
