
#include <NewPing.h>
#include "Config.h"

const int WALL=9;



void initSensors(){
    
}

bool wallRight(){
    NewPing sonar(SRtrig, SRecho, MAX_DISTANCE);
    float distance = sonar.ping_cm();
    if(distance<=WALL) return true;
    else return false;
}

bool wallLeft(){
    NewPing sonar(SLtrig, SLecho, MAX_DISTANCE);
    float distance = sonar.ping_cm();
    if(distance<=WALL) return true;
    else return false;
}

bool wallFront(){
    //NewPing sonar(SFtrig, SFecho, MAX_DISTANCE);
    //float distance = sonar.ping_cm();
    //if(distance<=WALL) return true;
    //else return false;
}