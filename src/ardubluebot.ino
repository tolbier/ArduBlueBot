

void setup()  { 
  Serial.begin(115200);
  delay(1000);
  setup_bluetooth_mod();
  setup_auto_mod();
  setup_motor_mod();
  setup_distance_mod();

} 

void loop(){
  loop_bluetooth_mod();
  loop_auto_mod();
  loop_motor_mod();
}
     
