#include<ArduinoSTL.h>
#include "Motors.h"
#include "Sensors.h"
#include "Algorithm.h"
#include "PID.h"

void setup(){
    initMotors();
}
void loop(){
    /*if(wallRight()) moveForward();
    stop();
    delay(2000);
    if(wallLeft()) moveForward();
    stop();*/
    pidLeft(A5,A4);
    while(true);
}