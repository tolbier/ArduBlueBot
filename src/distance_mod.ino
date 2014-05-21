#define trigPin 12
#define echoPin 11

#define MAX_DISTANCE_VALUE 200
void setup_distance_mod(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
long getDistanceValue(){

  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance>MAX_DISTANCE_VALUE) distance=MAX_DISTANCE_VALUE;
  return distance;
}

int getMaxDistanceValue(){
  return MAX_DISTANCE_VALUE;
}
