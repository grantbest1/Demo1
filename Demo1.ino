#include <Wire.h>
#include <stdio.h>
#include <stdlib.h>


#define SLAVE_ADDRESS 0x04
//int number = 0;
int tate = 0;
int offset = 0;
char data[64] = "";
int i = 0;
float angle = 0.0;
bool decimal_found = false;
int sign = 1;
int decimal_location = 0;
void recieveData(int);
void sendData();

void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);

  Serial.println("Ready!");
}

void loop() {
  delay(100);
}


// callback for received data
void receiveData(int byteCount) {
  Wire.read();
  while (Wire.available()) {
    data[i] = Wire.read();
    //Serial.println(data);
    i++;
  }
  i = 0;
  angle = atof(data);
  Serial.println(angle);

  Serial.print("Size ");
  Serial.println(i);

  Wire.read();
  Wire.flush();
}
