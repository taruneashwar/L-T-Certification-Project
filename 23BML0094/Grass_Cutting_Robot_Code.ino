// Motor A pins to Arduino (via first L293D)
int enA = 9, in1 = 8, in2 = 7;
// Motor B pins to Arduino (via first L293D)
int enB = 10, in3 = 6, in4 = 5;

// Blade motor pins to Arduino (via second L293D)
int bladeEN = 2, bladeIN1 = 3, bladeIN2 = 4;

// Ultrasonic sensor pins to Arduino (via Ultrasonic sensor- HC SR04)
int trig = 12;
int echo = 11;

// Declaring variables
long duration;
int distance, objectdistance = 20, waitafterstopping = 500;
int waitaftergoingback = 800, waitaftergoingright = 600;

void setup() {
  // Motor A & B pins
  pinMode(enA, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  
  // Blade motor pins
  pinMode(bladeEN, OUTPUT); pinMode(bladeIN1, OUTPUT); pinMode(bladeIN2, OUTPUT);
  startBlade();  // Initially ON

  // Ultrasonic sensor pins
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //Setting Baud Rate
  Serial.begin(9600);
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  duration = pulseIn(echo, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No echo received");
    stopMotors();
    stopBlade();
    delay(200);
    return;
  }
  //Calculating distance
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < objectdistance) {
    Serial.println("Object nearby, stopped cutting grass!");
    Serial.println("Robot is avoiding object.");
    stopBlade();      // Stop blade motor
    stopMotors();     // Stop drive motors
    delay(waitafterstopping);
    moveBackward();   // Reverse
    delay(waitaftergoingback);
    turnRight();      // Turn
    delay(waitaftergoingright);
  } else {
    Serial.println("Grass is getting cut!");
    startBlade();     // Run blade motor
    moveForward();    // Continue forward
  }

  delay(100);
}

// Movement Functions
void moveForward() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  analogWrite(enA, 200);

  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  analogWrite(enB, 200);
}

void moveBackward() {
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  analogWrite(enA, 200);

  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  analogWrite(enB, 200);
}

void turnRight() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  analogWrite(enA, 200);

  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  analogWrite(enB, 200);
}

void stopMotors() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

// Blade Motor Control Functions (via second L293D)
void startBlade() {
  digitalWrite(bladeIN1, HIGH);
  digitalWrite(bladeIN2, LOW);
  analogWrite(bladeEN, 200);  // Blade speed
}

void stopBlade() {
  digitalWrite(bladeIN1, LOW);
  digitalWrite(bladeIN2, LOW);
  analogWrite(bladeEN, 0);
}
