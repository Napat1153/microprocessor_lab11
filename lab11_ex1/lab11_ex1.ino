#include <SPI.h> 
#define DATA_PIN 11
#define CLOCK_PIN 13
#define SS_PIN_1 10
#define SS_PIN_2 9
#define EncoderB 3
#define EncoderA 2
#define MOTOR_D1 8
#define MOTOR_D2 7
#define MOTOR_PWM 6
#define LED 4

volatile int enCount = 0;
volatile int enDir;
int j = 0;

int map7seg[10] = {
   0b00111111, //0
   0b00000110, //1
   0b01011011, //2
   0b01001111, //3
   0b11100110, //4
   0b01101101, //5
   0b01111101, //6
   0b00100111, //7
   0b01111111, //8
   0b01101111, //9
};

void setup(){
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(SS_PIN_1, OUTPUT);
  pinMode(SS_PIN_2, OUTPUT);
  pinMode(EncoderB, INPUT_PULLUP);
  pinMode(EncoderA, INPUT_PULLUP);
  pinMode(MOTOR_D1, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  
  attachInterrupt(digitalPinToInterrupt(EncoderA),
                  ChannelA, RISING);
  attachInterrupt(digitalPinToInterrupt(EncoderB),
                  ChannelB, RISING);
}

void loop(){
  int control_value = 100;
  //int control_value = -100;
  setSpeed(control_value);
  Serial.print(control_value);
  Serial.print(" , ");
  Serial.print(enCount);
  Serial.print(" , ");
  Serial.println(enDir);
}

void moveForward(int v){
  digitalWrite(MOTOR_D1, LOW);
  digitalWrite(MOTOR_D2, HIGH);
  analogWrite(MOTOR_PWM, v);
}

void moveBackward(int v){
  digitalWrite(MOTOR_D1, HIGH);
  digitalWrite(MOTOR_D2, LOW);
  analogWrite(MOTOR_PWM, v);
}

void setSpeed(int i){
  if(i > 0 && i < 255){
    moveForward(i);
  }
  if(i < 0 && i > -255){
    int vv = i*(-1);
    moveBackward(i);
  }
  digitalWrite(LED, enDir);
  int j = enCount/100;
  write7segment(j);
}

void spi_write1(unsigned char cData){
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, cData);
    digitalWrite(SS_PIN_1, HIGH);
    digitalWrite(SS_PIN_1, LOW);
}
void spi_write2(unsigned char cData){
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, cData);
    digitalWrite(SS_PIN_2, HIGH);
    digitalWrite(SS_PIN_2, LOW);
}
void write7segment(int num){
  int tens = num/10;
  int digit = num%10;
  spi_write1(map7seg[tens]);
  spi_write2(map7seg[digit]);
}
void ChannelA() {
  if (digitalRead(EncoderA) == 1 
      && digitalRead(EncoderB) == 0) {
    enCount--;
    enDir = 0;
  }
}
void ChannelB() {
  if (digitalRead(EncoderA) == 0 
      && digitalRead(EncoderB) == 1){
    enCount++;
    enDir = 1;
  }
}
