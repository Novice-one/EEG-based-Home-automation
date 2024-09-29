void setup()
{
Serial.begin(9600)
}
void loop()
{
AT+UART=57600,0,0
AT+ROLE=1
AT+PSWD="0000"
AT+CMODE=0
AT+BIND=<Address>  (E07D,EA,E5FC59)  
AT+IAC=9E8B33
AT+CLASS=0
AT+INQM=1,9,48
}
