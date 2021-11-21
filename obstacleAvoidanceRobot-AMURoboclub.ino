#include <Servo.h>                                //Import library to control the servo

const int motor1dir = 2;
const int motor2dir = 7; 
const int motor1ang = 3;
const int motor2ang = 6;
const int gndPin = 8;
Servo servoLook;
Servo myservo2;                              // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos2 = 0;    // variable to store the servo position
//Create an object to control the servo

byte trig = 13;                                    //Assign the ultrasonic sensor pins
byte echo = 5;
byte maxDist = 150;                               //Maximum sensing distance (Objects further than this distance are ignored)
byte stopDist = 50;                               //Minimum distance from an object to stop in cm
float timeOut = 2 * (maxDist + 10) / 100 / 340 * 1000000; //Maximum time to wait for a return signal

byte motorSpeed = 40;                             //The maximum motor speed
int motorOffset = 5;                             //Factor to account for one side being more powerful
int turnSpeed = 10;                               //Amount to add to motor speed when turning


void setup()
{
  Serial.begin(9600);
  pinMode(motor1dir, OUTPUT);
  pinMode(motor2dir, OUTPUT);
  pinMode(motor2ang, OUTPUT);
  pinMode(motor1ang, OUTPUT);
  digitalWrite(gndPin, LOW);
  digitalWrite(4, LOW);

  servoLook.attach(11);                           //Assign the servo pin
  myservo2.attach(A2);
  pinMode(trig, OUTPUT);                          //Assign ultrasonic sensor pin modes
  pinMode(echo, INPUT);
}

void loop()
{
  Serial.println("Move Forward");
  digitalWrite(motor1dir, LOW);
  digitalWrite(motor2dir , LOW);
  analogWrite(motor2ang , motorSpeed);
  analogWrite(motor1ang , motorSpeed);
  delay(2000);
  Serial.println("Move Forward");
  digitalWrite(motor1dir, HIGH);
  digitalWrite(motor2dir , HIGH);
  analogWrite(motor2ang , motorSpeed);
  analogWrite(motor1ang , motorSpeed);
  delay(2000);
  
//  //   for (pos2 = 0; pos2 <= 180; pos2 += 1) { // goes from 0 degrees to 180 degrees
//  //    // in steps of 1 degree
//  //    myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
//  //    delay(15);                       // waits 15 ms for the servo to reach the position
//  //  }
//  //  for (pos2 = 180; pos2 >= 0; pos2 += 1) { // goes from 180 degrees to 0 degrees
//  //    myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
//  //    delay(15);                       // waits 15 ms for the servo to reach the position
//  //  }
//  servoLook.write(90);                            //Set the servo to look straight ahead
//  delay(750);
//  int distance = getDistance();                   //Check that there are no objects ahead
//  Serial.print( "Front Distance: ");
//  Serial.println(distance);
//  Serial.print("In loop vla distance: ");
//  Serial.println(distance);
//  moveForward();
//  //Stop the motors
//  if (distance < stopDist) {
//    stopMove();
//    int turnDir = checkDirection();
//    Serial.println(turnDir);
//    moveReverse();
//                                    //Check the left and right object distances and get the turning instruction
//    switch (turnDir)                                //Turn left, turn around or turn right depending on the instruction
//    {
//      case 0:                                       //Turn left
//        Serial.println("Turn Left");
//        turnLeft (500);
//        break;
//      case 1:                                       //Turn around
//        Serial.println("Turn Around");
//        turnLeft (1000);
//        break;
//      case 2:                                       //Turn right
//        Serial.println("Turn Right");
//        turnRight (500);
//        break;
//    }
//  }
}

void accelerate()                                 //Function to accelerate the motors from 0 to full speed
{
  for (int i = 0; i < motorSpeed; i++)            //Loop from 0 to full speed
  {
    digitalWrite(motor1dir, LOW);
    digitalWrite(motor2dir , LOW);
    analogWrite(motor2ang , i);
    analogWrite(motor1ang , i + motorOffset);
    delay(10);
  }
}

