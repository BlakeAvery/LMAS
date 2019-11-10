#include "MPU9250.h"
#include "Arduino.h"
#include <Wire.h>
#include <SoftwareSerial.h>

#define WireSend(args) Wire.write(args)
#define WireRead(args) Wire.read(args) //I hope whoever wrote this example code perishes.

/*
 * CS19WholeFuckinGamut - ©UFO Productions 2019 for Codestellation 2019.
 * This code is meant to run on the Arduino uno, despite the filename I really can't be fucked to redo this right now
 * Some of this code is trash cause it's cursed don't ask any fuckin questions
 * This code is licensed under the ISC license.
 */

SoftwareSerial jeff(2, 3);
MPU9250 IMU(Wire,0x68);

void setup() {
  Wire.begin();
  pinMode(12, INPUT);
  digitalWrite(12, LOW); //This is our reset solution, if we pull this low we reboot.
  Serial.begin(9600); //This serial port talks to debug system slash whatever else
  jeff.begin(9600); //This one sends nice pretty csvs over software serial to the ESP8266.
  Serial.println("LMAS v1 ©UFO Productions 2019");
  int status = IMU.begin();
  if (status < 0) {
    Serial.println("We can't detect the accelerometer, bailing!");
    Serial.print("Error code: ");
    Serial.println(status);
    delay(1000);
    pinMode(12, OUTPUT);
    Serial.println("F"); //We shouldn't reach here unless something's wrong with the wire between 12 and RESET.
  }
  Serial.println("IMU init!");
}

int GPSAddress = 0x42;//GPS I2C Address

double Datatransfer(char *data_buf,char num)//Data type converter：convert char type to float
{                                           //*data_buf:char data array ;num:float length
  double temp=0.0;
  unsigned char i,j;

  if(data_buf[0]=='-')//The condition of the negative
  {
    i=1;
    //The date in the array is converted to an integer and accumulative
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    //The date will converted integer transform into a floating point number
    for(j=0;j<num;j++)
      temp=temp/10;
    //Converted to a negative number
    temp=0-temp;
  }
  else//Positive case
  {
    i=0;
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    for(j=0;j<num;j++)
      temp=temp/10 ;
  }
  Serial.println(temp);
  return temp;
}
void rec_init()//initial GPS
{
  Wire.beginTransmission(GPSAddress);
  WireSend(0xff);//To send data address
  Wire.endTransmission();

  Wire.beginTransmission(GPSAddress);
  Wire.requestFrom(GPSAddress,10);//Require 10 bytes read from the GPS device
}
char ID()//Receive the statement ID
{
  char i = 0;
  char value[7]={
    '$','G','P','G','G','A',','      };//To receive the ID content of GPS statements
  char buff[7]={
    '0','0','0','0','0','0','0'      };

  while(1)
  {
    rec_init();//Receive data initialization
    while(Wire.available())
    {
      buff[i] = WireRead();//Receive serial data
      if(buff[i]==value[i])//Contrast the correct ID
      {
        i++;
        if(i==7)
        {
          Wire.endTransmission();//End of receiving
          return 1;//Receiving returns 1

        }
      }
      else
        i=0;
    }
    Wire.endTransmission();//End receiving
  }
}
void UTC()//Time information
{
  char i = 0,flag=0;
  char value[7]={
    '$','G','P','G','G','A',','   };
  char buff[7]={
    '0','0','0','0','0','0','0'       };
  char time[9]={
    '0','0','0','0','0','0','0','0','0'    };//Storage time data

  double t=0.0;

  while(1)
  {
    rec_init();
    while(Wire.available())
    {
      if(!flag)
      {
        buff[i] = WireRead();
        if(buff[i]==value[i])
        {
          i++;
          if(i==7)
          {
            i=0;
            flag=1;
          }
        }
        else
          i=0;
      }
      else
      {
        time[i] = WireRead();
        i++;
        if(i==9)
        {
          t=Datatransfer(time,2);//Converts floating-point data
          //t=t+80000.00;//To convert time to Beijing time
          Serial.println(t);//The time data output
          Wire.endTransmission();
          return;
        }
      }
    }
    Wire.endTransmission();
  }
}
void rec_data(char *buff,char num1,char num2)//Receive data function
{                        //*buff：Receive data array；num1：Number of commas ；num2：The   length of the array
  char i=0,count=0;

  if(ID())
  {
    while(1)
    {
      rec_init();
      while(Wire.available())
      {
        buff[i] = WireRead();
        if(count!=num1)
        {
          if(buff[i]==',')
            count++;
        }
        else
        {
          i++;
          if(i==num2)
          {
            Wire.endTransmission();
            return;
          }
        }
      }
      Wire.endTransmission();
    }
  }
}
double latitude()//Latitude information
{
  Serial.println("Getting latitude");
  char lat[10]={
    '0','0','0','0','0','0','0','0','0','0' };//Store the latitude data
  rec_data(lat,1 ,10);//Receive the latitude data
  Serial.println("Latitude get");
  double ret = Datatransfer(lat,5);
  Serial.println(ret);
  if(ret > 90 || ret < 0 || ret == INFINITY) { //Hopefully this covers our asses in the event that we can't get a fix
    ret = 0.0;
  }
  return ret;
  //Serial.println(Datatransfer(lat,5),5);//output
}
String lat_dir()//Latitude direction information
{
  char dir[1]={'0'};//Store latitude direction data
  rec_data(dir,2,1);//Receive latitude direction data
  String ret = String(dir[0]);
  if(!ret.equals("N") || !ret.equals("S")) {
    ret = "N";
  }
  return ret;
  //printlnByte(dir[0]);//output latitude direction information
}
double longitude()//Longitude information
{
  char lon[11]={
    '0','0','0','0','0','0','0','0','0','0','0' };//Store longitude data
  rec_data(lon,3,11);//Receive the longitude data
  double ret = Datatransfer(lon, 5);
  if(ret < 0 || ret > 180) {
    ret = 0.0;
  }
  return ret;
  //Serial.println(Datatransfer(lon,5),5);//out put date
}
String lon_dir()//Longitude direction information
{
  char dir[1]={'0'};
  rec_data(dir,4,1);
  String ret = String(dir[0]);
  if(!ret.equals("W") || !ret.equals("E")) {
    ret = "E";
  }
  return ret;
  //printlnByte(dir[0]);//output latitude direction information
}

