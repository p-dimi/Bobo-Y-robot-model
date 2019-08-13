// connect motor controller pins to Arduino digital pins
// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;

// motor control
int motor_speed = 255;
int dir = 0;

// timing mechanism
//unsigned long previousMillis = 0;
//long interval = 1000;

// Proximity sensor mechanism
//############################################################
// defines pins numbers
const int trigPin = 11;
const int echoPin = 12;

// defines variables
long duration;
// distance is in centimeters
int distance;
// allowed distance before behavior change (in centimeters)
int dis_thresh = 32;
// distance to reverse at (if something's too close, move backwards)
int dis_reverse = 15;
// overshoot reading gotten when too close
int overshoot = 3000;
// boolean storing if obstacle detected
bool obs_dect = false;
//############################################################

// Navigational parameters
int rand_dir = 0;

void setup()
{
// set all the motor control pins to outputs
pinMode(enA, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
// proximity sensor pins
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

// Motor controls
//############################################################
void moveMotorA(int dir, int spd){
  if(dir == 1){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if(dir == -1){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  analogWrite(enA, spd);
}

void moveMotorB(int dir, int spd){
  if(dir == 1){
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else if(dir == -1){
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  analogWrite(enB, spd);
}

void turnLeft(){
  moveMotorB(1, motor_speed);
  moveMotorA(-1, motor_speed);
}

void turnRight(){
  moveMotorB(-1, motor_speed);
  moveMotorA(1, motor_speed);
}

void moveStraight(){
  moveMotorB(-1, motor_speed);
  moveMotorA(-1, motor_speed);
}

void moveBack(){
  moveMotorB(1, motor_speed);
  moveMotorA(1, motor_speed);
}

void stopMovement(){
  moveMotorB(0, 0);
  moveMotorA(0, 0);
}
//############################################################

// Proximity sensor controls
//############################################################
void getDistance(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(200);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration*0.034/2;
}
//############################################################

void loop()
{
  // before moving, get distance from front obstacle
  getDistance();
  // detect obstacle
  if((distance > dis_thresh) && (distance < overshoot)){
    // reset obstacle detected
    obs_dect = false;
    // reset random direction
    rand_dir = 0;
  } else {
    obs_dect = true;
  }
  // if we're not facing an obstacle
  if(obs_dect == false){
    moveStraight();
    // otherwise, if we ARE facing an obstacle
  } else {
    // pick a direction at random, if one is not picked yet
    if(distance < dis_reverse){
      // if obstacle too close, reverse
      moveBack();
    } else{
      if(rand_dir == 0){
        rand_dir = random(1, 3);
        // 1 is left, 2 is right, 0 is blank
    }
    }
    // turn in the random direction
    if(rand_dir == 1){
      // turn left
      turnLeft();
    } else if (rand_dir == 2){
      // turn right
      turnRight();
    }
  }
}
