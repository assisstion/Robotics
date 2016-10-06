int aBackwards = 6;
int aForwards = 7;
int bBackwards = 8;
int bForwards = 9;

int aSpeedPin = 10;
int bSpeedPin = 11;

bool motorStop = false;

//-2 = sharp left, -1 = left, 0 = center, 1 = right, 2 = sharp right
int turningMode = 1;

int delayPerLoop = 10;

int stopCounter = 0;
int runCounter = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for(int i = 6; i <= 11; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(2, OUTPUT);
}

int counter = 0;

float factor = 0.5;

void loop() {

  // Set thresholds
  int leftThreshold = 275;
  int centerThreshold = 250;
  int rightThreshold = 250;

  // Read photo values
  int left = leftPhoto();
  int center = centerPhoto();
  int right = rightPhoto();

  // Determine whether each photoresistor is black or not
  // Find this by a simple comparison to the threshold
  bool leftBlack = false;
  bool centerBlack = false;
  bool rightBlack = false;

  if(left < leftThreshold){
    leftBlack = true;
  }
  if(center < centerThreshold){
    centerBlack = true;
  }
  if(right < rightThreshold){
    rightBlack = true;
  }

  // Algorithm for finding the left and right motor speeds
  double leftSpeed = 0;
  double rightSpeed = 0;

  // Code to accomodate stopping three seconds
  if(runCounter > 0){
    leftSpeed = 0.8 * factor;
    rightSpeed = 0.8 * factor;
    runCounter--;
  }
  // Code to accomodate stopping three seconds
  else if(stopCounter > 0){
    stopCounter--;
    if(stopCounter <= 0){
      leftSpeed = 0.8 * factor;
      rightSpeed = 0.8 * factor;
      runCounter = 100 / delayPerLoop;
      digitalWrite(2, LOW);
    }
  }
  // Code to accomodate stopping three seconds
  else if(leftBlack && rightBlack && centerBlack){
    leftSpeed = 0.00;
    rightSpeed = 0.00;
    digitalWrite(2, HIGH);
    stopCounter = 3000 / delayPerLoop;
  }
  // Stop when leftBlack and rightBlack but not centerBlack
  else if(leftBlack && rightBlack){
    leftSpeed = 0.05;
    rightSpeed = 0.05;
  } 
  // Code to accomodate turning left
  else if(turningMode == 2){
    //if(centerBlack && !rightBlack && !leftBlack){
    if(rightBlack && !leftBlack){
      turningMode = 0;
    }
    leftSpeed = -0.25 * factor;
    rightSpeed = 0.75 * factor;
  }
  // Code to accomodate turning right
  else if(turningMode == -2){
    if(!rightBlack && leftBlack){
      turningMode = 0;
    }
    leftSpeed = 0.75 * factor;
    rightSpeed = -0.25 * factor;
  }
  // Code to accomodate turning right
  else if(rightBlack){
    if(centerBlack){
      turningMode = -2;
    }
    else{
      turningMode = -1;
    }
    leftSpeed = 0.8 * factor;
    rightSpeed = 0.1 * factor;
  }
  // Code to accomodate turning left
  else if(leftBlack){
    if(centerBlack){
      turningMode = 2;
    }
    else{
      turningMode = 1;
    }
    leftSpeed = 0.1 * factor;
    rightSpeed = 0.8 * factor;
  }
  // Going straight forward
  else if(centerBlack){
    turningMode = 0;
    leftSpeed = 0.8 * factor;
    rightSpeed = 0.8 * factor;
  }
  // Code to accomodate turning left
  else if (turningMode == 1) {
    leftSpeed = 0.1 * factor;
    rightSpeed = 0.5 * factor;
  // Code to accomodate turning right
  } else if (turningMode == -1) {
    leftSpeed = 0.5 * factor;
    rightSpeed = 0.1 * factor;
  }
  // None black and no other condition, go straight forward
  else{
    leftSpeed = 0.6 * factor;
    rightSpeed = 0.6 * factor;
  }

  if (!motorStop) {
    runWithSpeeds(leftSpeed, rightSpeed);
  }

  // Use counter to set println delay to 10 times delayPerLoop
  counter++;

  if(counter >= 10){
  
    Serial.println(left);
    Serial.println(center);
    Serial.println(right);
    Serial.println(leftSpeed);
    Serial.println(rightSpeed);
    Serial.println(turningMode);
    
    Serial.println();
    counter = 0;
  }
  delay(delayPerLoop);
}

void runWithSpeeds(double aSpeed, double bSpeed){
  runWithSpeed(aSpeedPin, aForwards, aBackwards, aSpeed);
  runWithSpeed(bSpeedPin, bForwards, bBackwards, bSpeed);
}

/*
 * speedValue ranges from -1 to 1
 */
void runWithSpeed(int pwmPin, int forwardsPin, int backwardsPin, 
  double speedValue){
  if(speedValue < 0){
    if(speedValue < -1){
      speedValue = -1;
    }
    analogWrite(pwmPin, (int)(-speedValue * 254));
    digitalWrite(forwardsPin, LOW);
    digitalWrite(backwardsPin, HIGH);
  }
  else if(speedValue > 0){
    if(speedValue > 1){
      speedValue = 1;
    }
    analogWrite(pwmPin, (int)(speedValue * 254));
    digitalWrite(forwardsPin, HIGH);
    digitalWrite(backwardsPin, LOW);
  }
  else{
    digitalWrite(forwardsPin, LOW);
    digitalWrite(backwardsPin, LOW);
  }
}

float leftPhoto(){
  return analogRead(A2);
}

float centerPhoto(){
  return analogRead(A1);
}

float rightPhoto(){
  return analogRead(A0);
}