void loop() {
  byte alertcode = 0;
  byte scream = 0;
  double ax = IMU.getAccelX_mss();
  double ay = IMU.getAccelY_mss();
  double az = IMU.getAccelZ_mss();
  IMU.readSensor();
  double avg = ax + ay + az; //technically not an average but get off my back mate
  Serial.println(avg);
  int crit = 100;
  double temp = ((IMU.getTemperature_C()) * 9 / 5) + 32;
  if(scream > 0) {
    alertcode |= 1UL << 0;
    Serial.println("bit one set, " + String(alertcode));
  }
  if(avg >= 11 || avg <= -11) {
    alertcode |= 1UL << 1;
    Serial.println("bit two set, " + String(alertcode));
  }
  if(temp > crit) {
    alertcode |= 1UL << 2;
    Serial.println("bit three set, " + String(alertcode));
  }
  //Serial.println("We have reached shooter stringbuilder");
  String shooter = "CF19,42.365970,N,71.257889,W," + String(avg) + "," + String(temp) + "," + String(alertcode); //Empty string we shit out on softwareserial to ESP8266 TODO MAKE THIS WORK
  Serial.println(shooter);
  /*shooter += latitude() + lat_dir() + ",";
  Serial.println(shooter);
  shooter += longitude() + lon_dir() + ",";
  Serial.println(shooter);
  shooter += String(avg) + ",";
  Serial.println(shooter);
  shooter += String(temp) + ",";
  Serial.println(shooter);
  shooter += String(alertcode);
  Serial.println(shooter);*/
  if(alertcode > 0) {
    jeff.println(shooter); //we only wanna talk to the internet if we actually have a problem.
  }
  delay(2000); //we wanna delay 2secs here I don't wanna fry the node
}
