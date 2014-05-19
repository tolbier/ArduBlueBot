#define AUTO_ROBOT_MODE 1
#define BT_ROBOT_MODE 2

int robotMode = BT_ROBOT_MODE;

void loop_auto_mod(){
  if (robotMode==AUTO_ROBOT_MODE){
    stopMotors();
  }
}
