#include <Servo.h> 

#define AUTO_ROBOT_MODE 1
#define BT_ROBOT_MODE 2
#define DISTANCES_TABLE_LENGTH 9
#define delay4servoStep 50
#define SERVO_STEP 20
#define SERVO_MIN_POSITION 10
#define SERVO_MAX_POSITION 170

#define blockedValue 30
long distances_table[DISTANCES_TABLE_LENGTH];

int leftDecission[10]={-255,-200 ,0  ,200  ,255,255 ,255,255 ,255,-255};
int rightDecission[10]={255,255,255,255,255,200,0 ,-200,-255,-255};
int timeDecission[10]={500,500,500,500,500,500,500,500,500,500};
boolean aplyingDecission=false;
unsigned long finishtimeDecission;
Servo servo1;
int robotMode = BT_ROBOT_MODE;
int bot_direction=4;
void loop_auto_mod(){
  static boolean initial_scan=true;
  if (robotMode==AUTO_ROBOT_MODE){
    if (initial_scan){    
      while (getDistancesMap()!=(DISTANCES_TABLE_LENGTH-1));
      initial_scan=false;
    }
    getDistancesMap();
    if (aplyingDecission) {
       if (millis()>finishtimeDecission){
         aplyingDecission=false;
         motorsGoAhead();
       }
    }else{
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
  if (getDirectionDistanceMinValue(bot_direction)<blockedValue){
      //Buscar Mejor Direccion
       int bestDirection=getBestDirection();
      //AplicarDecision
      applyDecission(bestDirection);
      //Ir de Frente
  }
      
}

void applyDecission(int dir){
  aplyingDecission=true;
  setMotorsValues(leftDecission[dir],rightDecission[dir]);
  finishtimeDecission= timeDecission[dir]+millis();
  
}
void setup_auto_mod(){
  servo1.attach(10);
  Serial.print("setup_auto_mod");
}
int getDistancesMap(){
  static unsigned long lasttime=millis();

  int idx;
  if (millis()-lasttime > delay4servoStep){
    int servo_position=move_servo_next_position();
    long distance=getDistanceValue();
    idx=(servo_position-10)/20;
    distances_table[idx]=distance;
    
     if (idx==8 || idx==0){
      for(int i=0;i<=8;i++){
        
        Serial.print (distances_table[i]);
        Serial.print("\t");
      }
      Serial.println();
    }
    
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

