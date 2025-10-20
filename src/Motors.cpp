
#include <NewPing.h>

#include<Arduino.h>
#include "Config.h"


volatile long ticks1 = 0;
volatile long ticks2 = 0;

void encoder1A() { ticks1++; }
void encoder2A() { ticks2++; }

void initMotors(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(ENC1A, INPUT_PULLUP);
  pinMode(ENC1B, INPUT_PULLUP);
  pinMode(ENC2A, INPUT_PULLUP);
  pinMode(ENC2B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC1A), encoder1A, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2A), encoder2A, RISING);
}

void moveForwardSpeed(int leftSpeed, int rightSpeed) {
  // المحرك الأيسر
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, leftSpeed);

  // المحرك الأيمن
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, rightSpeed);
}

void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

float getDistanceCm() {
   float wheel_circumference = 3.1416 * WHEEL_DIAMETER;
 float distance_per_tick = wheel_circumference / FULL_ROTATION_TICKS;
  long avg_ticks = (ticks1 + ticks2) / 2;   // المتوسط بين العجلتين
  float distance_cm = avg_ticks * distance_per_tick;
  return distance_cm;
}

void turn(bool direction){
  ticks1 = 0;
  ticks2 = 0;
  if(direction){
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }else{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  analogWrite(ENA,150);
  analogWrite(ENB,150);
  // تحريك العجلات للأمام والخلف
  

  // الانتظار حتى الوصول للنبضات
  while((abs(ticks1) + abs(ticks2))/2 < TURN_90) {
    // مجرد انتظار، يمكن إضافة PID لاحقاً
  }

  // إيقاف الموتور
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}


void pidLeft(int ECHO, int TRIG) {
  ticks1=0;
  ticks2=0;
  float mileage = 0;
  NewPing sonar(TRIG, ECHO, MAX_DISTANCE);
  float Kp = 1.4;  // تناسب (كلما زادت، تستجيب أسرع لكن ممكن تتذبذب)
  float Ki = 0.0;  // تكامل (يفيد بتصحيح الانحياز، غالباً نتركه 0 بالبداية)
  float Kd = 8.0;  // مشتقة (تقلل التذبذب)

  float setpoint = 4.0;  // المسافة المطلوبة من الحائط
  float integral = 0;
  float lastError = 0;

  int baseSpeedL = 90;
  int baseSpeedR = 60;  // السرعة الأساسية
  while (mileage < 16.8) {
    float distance = sonar.ping_cm();
    if (distance <= 0.1) distance = MAX_DISTANCE;

    // حساب الخطأ
    float error = distance - setpoint;

    // PID حساب
    integral += error;
    float derivative = error - lastError;
    float correction = Kp * error + Ki * integral + Kd * derivative;
    lastError = error;

    // تعديل السرعات
    int leftSpeed = baseSpeedL - correction;
    int rightSpeed = baseSpeedR + correction;

    // تأمين القيم ضمن النطاق المسموح
    leftSpeed = constrain(leftSpeed, 60, 255);
    rightSpeed = constrain(rightSpeed, 60, 255);

    // تحريك السيارة للأمام
    moveForwardSpeed(leftSpeed, rightSpeed);
    mileage=getDistanceCm();
    //طباعة القيم للتتبع
    Serial.print("Dist: ");
    Serial.print(distance);
    Serial.print(" cm | Err: ");
    Serial.print(error);
    Serial.print(" | Corr: ");
    Serial.print(correction);
    Serial.print(" | L: ");
    Serial.print(leftSpeed);
    Serial.print(" | R: ");
    Serial.println(rightSpeed);

    delay(100);
  }
}
void pidRight(int ECHO, int TRIG) {
    delay(100);
  //hahaha
}