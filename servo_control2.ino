#include <Servo.h>

//initialize global variables
Servo pan;
Servo tilt;
Servo roll;
int rollPin=9;
int panPin = 10;
int tiltPin = 11;
int inByte;
int sep_flag = 0;
int init_flag=0;
int controlmode=0;
int tempx = 0;
int tempy = 0;
int newx=-1;
int newy=-1;
int xpos=90;
int ypos=90;
int done_flag=0;

void setup()
{
  pan.attach(panPin);
  tilt.attach(tiltPin);
  roll.attach(rollPin);
  roll.write(82);
  pan.writeMicroseconds(1560);
  tilt.write(ypos);
  Serial.begin(57600);
}

//main program loop
void loop()
{
  int i = 0;


  //read from serial input
  if( Serial.available() > 0 )
  {
    //parse input based on communication protocol of "s'xpos'b'ypos'e"
    if(done_flag==1) 
    {
      tempx=0;
      tempy=0;
      done_flag=0;
    }
    inByte = Serial.read();
    if(inByte=='s') {
      init_flag=0;
      done_flag=0;
      sep_flag=0;
      tempx=0;
      tempy=0;
    }
    else if(init_flag==1) {
      if(inByte!='e') 
      {
        controlmode=inByte-'0';
      }
      else {
        init_flag=0;
      }
    }
    else if(inByte=='i') {
      newx=-1;
      newy=-1;
      init_flag=1;
    }
    else if( inByte != 'b' && inByte != 'e' && inByte!='s')
    {
      if( sep_flag==0) 
      {
        tempx=10*tempx;
        tempx+=inByte-'0';
        
      }
      else
      {
        tempy=10*tempy;
        tempy+=inByte-'0';
        
      }
    }
    else
    {
      if(sep_flag==0) 
      {
        sep_flag=1;
      }
      else
      {
        sep_flag=0;
      }
      if(inByte=='e')
      {
        done_flag=1;
        if(tempx>180)
          tempx=180;
        if(tempx<0)
          tempx=0;
        if(tempy>180)
          tempy=180;
        if(tempy<0)
          tempy=0;
        newx=tempx;
        newy=tempy;
        Serial.println(tempx);
        Serial.println(tempy);
        //servocontrol(newx,newy);
      }
    } 
  }
  if(newx!=-1 && newy!=-1) 
  {
    if(controlmode==1)
      modeone(newx,newy);
    else if(controlmode==2)
      modetwo(newx,newy);
  }
}

//for direct control
void modeone(long x, long y) {

  int longest;
  int del=0;
  int xdif=(x-xpos);
  int ydif=(y-ypos);
  if(ydif<0)
    ydif=(ydif*-1);
    longest=ydif;
  if(longest<13)
    del=230/(longest);
  else
    del=210/(longest*.8);
    
    //control x servo
    if(xdif>0)
      xdif=(xdif/10)+1;
    else
      xdif=(xdif/10)-1;
    xdif=xdif*10;

    //If x is not inbetween 75 and 105 then add it to 1560 (center point of servo) otherwise set it to the center point
    if(x<75 || x>105)
      pan.writeMicroseconds(1560+xdif);
    else
      pan.writeMicroseconds(1560);
 

    while(ypos!=y) 
    {
      
      
      if(ypos!=y) {
        if(ypos<y)
          ypos++;
        if(ypos>y)
          ypos--;
      }
      //pan.write(xpos);
      
      tilt.write(ypos);
      
      delay(del);
    }
  

   
}

//for joystick like control
void modetwo(long x,long y) 
{
 //continuouscontrol(x);
    if(y<70) 
      ypos--;
    else if(y>110)
      ypos++;
    if(ypos>180)
      ypos=180;
    if(ypos<0)
      ypos=0;
   // if(ypos<=180 && ypos>=0)
    tilt.write(ypos);
    if(ypos<180 && ypos>0)
      delay(32);
  
}
void continuouscontrol(long x) 
{
  
  if(xpos!=x || controlmode==2) 
  {
    
    if(xpos<x)
    {
      xpos++;
      pan.writeMicroseconds(2000);
    }
    else if(xpos>x)
    {
      xpos--;
      pan.writeMicroseconds(1000);
    }
    //number of milliseconds for a 1 degree rotation of servo
    int del=4;
    delay(del);
    //stop
    pan.writeMicroseconds(1560);
    
  }
}
