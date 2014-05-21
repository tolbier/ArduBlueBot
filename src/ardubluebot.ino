

void setup()  { 
 //Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  setup_bluetooth_mod();
  setup_auto_mod();
  setup_motor_mod();
  setup_distance_mod();
  //applyDecission(2);
} 

void loop(){
  loop_bluetooth_mod();
  loop_auto_mod();
  //loop_auto_SINGLE_mod();
  loop_motor_mod();
}
     
