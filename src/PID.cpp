
#include <NewPing.h>
#include "Motors.h"
#include "Config.h"
#include "Sensors.h"


void moveForward() {
  if (wallRight()) pidRight(34, 4);
  else if (wallLeft()) pidLeft(16, 17);
}



