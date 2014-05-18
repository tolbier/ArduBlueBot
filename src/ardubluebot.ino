#include <SoftwareSerial.h>


//SoftwareSerial miSerial =  SoftwareSerial(rxPin, txPin);

int rightMotor[] = {6, 5, 7}; //ENA , Reverse1,Forward2 (MOTORA) -RIGHT
int leftMotor[] = {3, 4, 2}; //ENB , Reverse3,Forward4 (MOTORB) -LEFT
int left = 0; 
int right = 0; 
unsigned char data_packet[6];
unsigned char packet_index=0;

int joyX=0;
int joyY=0;
int slide1=0;
int slide2=0;

void setup()  { 
  setup_bluetooth_mod();
  
  // use the baud rate your bluetooth module is configured to 
 // pinMode(rxPin, INPUT);
  //pinMode(txPin, OUTPUT);
  
  Serial.begin(57600);

 pinModeMotor(rightMotor,OUTPUT);
 pinModeMotor(leftMotor,OUTPUT);


} 

void loop(){
  loop_bluetooth_mod();
  
  process_data();
  
  
  setMotors();


}
void pinModeMotor(int motor[],int mode ){
  for (int i = 0;i<=2; i++){
    pinMode(motor[i], mode);
  }
} 

int  sigNumb(int value){
  return ( (value>0) - (value<0));
}

int speedMotor(int aX, int aY){
  int signX =sigNumb(aX);
  int signY =sigNumb(aY);

  int retorno=0;
  if ((signX * signY)>0){
     long lX= (long)sq((long)aX);
     long lY= (long)sq((long)aY);
     retorno= (int)((sqrt(lX+lY)) * signX);
  }else{
     retorno= aX+aY;
  }
  retorno= constrain(retorno,-255,255);  
  return retorno;
}


void process_data()
{
  right = speedMotor(-joyX,joyY);
  left = speedMotor(joyX,joyY);
  Serial.print(joyX,DEC);
  Serial.print(" ");
  //Serial.print(" joyY=");
  Serial.print(joyY,DEC);
  Serial.print(" ");
  Serial.print(right,DEC);
  Serial.print(" ");
  //Serial.print(" joyY=");
  Serial.print(left,DEC);
  Serial.println(" ");
  
}

void setMotors()
{
     setMotor(leftMotor, left);
     setMotor(rightMotor, right);
}

void setMotor(int motor[], int value)
{
  boolean forward= (value > 0 ) ;
  
  digitalWrite(motor[2], forward?HIGH:LOW);
  digitalWrite(motor[1], forward?LOW:HIGH);
  analogWrite (motor[0], abs(value));
}
     
     
     
/*     
#include <Servo.h>

Servo motorL;
Servo motorR;
int motorLspeed;
int motorRspeed;

int joyX=0;
int joyY=0;
int slide1=0;
int slide2=0;
int checksum=0;
int double_check=0;


unsigned long last_time=millis();
void setup () {
  Serial.begin(9600);
  motorL.attach(8);
  motorR.attach(9);
}
// Read the data packed from Bluebot's joystick mode
void loop () {
  if (Serial.available()>0) {
    double_check=0;
    if (Serial.read()==125) { //125 indicates a joystick packet
      joyX=Serial.read(); // type mismatch issues occur
        if (joyX>128) joyX=joyX-256; //so make sure we get the negatives correct
      joyY=Serial.read();
        if (joyY>128) joyY=joyY-256;
      slide2=Serial.read();
      slide1=Serial.read();
      checksum=Serial.read(); //see Bluebots help file for checksum
      double_check=(125+joyX+joyY+slide1+slide2)%256;
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
   if (millis()-last_time>500)Serial.println("Time Out");
   if (checksum!=double_check)Serial.println("Checksum Error");
  }
  
  // Map and send motor outputs
  motorLspeed=joyY-joyX;
  motorLspeed=map(motorLspeed,-124,124,0,180);
  motorLspeed=constrain(motorLspeed,0,180);
  
  motorRspeed=joyY+joyX;
  motorRspeed=map(motorRspeed,-124,124,0,180);
  motorRspeed=constrain(motorRspeed,0,180);
  
  motorL.write(motorLspeed);
  motorR.write(motorRspeed);
  
  delay(10);
  
/* Debugging print routine
  Serial.print("X");
  Serial.print(joyX);
  Serial.print(" Y");
  Serial.print(joyY);
  Serial.print(" S1 ");
  Serial.print(slide1);
  Serial.print(" S2 ");
  Serial.print(slide2);
  Serial.print(" Chk ");
  Serial.print(checksum); 
  Serial.print(" DblChk ");
  Serial.print((double_check));
  Serial.print(" MotL ");
  Serial.print(motorLspeed);
  Serial.print(" MotR ");
  Serial.println(motorRspeed);
  delay(100);
//  
} 
*/
     
