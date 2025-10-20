void pidLeft(int ECHO, int TRIG);
void pidRight(int ECHO, int TRIG);

float getDistanceCm();
void initMotors();

void turn(bool direction);

void moveForwardSpeed(int leftSpeed, int rightSpeed);

void stop();