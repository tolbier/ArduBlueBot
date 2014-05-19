#include <SoftwareSerial.h>


unsigned char packet_index=0;

int joyX=0;
int joyY=0;
int slide1=0;
int slide2=0;

void setup()  { 
  setup_bluetooth_mod();
  setup_motor_mod();
  
  Serial.begin(57600);

} 

void loop(){
  loop_bluetooth_mod();
  loop_auto_mod();
  loop_motor_mod();
}
     
