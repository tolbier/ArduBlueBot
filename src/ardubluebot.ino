

void setup()  { 
  setup_bluetooth_mod();
  setup_auto_mod();
  setup_motor_mod();
  Serial.begin(57600);

} 

void loop(){
  loop_bluetooth_mod();
  loop_auto_mod();
  loop_motor_mod();
}
     
