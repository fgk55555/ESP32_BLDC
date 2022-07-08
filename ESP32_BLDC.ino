#include <Arduino.h>
#include "driver/mcpwm.h"

#define H1 18 //SCK
#define L1 23 //MOSI
#define H2 2  //D9
#define L2 13 //D7
#define H3 14 //D6
#define L3 0  //D5
#define BEMF1 A4
#define BEMF2 A3
#define BEMF3 A2
#define HALL1 D12
#define HALL2 D11
#define HALL3 D10
#define STATE1  3
#define STATE2  1
#define STATE3  0
#define STATE4  4
#define STATE5  6
#define STATE6  7
#define PWM_DEFAULT_FREQ   20000
#define OFF 0
#define FORWARD 1
#define REVERSE 0

float phaseU, phaseV, phaseW; //Phase throttle per channel
float angle = 0;
float Speed = 0; //Throttle
//unsigned int states[3] = {0};
unsigned int bemf[3] = {0};
unsigned int prevBemf[3] = {0}; //Initial states on change of bldc state
int storedData[7][1000];
int index = 0;
byte orderIndex = 0;
byte bldcState = 0; //Where we are in the rotation (1-6)
bool report = false;
bool dir = FORWARD;
bool bemfCommutate = false;
bool useHallSensors = true;


void setup() {  
  Serial.begin(1000000);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, H1);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, L1);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, H2);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, L2);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, H3);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, L3);

  pinMode(BEMF1, INPUT);
  pinMode(BEMF2, INPUT);
  pinMode(BEMF3, INPUT);
  pinMode(HALL1, INPUT_PULLUP);
  pinMode(HALL2, INPUT_PULLUP);
  pinMode(HALL3, INPUT_PULLUP);
  
  mcpwm_config_t pwm_config;
  pwm_config.frequency = PWM_DEFAULT_FREQ;
  pwm_config.cmpr_a = 0;
  pwm_config.cmpr_b = 0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);
}

void loop() {
  bemf[0] = analogRead(BEMF1);
  bemf[1] = analogRead(BEMF2);
  bemf[2] = analogRead(BEMF3);
  
  /*if(bemfCommutate){
    bemfStep();
  }*/
  states[0] = digitalRead(HALL1);
  states[1] = digitalRead(HALL2);
  states[2] = digitalRead(HALL3);
  bldcState = digitalRead(HALL1) + (digitalRead(HALL2) << 1) + (digitalRead(HALL3) << 2);
  sensoredStep();
  //bemfStep();

  if(report){
    storedData[1][i] = bemf[1];
    storedData[2][i] = bemf[2];
    storedData[3][i] = bemf[3];
    storedData[4][i] = phaseU;
    storedData[5][i] = phaseV;
    storedData[6][i] = phaseW;
    ++index;
    if(index == 1000){
      Speed = 0;
      Serial.println("Stopping");
      sensoredStep();
      printValues
    }
  }
  SerialProcess();  //Check and handle serial commands
}
