//WIFI控制宿舍电灯项目
// Data 2018/04/01
// by 205
// update log:
//      04/02    优化电机代码




#include <SoftwareSerial.h>
#include <Servo.h>




SoftwareSerial mySerial(10, 11); // RX, TX


bool ButtonA=false,ButtonB=false;
const int buttonPina=7;
const int buttonPinb=4;
int changeflagA=0;  
int changeflagB=0;
int anglea=0;
int angleb=0;
int lastButtonStatea=LOW;
int lastButtonStateb=LOW;
int buttonStatea;
int buttonStateb;
  

  long lastDebounceTimea=0;
  long lastDebounceTimeb=0;
  long debounceDelaya=50;
  long debounceDelayb=50;

 int angle=65;

Servo myservoa;
Servo myservob;

void setup()
{
  
  pinMode(buttonPina,INPUT);
  pinMode(buttonPinb,INPUT);
  myservoa.write(0);
  myservob.write(0);
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for Leonardo only
  }
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  initWifi();
  

}


void initWifi()
{
  mySerial.println("AT+CIPMUX=1");
  delay(2000);
  mySerial.println("AT+CIPSERVER=1,8080");

}

void info()
{
  char cmd[20];
  int i=0;

  for(i=0;i<20;i++)cmd[i]='\0';

  i=0;
  if(mySerial.available())
  {
    for(i=0;i<19;i++)
    {
      if(mySerial.available())
      {
       cmd[i]=mySerial.read();
       delay(1);

       }
       else 
       {
        break;
       }
    }


    int j;
    int onAlike;
    int offAlike;
    int onBlike;
    int offBlike;

    onAlike=0;
    offAlike=0;
    onBlike=0;
    offBlike=0;


    if(find(cmd,"ona")==true)
    {
      ButtonA=true;
      changeflagA=1;
    }
    else if(find(cmd,"onb")==true)
    {
      ButtonB=true;
      changeflagB=1;
    }
    else if(find(cmd,"offa")==true)
    {
      ButtonA=false;
      changeflagA=1;
    }
    else if(find(cmd,"offb")==true)
    {
      ButtonB=false;
      changeflagB=1;
    }
    else if(find(cmd,"on")==true)
    {
      changeflagA=1;
      changeflagB=1;
      ButtonA=true;
      ButtonB=true;
    }
    else if(find(cmd,"off")==true)
    {
      changeflagA=1;
      changeflagB=1;
      ButtonA=false;
      ButtonB=false;
    }
    else if(find(cmd,'ERROR')==true)
    {
      mySerial.println('AT+RST');
    }
    else 
    {
      changeflagA=0;
      changeflagB=0;
    }
    
    


    Serial.println(cmd);

  
  }
}


void loop() // run over and over
{

  info();


  swich2();

  
  if(changeflagA!=0||changeflagB!=0)swich1();


  

}


bool find(char a[],char key[])
{

    int i,j;
    int flag=-1;
    int like=0;
    int len=0;

    for(i=0;key[i]!='\0';i++)
    {
        len++;
    }
    j=0;
    for(i=0;a[i]!='\0';i++)
    {
        if(a[i]==key[j])
        {
            if(flag==-1||flag==1)
            {
                j++;
                like++;
            }
            flag=1;
            printf("like %d len %d ",like,len);
            if(like==len)
            {
                return true;
            }
        }
        else if(flag==1)flag=0;
    }
    return false;
}


void swich1() {

  if(changeflagA==1)myservoa.attach(3);
  if(changeflagB==1)myservob.attach(5);

  if(anglea==0&&ButtonA==false)
  {
    anglea+=angle;
    myservoa.write(anglea);
  }
  if(anglea==angle&&ButtonA==true)
  {
    
    anglea-=angle;
    myservoa.write(anglea);
  }
  delay(600);
  myservoa.detach();

  if(angleb==0&&ButtonB==true)
  {
    angleb+=angle;
    myservob.write(angleb);
  }
  if(angleb==angle&&ButtonB==false)
  {
    angleb-=angle;
    myservob.write(angleb);
  }
  delay(600);
  myservob.detach();
  changeflagA=0;
  changeflagB=0;
}

void swich2() {

  int readinga=digitalRead(buttonPina);
  int readingb=digitalRead(buttonPinb);

  


  if(readinga!=lastButtonStatea)
  {
    lastDebounceTimea=millis();
  }
  if((millis()-lastDebounceTimea)>debounceDelaya)
  {
    if(readinga!=buttonStatea)
    {
      buttonStatea=readinga;
      if(buttonStatea==HIGH)
      {
        if(ButtonA==true) 
        {
          ButtonA=false;
        }
        else if(ButtonA==false) 
        {
          ButtonA=true;
          
        }
        changeflagA=1;
      }
    }
  }
  lastButtonStatea=readinga;

  
  if(readingb!=lastButtonStateb)
  {
    lastDebounceTimeb=millis();
  }
  if((millis()-lastDebounceTimeb)>debounceDelayb)
  {
    if(readingb!=buttonStateb)
    {
      buttonStateb=readingb;
      if(buttonStateb==HIGH)
      {
        if(ButtonB==true) 
        {
          ButtonB=false;

        }
        else if(ButtonB==false) 
        {
          ButtonB=true;
        }
        changeflagB=1;
      }
    }
  }
  lastButtonStateb=readingb;
}
