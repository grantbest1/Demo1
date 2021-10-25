//Include motorshield library
#include "DualMC33926MotorShield.h"

DualMC33926MotorShield md;

//defining pin numbers
#define clk_left 2
#define clk_right 3
#define dt_left 4
#define dt_right 5




//variables
//left variables
int counter_left = 0;
int oldCount_left = 0;
int newCount_left = 0;
int deltaCount_left = 0;
float rad_left;
int currentState_left;
int lastState_left;
String currentDir_left;
int tNew_left;
int tOld_left;
int deltaT_left;


float v_LEFT = 0; // velocity of left encoder
float v_RIGHT = 0; // velocity of right encoder
float deltaDistance_LEFT = 0; // change in left distance
float deltaDistance_RIGHT = 0; // change in right distance

float x_DISTANCE = .001; //total x distance 
float y_DISTANCE = .001; // total y distance 
float phi = .001; // current angle from straight


//right variables
int counter_right = 0;
int oldCount_right = 0;
int newCount_right = 0;
int deltaCount_right = 0;
float rad_right;
int currentState_right;
int lastState_right;
String currentDir_right;
int tOld_right;
int tNew_right;
int deltaT_right;

float r = 0.25;
float b = 0.91;

//// angular velocity
double thetaDot_left; 
double thetaDot_right;

bool flag = false;


//if the motor shield has a fault it will tell the user
void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}


//setup function
void setup() {
  //baud rate set to 250,000
  Serial.begin(9600);
  pinMode(clk_left, INPUT);
  pinMode(clk_right, INPUT);
  pinMode(dt_left, INPUT);
  pinMode(dt_right, INPUT);

  Serial.println("Dual MC33926 Motor Shield");
  
//    Serial.print("LEFT: ");
//    Serial.print("counter_left");
//    Serial.print(" ");
//    Serial.print("rad_left");
//    Serial.print(" ");
//    Serial.print("thetaDot_left");
//    Serial.print(" ");
//    Serial.print("x_DISTANCE");
//    Serial.print("    RIGHT: ");
//    Serial.print("counter_right");
//    Serial.print(" ");
//    Serial.print("rad_right");
//    Serial.print(" ");
//    Serial.print("thetaDot_right");
//    Serial.print(" ");
//    Serial.print("y_DISTANCE");
//    Serial.print("      ");
//    Serial.print("phi");
//  Serial.println();
  
  md.init();
  tOld_left = millis();
  tOld_right = millis();


  
  lastState_left = digitalRead(clk_left);
  lastState_right = digitalRead(clk_right);

  
  attachInterrupt(digitalPinToInterrupt(2), updateLeftEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateRightEncoder, CHANGE);
}



void loop() {
  if(flag == true){
      Serial.print("LEFT: ");
//      Serial.print(counter_left);
//      Serial.print(" ");
//      Serial.print(rad_left);
//      Serial.print(" ");
      Serial.print(thetaDot_left);
      Serial.print(" ");
      Serial.print(v_LEFT);
      Serial.print(" ");
      Serial.print(x_DISTANCE);
      Serial.print("    RIGHT: ");
//      Serial.print(counter_right);
//      Serial.print(" ");
//      Serial.print(rad_right);
//      Serial.print(" ");
      Serial.print(thetaDot_right);
      Serial.print(" ");
      Serial.print(v_RIGHT);
      Serial.print(" ");
      Serial.print(y_DISTANCE);
      Serial.print("      ");
      Serial.print(phi);
//      Serial.print(deltaT_left);
//      Serial.print(" ");
//      Serial.print(deltaCount_left);
//      Serial.print(" ");
//      Serial.print(thetaDot_left);
//      Serial.print(" ");
//      Serial.print(deltaT_right);
//      Serial.print(" ");
//      Serial.print(deltaCount_right);
//      Serial.print(" ");
//      Serial.print(thetaDot_right);
      Serial.println();
      flag = false;
  }
  if(counter_left >=800 && counter_right >=800){
    md.setM1Speed(0);
    md.setM2Speed(0);
  }else{
    md.setM1Speed(200);
    md.setM2Speed(-200);
  }
  
  
}




//this is the ISR for the left encoder
void updateLeftEncoder(){
  currentState_left = digitalRead(clk_left);


  if(currentState_left != lastState_left && currentState_left == 1){
    tNew_left = millis();
    deltaT_left = tNew_left - tOld_left;
    deltaCount_left = newCount_left-oldCount_left;
    //thetadot is the angular velocity
    thetaDot_left = abs((deltaCount_left*1000*2*3.1415)/((deltaT_left)*800));

    v_LEFT = thetaDot_left * r; // actual velocity of the left wheel is calculated
    deltaDistance_LEFT = v_LEFT * deltaT_left/1000; // the distance the left wheel travels per clicks
    x_DISTANCE += (cos(phi)*(deltaDistance_LEFT + deltaDistance_RIGHT))/2; // the full x distance the wheel travels
    y_DISTANCE += (sin(phi)*(deltaDistance_LEFT + deltaDistance_RIGHT))/2; // full y distance the wheel
    phi += (r/b)*(deltaDistance_LEFT - deltaDistance_RIGHT); // calculates the new angle from perfectly straight
    
    oldCount_left = counter_left;
    if(digitalRead(dt_left) != currentState_left){
      counter_left --;
      currentDir_left = "CW";
    }else{
      counter_left ++;
      currentDir_left = "CCW";
    }
    rad_left = (counter_left*2*PI)/800;
    newCount_left=counter_left;
    tOld_left=millis();
    flag = true;
    
  }
  lastState_left = currentState_left;
  
}


//this is the ISR for the right encoder
void updateRightEncoder(){
  currentState_right = digitalRead(clk_right);


  if(currentState_right != lastState_right && currentState_right == 1){
    tNew_right = millis();
    deltaT_right = tNew_right - tOld_right;
    deltaCount_right = newCount_right-oldCount_right;
    //thetadot is the angular velocity
    thetaDot_right = abs((deltaCount_right*1000*2*3.1415)/((deltaT_right)*800));

    v_RIGHT = thetaDot_right * r;
    deltaDistance_RIGHT = v_RIGHT * deltaT_right/1000;
    x_DISTANCE += (cos(phi)*(deltaDistance_LEFT + deltaDistance_RIGHT))/2; // the full x distance the wheel travels
    y_DISTANCE += (sin(phi)*(deltaDistance_LEFT + deltaDistance_RIGHT))/2; // full y distance the wheel
    phi += (r/b)*(deltaDistance_LEFT - deltaDistance_RIGHT); // calculates the new angle from perfectly straight
      
    
    oldCount_right = counter_right;
    if(digitalRead(dt_right) != currentState_right){
      counter_right --;
      currentDir_right = "CW";
    }else{
      counter_right ++;
      currentDir_right = "CCW";
    }
    rad_right = (counter_right*2*PI)/800;
    newCount_right=counter_right;
    tOld_right=millis();
    flag = true;
    
  }
  lastState_right = currentState_right;
  
}
