#include <SoftwareSerial.h>

#define baudRate_bluetooth_mod 9600

#define rxPin_bluetooth_mod A0
#define txPin_bluetooth_mod A1

#define btDataPacketMaxLength 5

#define COORDS_PACKET 0x7d
#define BUTTON_PACKET 0x7f

#define ORIGIN_COORD 0 
#define ORIGIN_SLIDE 62


char btDataPacket[btDataPacketMaxLength];


SoftwareSerial btSerial(rxPin_bluetooth_mod, txPin_bluetooth_mod);
unsigned long last_time=millis();


void setup_bluetooth_mod(){
  btSerial.begin(baudRate_bluetooth_mod);
}

void loop_bluetooth_mod(){

 
  // check if data has been sent from the computer:
  if (getBtDataPacket(btDataPacket)){
    last_time=millis();
    process_bt_data(btDataPacket);

  }

}
void process_bt_data(char btDataPacket[])
{
  if (btDataPacket[0]==COORDS_PACKET && robotMode==BT_ROBOT_MODE){
        int joyX=map(btDataPacket[1],-124,124,-255,255);
        int joyY=map(btDataPacket[2],-124,124,-255,255);
        setSpeedMotorFromCoords(joyX,joyY);
  }else if (btDataPacket[0]==BUTTON_PACKET){
    robotMode= (btDataPacket[1]==1)?AUTO_ROBOT_MODE:BT_ROBOT_MODE;
  }
}
boolean  getChar(char &car){
  unsigned long wait_begin_time=millis();
  while (btSerial.available()<=0
         && ((millis()-wait_begin_time)<100) );
    if (!((millis()-wait_begin_time)<100)) return false;
    car =  btSerial.read();



    return true;
}
void initBTDataPacket(char btDataPacket[]){
  btDataPacket[0]=COORDS_PACKET;
  btDataPacket[1]=ORIGIN_COORD;
  btDataPacket[2]=ORIGIN_COORD;
  btDataPacket[3]=ORIGIN_SLIDE;
  btDataPacket[4]=ORIGIN_SLIDE;
}  

boolean getBtDataPacket(char btDataPacket[]){
  initBTDataPacket(btDataPacket);
  if (millis()-last_time>500) return true;
  if (btSerial.available()==0) return false;
  //init adq
  int i=0;
  char car =0;

  if (!getChar(car)) return false; 
  if (!(car==COORDS_PACKET || car==BUTTON_PACKET)) return false;
  int lengthBtDataPacket= (car==COORDS_PACKET)?5:3;
  char checksum=car;
  int endItem=lengthBtDataPacket;

  btDataPacket[i]=car;
   
  //OBTENER PRIMERO
  i++; 
  if (!getChar(car)) return false;     
  
  while (i<endItem){
  //TRATAR
    btDataPacket[i]=car;
    checksum+=car;
  //OBTENER
    i++;
    if (!getChar(car)) return false;     
  }
  if (checksum!=car){
    initBTDataPacket(btDataPacket);
    btSerial.println("Checksum Error");
    return false;
  }

  return true;
}
