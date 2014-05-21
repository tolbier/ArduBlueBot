#include <Servo.h> 

#define AUTO_ROBOT_MODE 1
#define BT_ROBOT_MODE 2


#define pinServo 10
#define pinSwitch 8

#define DISTANCES_TABLE_LENGTH 9
#define delay4servoStep 50
#define SERVO_STEP 20
#define SERVO_MIN_POSITION 10
#define SERVO_MAX_POSITION 170

#define blockedValue 30
long distances_table[DISTANCES_TABLE_LENGTH];

int rightDecission[10]={-255,-255,-255,-255, 255, 255, 255, 255, 255,-255};
int leftDecission[10]={  255, 255, 255, 255, 255,-255,-255,-255,-255,-255};
unsigned long timeDecission[10]={  500, 400, 300, 200, 800, 200, 300, 400, 500, 500};

boolean aplyingDecission=false;
unsigned long finishtimeDecission;
Servo servo1;
int robotMode = (digitalRead(pinSwitch)==HIGH)?AUTO_ROBOT_MODE:BT_ROBOT_MODE;
int bot_direction=4;



void loop_auto_mod(){
  static boolean initial_scan=true;
  if (robotMode==AUTO_ROBOT_MODE){
    if (initial_scan){    
      for (int i=0; i<DISTANCES_TABLE_LENGTH;i++){
        getDistancesMap();
      }
    }
    
    if (aplyingDecission) {
       if (( ((unsigned long)millis())>finishtimeDecission)){
         aplyingDecission=false;
         motorsGoAhead();
       }
    }else{
      
      int posi=getDistancesMap();
      motorsGoAhead();
      decissionAlgorithm();
    }
    
  }else{
    initial_scan=true;
  }
}



int getBestDirection(){
  int bestDirection=9;
  long bestDistance=blockedValue;
  for (int i=0;i<DISTANCES_TABLE_LENGTH;i++){
    long dist=getDirectionDistanceMinValue(i);
    if (dist>bestDistance){
      bestDirection=i;
      bestDistance=dist;
    }
  }
  return bestDirection;
}
long getDirectionDistanceMinValue(int bot_direction){
  long minTable[3];
  minTable[0]=distances_table[bot_direction];
  minTable[1]=(bot_direction>0)?distances_table[bot_direction-1]: getMaxDistanceValue();
  minTable[2]=(bot_direction<DISTANCES_TABLE_LENGTH-1)?distances_table[bot_direction+1]:  getMaxDistanceValue();
  
  long minValue= getMaxDistanceValue();
  for (int i=0;i<=2;i++){
    if (minTable[i]<minValue) minValue= minTable[i];
  }
  return minValue;
}
void decissionAlgorithm(){
  //SiDireccion Bloqueada
  long minDistance=getDirectionDistanceMinValue(bot_direction);

  if (minDistance<blockedValue){
      //Buscar Mejor Direccion
       int bestDirection=getBestDirection();
       //printDistancesMap(minDistance,bestDirection);
      //AplicarDecision
      applyDecission(bestDirection);
      //Ir de Frente
  }
      
}
  
void applyDecission(int dir){
  aplyingDecission=true;
  setMotorsValues(leftDecission[dir],rightDecission[dir]);
  //apuntar adelante el servo
  moveServo(90);
  finishtimeDecission=(unsigned long) (((unsigned long)timeDecission[dir])+((unsigned long)millis()));
  
}
void printDistancesMap(long minDistance,int bestDirection){
  static unsigned long lasttime=0;
  if ((millis()-lasttime)> 50) {
    
     for(int i=0;i<=8;i++){
      
       Serial.print (distances_table[i]);
       Serial.print("-");
     }
     Serial.print("mDist");
     Serial.print(minDistance);
     Serial.print("bDirec");
     Serial.print(bestDirection);

     Serial.println();
     lasttime=millis();
    
  }
}
void setup_auto_mod(){  
  pinMode(pinSwitch, INPUT);           // pone el pin como entrada
  digitalWrite(pinSwitch, HIGH);
  servo1.attach(pinServo);
}
int getDistancesMap(){
  static unsigned long lasttime=millis();

  int idx;
  if (millis()-lasttime > delay4servoStep){
    int servo_position=move_servo_next_position();
    long distance=getDistanceValue();
    idx=(servo_position-10)/20;
    distances_table[idx]=distance;

    lasttime=millis();
  } 
  return idx;  
}
int move_servo_next_position(){
    int servo_position = next_servo_position();
    moveServo(servo_position);
    return  servo_position;
}

void moveServo(int servo_position){
  servo1.write(servo_position);
}
int next_servo_position(){
  static int servo_position=SERVO_MIN_POSITION-SERVO_STEP;
  static int servo_step=SERVO_STEP;
  servo_position+=servo_step;
  if (((servo_position>= SERVO_MAX_POSITION) && (servo_step>0)) ){
    servo_position=SERVO_MAX_POSITION;
    servo_step*=-1;
  }else if ((servo_position<= SERVO_MIN_POSITION) && (servo_step<0) ){
    servo_position=SERVO_MIN_POSITION;
    servo_step*=-1;

  }
  return servo_position;
} 

