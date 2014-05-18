#include <SoftwareSerial.h>

#define baudRate_bluetooth_mod 9600

#define rxPin_bluetooth_mod 8
#define txPin_bluetooth_mod 9

unsigned long last_time=millis();
SoftwareSerial btSerial(rxPin_bluetooth_mod, txPin_bluetooth_mod);

void setup_bluetooth_mod(){
  btSerial.begin(baudRate_bluetooth_mod);
}

void loop_bluetooth_mod(){
  int double_check=0;
  int checksum=0;  
  // check if data has been sent from the computer:
  if (btSerial.available()>0) {
    //Serial.println("AV");
    int car = getChar();
    double_check=0;
    if (car==125) { //125 indicates a joystick packet
        joyX = getChar();
        joyY = getChar();
        slide2 = getChar();
        slide1 = getChar();
        checksum = getChar();

        double_check=(125+joyX+joyY+slide1+slide2)%256;        
        if (joyX>128) joyX=joyX-256;
        if (joyY>128) joyY=joyY-256;
        joyX=map(joyX,-124,124,-255,255);
        joyY=map(joyY,-124,124,-255,255);
        
        last_time=millis();
    }
  }
 // Timeout routine to stop motors if signal is lost for 500ms
 // also stops the motors if the checksum indicates lost data
 // This section would be better if I used 2x oversampling and
 // only killed the motors if two checksums in sequence were flawed
 // This section can be commented out, but it does play a safety role
 // in the event that Bluetooth communication is lost

 if ((millis()-last_time)>500||(checksum!=double_check)){
   joyX=0;
   joyY=0;
   slide2=62;
   slide1=62;
   if (millis()-last_time>500)btSerial.println("Time Out");
   if (checksum!=double_check)btSerial.println("Checksum Error");
  }
}

int getChar(){
    unsigned long wait_begin_time=millis();
    while (btSerial.available()<=0
           && ((millis()-wait_begin_time)<100) );
           
    int car = (int) btSerial.read();
//      Serial.print(car,DEC);
//      Serial.print("\t");
    return car;
}