void decelerate()                                 //Function to decelerate the motors from full speed to zero
{
  for (int i = motorSpeed; i != 0; i--)           //Loop from full speed to 0
  {
    digitalWrite(motor1dir, LOW);
    digitalWrite(motor2dir , LOW);
    analogWrite(motor2ang , i);
    analogWrite(motor1ang , i + motorOffset);
    delay(10);
  }
}

void moveForward()                                //Set all motors to run forward
{
  Serial.println("Move Forward");
  digitalWrite(motor1dir, LOW);
  digitalWrite(motor2dir , LOW);
  analogWrite(motor2ang , motorSpeed);
  analogWrite(motor1ang , motorSpeed);
}

void stopMove()                                   //Set all motors to stop
{
  Serial.println("Stop");
  analogWrite(motor1ang, 0);
  analogWrite(motor2ang, 0);
}

void moveReverse()                                   //Set all motors to stop
{
  Serial.println("Move Reverse");
  digitalWrite(motor1dir, HIGH);
  digitalWrite(motor2dir , HIGH);
  analogWrite(motor2ang , motorSpeed);
  analogWrite(motor1ang , motorSpeed);
  delay(1000);
}

void turnLeft(int duration)                                 //Set motors to turn left for the specified duration then stop
{
  Serial.println("Turn Left");
  digitalWrite(motor1dir, HIGH);
  digitalWrite(motor2dir , LOW);
  analogWrite(motor2ang , motorSpeed * 1.2);
  analogWrite(motor1ang , motorSpeed * 1.2);
  delay(duration);

  digitalWrite(motor1dir, LOW);
  digitalWrite(motor2dir , LOW);
  analogWrite(motor2ang , motorSpeed + motorOffset);
  analogWrite(motor1ang , motorSpeed + motorOffset);

}

void turnRight(int duration)                                //Set motors to turn right for the specified duration then stop
{
  Serial.println("Turn Right");
  digitalWrite(motor1dir, LOW);
  digitalWrite(motor2dir , HIGH);
  analogWrite(motor2ang , motorSpeed * 1.2);
  analogWrite(motor1ang , motorSpeed * 1.2);
  delay(duration);

  digitalWrite(motor1dir, LOW);
  digitalWrite(motor2dir , LOW);
  analogWrite(motor2ang , motorSpeed + motorOffset);
  analogWrite(motor1ang , motorSpeed + motorOffset);
}

int getDistance()                                   //Measure the distance to an object
{
  unsigned long pulseTime;                          //Create a variable to store the pulse travel time
  int distance;                                     //Create a variable to store the calculated distance
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);                         //Generate a 10 microsecond pulse
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pulseTime = pulseIn(echo, HIGH, timeOut);         //Measure the time for the pulse to return
  Serial.print("Pulse Time: ");
  Serial.println(pulseTime);
  distance = 2 * pulseTime * 0.034 / 2;   //Calculate the object distance based on the pulse time

  return distance;
}

int checkDirection()                                            //Check the left and right directions and decide which way to turn
{
  int distances [2] = {0, 0};                                   //Left and right distances
  int turnDir = 1;                                              //Direction to turn, 0 left, 1 reverse, 2 right
  servoLook.write(180);                                         //Turn servo to look left
  delay(1000);
  distances [0] = getDistance();
  Serial.print( "Left Distance: ");
  Serial.println(distances[0]);
  delay(1500);                                                 //Get the left object distance
  servoLook.write(20);                                           //Turn servo to look right
  delay(1000);
  distances [1] = getDistance();                                //Get the right object distance
  Serial.print( "Right Distance: ");
  Serial.println(distances[1]);
  delay(1500);
  if (distances[0] >= 200 && distances[1] >= 200)               //If both directions are clear, turn left
    turnDir = 0;
  else if (distances[0] <= stopDist && distances[1] <= stopDist) //If both directions are blocked, turn around
    turnDir = 1;
  else if (distances[0] >= distances[1])                        //If left has more space, turn left
    turnDir = 0;
  else if (distances[0] < distances[1])                         //If right has more space, turn right
    turnDir = 2;
  return turnDir;
}
