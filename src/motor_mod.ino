int rightMotor[] = {6, 5, 7}; //ENA , Reverse1,Forward2 (MOTORA) -RIGHT
int leftMotor[] = {3, 4, 2}; //ENB , Reverse3,Forward4 (MOTORB) -LEFT
int leftV = 0; 
int rightV = 0; 


void loop_motor_mod()
{
     setMotor(leftMotor, leftV);
     setMotor(rightMotor, rightV);
}

void setup_motor_mod(){
 pinModeMotor(rightMotor,OUTPUT);
 pinModeMotor(leftMotor,OUTPUT);
}

void pinModeMotor(int motor[],int mode ){
  for (int i = 0;i<=2; i++){
    pinMode(motor[i], mode);
  }
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
void stopMotors(){
  rightV=0;
  leftV =0;
}
void motorsGoAhead(){
  rightV=255;
  leftV =255;
}
void setMotorsValues(int _leftV,int _rightV){
  rightV=_rightV;
  leftV=_leftV;
}
void setSpeedMotorFromCoords(int joyX, int joyY){
        rightV = speedMotor(-joyX,joyY);
        leftV = speedMotor(joyX,joyY); 
}
void setMotor(int motor[], int value)
{
  boolean forward= (value > 0 ) ;
  
  digitalWrite(motor[2], forward?HIGH:LOW);
  digitalWrite(motor[1], forward?LOW:HIGH);
  analogWrite (motor[0], abs(value));
}
