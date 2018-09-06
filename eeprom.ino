#include "SIM900.h"
#include "SoftwareSerial.h"
SoftwareSerial SIM900A(D5,D6); //RX || TX
#include "sms.h"
SMSGSM sms;
#include <EEPROM.h>

boolean started=false;
boolean kirim=false;
char smsbuffer[160];
char n[20];

int addr = 1;
int led=D1;
int PIR=D7;
//byte value;
int PIRSTATE=0;
int LASTPIR=0;

void setup() {
  // put your setup code here, to run once:
  //conec to gsm
  SIM900A.begin(115200);   // Setting the baud rate of GSM Module  
  Serial.begin(115200);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  Serial.println(addr);
  pinMode(led,OUTPUT);
  pinMode(addr,INPUT);
  pinMode(PIR,INPUT);
  delay(9000);
  
//  if (gsm.begin(115200)){
//      started=true; 
//     digitalWrite(led,HIGH);
//    delay(100); 
//    digitalWrite(led,LOW);
//    }

  if(gsm.begin(115200))
  {
    started=true;
  }
  if(started)
  {
    hapus();
  }
  
  boolean value = digitalRead(addr);
  Serial.println(value);
  if(value==1)
  {
    digitalWrite(led,HIGH);
//    digitalWrite(buzzer,HIGH);
    delay(1000);
//    digitalWrite(buzzer,LOW);
//    sms.SendSMS("085861964627","Alarm dalam kondisi on");
    SendMessage();
    digitalWrite(led,LOW);
    
  }
  else
  {
    digitalWrite(led,HIGH);
//    digitalWrite(buzzer,HIGH);
//    sms.SendSMS("085861964627","Alarm dalam kondisi off");
    SendMessageOff();
    digitalWrite(led,LOW);
//    digitalWrite(buzzer,LOW);
  }
}

void hapus()
{
  for(int i=0;i<10;i++)
  {
    digitalWrite(led,HIGH);
    int pos=sms.IsSMSPresent(SMS_ALL);
    if(pos!=0)
    {
      if(sms.DeleteSMS(pos)==1)
      {
      }
    }
  }
  digitalWrite(led,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int pos=0;
  if(started)
  {
    pos=sms.IsSMSPresent(SMS_ALL);
    if(pos)
    {
      sms.GetSMS(pos,"+6285314808820",smsbuffer,100);
      //maksud n adalah no hp si yang sms
      delay(500);
      Serial.println(smsbuffer);
      if(!strcmp(smsbuffer,"alarmon"))
      //strcmp artinya membandingkan string yang ada
      {
        SendMessage();
//        sms.SendSMS(n,"Alarm dalam kondisi on");
        digitalWrite(addr,1);
      }
      if(!strcmp(smsbuffer,"alarmoff"))
      {
        
//        sms.SendSMS(n,"Alarm dalam kondisi off");
        SendMessageOff();
        digitalWrite(addr,0);
      }
      hapus();
    }
    
  }
 
  boolean value = digitalRead(addr);
  if(value==1)
  {
    boolean a=digitalRead(PIR);
  
    if(a){
      
      digitalWrite(led,HIGH);
      delay(100);
      digitalWrite(led,LOW);
      delay(100);
      digitalWrite(led,HIGH);
      delay(100); 
      digitalWrite(led,LOW);
      delay(100); 
      digitalWrite(led,HIGH);
      delay(100); 
      digitalWrite(led,LOW);
      delay(100); 
  //    digitalWrite(isd,HIGH);
  //    if(started){
          if(!kirim){
          SendMessagePir();
          kirim=true;
          }
  //    }
    }
    else{
      digitalWrite(led,LOW);
      kirim=false;
      }
    
//    PIRSTATE=digitalRead(PIR);
//    if(PIRSTATE!=LASTPIR)
//    {
//    if(PIRSTATE==HIGH)
//    //kalau sensor PIR out nya HIGH
//    {
////      digitalWrite(buzzer,HIGH);
////      sms.SendSMS("08970869443","Ada Pergerakan");
//      SendMessagePir();
//      delay(5000);
//      
//    }
//    else
//    {
//      digitalWrite(led,LOW);
//    }
//    }
//    LASTPIR=PIRSTATE;
  }
  
}

void SendMessage()
{
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900A.println("AT+CMGS=\"081221686350\"\r"); // Replace with your mobile number
  delay(1000);
  SIM900A.println("Alarm ON");// The SMS text you want to send
  delay(100);
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println (" ->SMS Selesai dikirim");
}

void SendMessageOff()
{
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900A.println("AT+CMGS=\"081221686350\"\r"); // Replace with your mobile number
  delay(1000);
  SIM900A.println("Alarm OFF");// The SMS text you want to send
  delay(100);
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println (" ->SMS Selesai dikirim");
}

void SendMessagePir()
{
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900A.println("AT+CMGS=\"081221686350\"\r"); // Replace with your mobile number
  delay(1000);
  SIM900A.println("Ada Pergerakan");// The SMS text you want to send
  delay(100);
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println (" ->SMS Selesai dikirim");
}
