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
float rad_left;
int currentState_left;
int lastState_left;
String currentDir_left;

int counter_right = 0;
int oldCount_right = 0;
int newCount_right = 0;
float rad_right;
int currentState_right;
int lastState_right;
String currentDir_right;


//// angular velocity
//double thetaDot_left; 
//double thetaDot_right;

bool flag = false;

//setup function
void setup() {
  //baud rate set to 250,000
  Serial.begin(250000);
  pinMode(clk_left, INPUT);
  pinMode(clk_right, INPUT);
  pinMode(dt_left, INPUT);
  pinMode(dt_right, INPUT);

  
  
  lastState_left = digitalRead(clk_left);
  lastState_right = digitalRead(clk_right);

  
  attachInterrupt(digitalPinToInterrupt(2), updateLeftEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateRightEncoder, CHANGE);
}



void loop() {
  if(flag == true){
      Serial.print(counter_left);
      Serial.print(" ");
      Serial.print(rad_left);
      Serial.print("    ");
      Serial.print(counter_right);
      Serial.print(" ");
      Serial.print(rad_right);
      Serial.println();
      flag = false;
      delay(10);
  }
  
}




//this is the ISR for the left encoder
void updateLeftEncoder(){
  currentState_left = digitalRead(clk_left);


  if(currentState_left != lastState_left && currentState_left == 1){
//    tNew = millis();
//    deltaT = tNew - tOld;
//    //thetadot is the angular velocity
//    thetaDot = ((newCount-oldCount)*1000*2*3.1415)/((deltaT)*800);
    
    oldCount_left = counter_left;
    if(digitalRead(dt_left) != currentState_left){
      counter_left ++;
      currentDir_left = "CW";
    }else{
      counter_left --;
      currentDir_left = "CCW";
    }
    rad_left = (counter_left*2*PI)/800;
    newCount_left=counter_left;
//    tOld=millis();
    flag = true;
    
  }
  lastState_left = currentState_left;
  
}


//this is the ISR for the right encoder
void updateRightEncoder(){
  currentState_right = digitalRead(clk_right);


  if(currentState_right != lastState_right && currentState_right == 1){
//    tNew = millis();
//    deltaT = tNew - tOld;
//    //thetadot is the angular velocity
//    thetaDot = ((newCount-oldCount)*1000*2*3.1415)/((deltaT)*800);
    
    oldCount_right = counter_right;
    if(digitalRead(dt_right) != currentState_right){
      counter_right ++;
      currentDir_right = "CW";
    }else{
      counter_right --;
      currentDir_right = "CCW";
    }
    rad_right = (counter_right*2*PI)/800;
    newCount_right=counter_right;
//    tOld=millis();
    flag = true;
    
  }
  lastState_right = currentState_right;
  
}
