
#include <Servo.h>

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

void loop()
{
  int i = 0;

  if( Serial.available() > 0 )
  {
    if(done_flag==1) 
    {
      tempx=0;
      tempy=0;
      done_flag=0;
    }
    inByte = Serial.read();
    //Serial.println(inByte);
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
        //Serial.print("control mode:");
        //Serial.println(controlmode);
      }
      else {
        init_flag=0;
      }
    }
    else if(inByte=='i') {
      newx=-1;
      newy=-1;
      init_flag=1;
      /*inByte=Serial.read();
      Serial.println(inByte);
      controlmode=inByte-'0';
      while(Serial.read()!='e') {}
      Serial.print("control mode:");
      Serial.println(controlmode);*/
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
  //servocontrol(newx,newy);
  if(newx!=-1 && newy!=-1) 
  {
    if(controlmode==1)
      modeone(newx,newy);
    else if(controlmode==2)
      modetwo(newx,newy);
  }
//  pan.write(panx);
  //tilt.write(tilty);
}

void modeone(long x, long y) {
  //int distance=sqrt(((x-xpos)*(x-xpos))+((y-ypos)*(y-ypos)));
  int longest;
  int del=0;
  int xdif=(x-xpos);
  int ydif=(y-ypos);
  /*if(xdif<0)
    xdif=(xdif*-1);*/
  if(ydif<0)
    ydif=(ydif*-1);
  /*if(xdif>ydif)
    longest=xdif;
  else*/
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
    if(x<75 || x>105)
      pan.writeMicroseconds(1560+xdif);
    else
      pan.writeMicroseconds(1560);
 
  //while(xpos!=x || ypos != y) {

    while(ypos!=y) 
    {
      
      //continuouscontrol(x);
      //control x
      /*if(x<70 || x>110) {
        
      } 
      if(ypos>180)
        ypos=180;
      if(ypos<0)
        ypos=0;*/
   // if(ypos<=180 && ypos>=0)
      // pan.writeMicroseconds(ypos);
      
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
