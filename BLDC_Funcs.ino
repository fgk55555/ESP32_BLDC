float degToRad(float deg){
  return deg * (PI/180);
}

float radToDeg(float rad){
  return rad * (180/PI);
}
void writePhases(){ //Analog sinusoidal commutation
  phaseU = sin(degToRad(angle));
  phaseV = sin(degToRad(angle+120));
  phaseW = sin(degToRad(angle+240));

  phaseU = Speed * phaseU; //Set the phase
  phaseV = Speed * phaseV;
  phaseW = Speed * phaseW;

  setPhases();  
}

void printValues(int state){
  for(int 
  Serial.print(String(state));Serial.print(":\t");
  Serial.print(bemf[0]);Serial.print('\t'); //BEMF
  Serial.print(bemf[1]);Serial.print('\t');
  Serial.print(bemf[2]);Serial.print("\t\t");
  
  Serial.print(phaseU,0);Serial.print('\t'); //Phase state
  Serial.print(phaseV,0);Serial.print('\t');
  Serial.print(phaseW,0);Serial.print('\t');

  Serial.print(states[0]);Serial.print('\t'); //Sensors
  Serial.print(states[1]);Serial.print('\t');
  Serial.print(states[2]);Serial.print('\n');
}

void bemfStep(){
  if(bldcState == STATE1){
    phaseU = Speed;
    phaseV = 0;
    phaseW = -Speed;
  }
  else if(bldcState == STATE2){
    phaseU = Speed;
    phaseV = -Speed;
    phaseW = 0;
  }
  else if(bldcState == STATE3){
    phaseU = 0;
    phaseV = -Speed;
    phaseW = Speed;
  }
  else if(bldcState == STATE4){
    phaseU = -Speed;
    phaseV = 0;
    phaseW = Speed;
  }
  else if(bldcState == STATE5){
    phaseU = -Speed;
    phaseV = Speed;
    phaseW = 0;
  }
  else if(bldcState == STATE6){
    phaseU = 0;
    phaseV = Speed;
    phaseW = -Speed;
  }
}

void sensoredStep(){
  if(states[0] && states[1] && !states[2]){ //Phase 0 degrees
    bldcState = STATE1;
    phaseU = Speed;
    phaseV = 0;
    phaseW = -Speed;
  }
  else if(states[0] && !states[1] && !states[2]){ //Phase 60
    bldcState = STATE2;
    phaseU = Speed;
    phaseV = -Speed;
    phaseW = 0;
  }
  else if(!states[0] && !states[1] && !states[2]){ //Phase 120
    bldcState = STATE3;
    phaseU = 0;
    phaseV = -Speed;
    phaseW = Speed;
  }
  else if(!states[0] && !states[1] && states[2]){ //Phase 180
    bldcState = STATE4;
    phaseU = -Speed;
    phaseV = 0;
    phaseW = Speed;
  }
  else if(!states[0] && states[1] && states[2]){ //Phase 240
    bldcState = STATE5;
    phaseU = -Speed;
    phaseV = Speed;
    phaseW = 0;
  }
  else if(states[0] && states[1] && states[2]){ //Phase 300
    bldcState = STATE6;
    phaseU = 0;
    phaseV = Speed;
    phaseW = -Speed;
  }
  setPhases();
}

void setPhases(){  
  if(phaseU <0){
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, 0.00);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_B, phaseU);
  }
  else if(phaseU >= 0){
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_B, 0.00);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, phaseU);
  }
  if(phaseV <0){
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_GEN_B, 0.00);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_GEN_B, phaseV);
  }
  else if(phaseV >= 0){
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_GEN_B, 0.00);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_GEN_A, phaseV);
  }
  if(phaseW <0){
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_GEN_B, 0.00);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_GEN_B, phaseV);
  }
  else if(phaseW >= 0){
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_GEN_B, 0.00);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_GEN_A, phaseW);
  }
}

void SerialProcess(){
  
  while(Serial.available()) {
    String inputString = "";
    byte i = 0;
    char inChar = (char)Serial.read();  
    if (inChar >= 'a' && inChar <= 'z' || inChar >= '0' && inChar <= ':' || inChar == '\n'
        || inChar == '\r' || inChar == '-') { //Picks up characters A through Z and 0 - :
      inputString += inChar;
    }
    if (inChar == '\n' || inChar == '\r') {
      Serial.flush();
    }
    int len = inputString.length();
    char inputChars[20];
    for (i = 0; i <= len; ++i)
      inputChars[i] = inputString[i];
    inputChars[i + 1] = '\n';
    char* command = strtok(inputChars, ":"); //break up input string
    char pieces[3][10]; //Command:Data
    i = 0;
  
    while (command != NULL) 
    { //iterate through sections
      strcpy(pieces[i++], command);
      command = strtok(NULL, ":"); //This moves to next chunk
    }
      
    if(inputString.startsWith("s")){ //Stop
      Speed = 0;
      Serial.println("Stopping");
    }
    if(inputString.startsWith("r")){ //Report values
      report = !report;
    }
    else if(inputString.startsWith("p")){ //Set the speed
      //Speed = atof(pieces[1]);
      Speed = 20;
      Serial.print("Setting speed: ");Serial.println(Speed);
    }
    else if(inputString.startsWith("u")){ //Set the speed
      Speed = Speed+5;
      if(Speed < 100){
        Speed = 100;
      }
      Serial.print("Setting speed: ");Serial.println(Speed);
    }
    else if(inputString.startsWith("d")){ //Set the speed
      Speed = Speed-5;
      if(Speed < 0){
        Speed = 0;
      }
      Serial.print("Setting speed: ");Serial.println(Speed);
    }
    else if(inputString.startsWith("a")){ //
      bemfCommutate = !bemfCommutate;
    }
  }
}
