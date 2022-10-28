
/*
Description: Home alarm System
*/
#include "main.h"   // main header file contain all the global variables , function instantiations , interrupts and other data types needed.

void State_Unset() {
  setLEDstate(led_bits = 0x0003);
  if (Password_right) // if correct input is given state changes to exit state.
  {
    PRESENT_STATE = EXIT;
    StateVariablesInitilize();
    Lcdset();

  } else if (Attempts_lost) // if any wrong code or input is given , state changes to alarm state
  {
    Cause_ERROR = WRONG_password;
    PRESENT_STATE = ALARM;
    StateVariablesInitilize();
    Lcdset();
  }
}

void State_Exit() {
  if (State_NEW) // if present state is exit
  {
    setLEDstate(led_bits = 0x000C);
    turnLEDon.attach( & onLED, 0.25); //led flashes every 250ms
    counter_start.attach( & countDownFlag, 60); //countdown of 60s starts.
    State_NEW = false;
  }

  if (Password_right) // the new state is unset if correct password is input.
  {
    PRESENT_STATE = UNSET;
    StateVariablesInitilize();
    Lcdset();
  } else if (Attempts_lost || High_fullSetZone) // if user enter wrong code for three times or fullsetzone activate
  {
    if (Attempts_lost) Cause_ERROR = WRONG_password;
    else if (High_fullSetZone) Cause_ERROR = ZONE_FULLSET;
    PRESENT_STATE = ALARM; //change to alarm state
    StateVariablesInitilize();
    Lcdset();
  } else if (Time_out) //if timeout of 1 minutes is over, the new state is set.
  {
    PRESENT_STATE = SET;
    StateVariablesInitilize();
    Lcdset();
  }
}

void State_SET() {
  if (State_NEW) {
    setLEDstate(led_bits = 0x0030);
    State_NEW = false;
  }
  if (High_fullSetZone) // if full set zone is on the new state is alarm state
  {
    Cause_ERROR = ZONE_FULLSET;
    PRESENT_STATE = ALARM;
    StateVariablesInitilize();
    Lcdset();
  } else if (High_entryExitZone) //the new state is entry if entry/exit zone trigger.
  {
    PRESENT_STATE = ENTRY;
    StateVariablesInitilize();
    Lcdset();
  }
}

void State_Entry() {
  if (State_NEW) {
    setLEDstate(led_bits = 0x00C0);
    turnLEDon.attach( & onLED, 0.25); //flashes led  every 250ms
    counter_start.attach( & countDownFlag, 120); //countdown of 2 minutes starts.
    State_NEW = false;
  }

  if (Password_right) //the new state is unset if correct password is input.
  {
    PRESENT_STATE = UNSET;
    StateVariablesInitilize();
    Lcdset();
  } else if (Time_out || High_fullSetZone) //if timeout of 1 minutes is over or fullsetzone activates, the new state is set.
  {
    if (High_fullSetZone) Cause_ERROR = ZONE_FULLSET;
    else if (Time_out) Cause_ERROR = NO_PASSWORD;
    PRESENT_STATE = ALARM; //go to alarm state
    StateVariablesInitilize();
    Lcdset();
  }
}


void State_Alarm() {
  if (State_NEW) {
    counter_start.attach( & countDownFlag, 120); //countdown of 2 minutes starts.
    setLEDstate(led_bits = 0x0300);
    sounderLed = true; //turn on led1
    State_NEW = false;
  }

  if (Time_out) //sounder unit deactivates after 2 minutes
  {
    sounderLed = false;
  }

  if (Password_right) //if input pssword is correct, go to report state
  {
    PRESENT_STATE = REPORT;
    StateVariablesInitilize();
    Lcdset();
  }
}


void State_Report() {
  if (State_NEW) {
    LCDStateCheck(); //error message is displayed on LCD
    setLEDstate(led_bits = 0x0C00);
    State_NEW = false;
  }
  if (Input_clear) //if user enter "C", go to unset state
  {
    PRESENT_STATE = UNSET;
    StateVariablesInitilize();
    Lcdset();
  }
}

// all variables and intrrups reset when state changes
void StateVariablesInitilize() {
  Attempts_lost = false;
  Password_right = false;
  setLEDstate(led_bits = 0x0000);
  Time_out = false;
  sounderLed = false;
  State_NEW = true;
  counter_start.detach(); // timout is detached
  turnLEDon.detach(); //ticker is detached 
  Input_clear = false;
}

// poll timer interrups and activate the flag
void countDownFlag() {
  Time_out = true;
}





void setLEDstate(int led_bits) {
  //these leds are combination of state and zones
  led_bits = led_bits | (High_entryExitZone * 0xC000 +
    High_fullSetZone * 0x3000);
  spi.write(led_bits); // the desire output led
  cs = 1; //cs is set to 1 to send output
  cs = 0; //cs is set to 0 to reset
}

// ir flashes sounder unit led
void onLED() {
  sounderLed = !sounderLed;
}

/*here we read rows and columns so see if voltage chabges and send respective key from table*/
char getKey() {
  int i, j;
  char ch = ' ';
  for (i = 0; i <= 3; i++) {
    rows = i;
    for (j = 0; j <= 3; j++) {
      if (((cols ^ 0x00FF) & (0x0001 << j)) != 0) {
        ch = Keytable[(i * 4) + j]; // the row and column which is found is decoded into keys
      }
    }
  }
  return ch;
}

//check if the password given by the user is equal to the stored password
void checkpssword() {
  if (!strcmp(password, AvailablePassword)) //if same
  {
    Attempts_Password = 0;
    Attempts_lost = false;
    Password_right = true;
    Lcdset(); //information to send to lcd
    Inputclear(); 
    Position_digit = 0;
  } else {
    Attempts_Password = Attempts_Password++;
    if (Attempts_Password >= 3) //if code is entered wrong 3 times
    {
      Attempts_lost = true;
      Password_right = false;
      Attempts_Password = 0;
    }
    Inputclear(); //reset digits
  }
}

void Inputclear() {
  for (unsigned char i = 0; i < 4; i++) //the input password entered by user is reset
  {
    AvailablePassword[i] = '_';
    Password_Seen[i] = '_';
  }
  Position_digit = 0;
  Lcdset();
}


void Inspect_debounce() {
  char newChar = ' ';
  newChar = getKey(); //read key again
  if ((newChar == Starting_char) && (newChar != ' ')) //if key input is  same or different
  {
    finalize_debounce = newChar; //confirm that really an input
    debounce_initilize = false; //it helps to read more inputs
  } else 
  {
    finalize_debounce = ' ';
    debounce_initilize = false;
  }
}




int read_switch() {
  rows = 5; //read first four leds
  int switchSetOne = cols;
  rows = 4; //read last four leds
  int switchSetTwo = cols * 16; //put in exact binary order
  return (switchSetOne + switchSetTwo); //send back  combination
}
// leds are initialise with desire clock rate and format
void initleds() {
  cs = 0;
  spi.format(16, 0); // SPI is set wit 16 bit data, low state, high going clock
  spi.frequency(1000000); // clock rate is set to 1Mz
}

unsigned char newInput(unsigned char newChar) {
  if ((newChar == 'C') && (Position_digit <= 4) && (PRESENT_STATE != REPORT)) //if backspace needed or not.
  {
    if (Position_digit > 0) Position_digit = Position_digit - 1; // the postion of password digit is move backword
    AvailablePassword[Position_digit] = '_';
    Password_Seen[Position_digit] = '_';
    Lcdset(); //reload LCD
  } else if ((Position_digit == 4) && (Starting_char == 'B')) //requested to check code
  {
    checkpssword(); //check password for match
    Position_digit = 0;
  } else if (!isdigit(newChar) && newChar != 'C' && PRESENT_STATE != REPORT) // stop if any other input is given
  {
    Inputclear();
    Position_digit = 0;
    Lcdset(); //reload LCD
  } else if (isdigit(newChar) && (Position_digit < 4) && (PRESENT_STATE != REPORT)) //mask character is added to password
  {
    AvailablePassword[Position_digit] = newChar;
    Password_Seen[Position_digit] = '*';
    Position_digit = Position_digit + 1;
    Lcdset(); //reload LCD
  } else if (PRESENT_STATE == REPORT && newChar == 'C') //input of C changes state to unset if current state is report
  {
    Input_clear = true;
    Position_digit = 0;
  }
  return Position_digit;
}
void KeypadInput() {
  Starting_char = getKey(); //read character

  if ((Starting_char != ' ') && (!debounce_initilize)) {
    debounce_initilize = true; //no input readings until debounce stops
  } else if (debounce_initilize) //first input reading is used debounce
  {
    Inspect_debounce();
    debounce_initilize = false;
  }

  if (finalize_debounce != ' ' && Starting_char != ' ') { // if the character is different
    newInput(Starting_char); 
    finalize_debounce = ' '; //reset input
  }
}




//error cause is shown and C to clear
void LCDStateCheck() {
  lcd.cls();
  lcd.locate(0, 0);
  lcd.printf("%s%s", Message_Print[3], Namesof_Errors[Cause_ERROR]); //the type of Error is shown:
  lcd.locate(0, 1);
  lcd.printf("%s", Message_Print[4]); //show press C to clear
}

//see which switch is on to see the zones.
void Switches_Read() {
  int zoneStates = read_switch();

  if ((zoneStates & 0x80) == 0x80) //switch 8  means entry/exit zone activated
  {
    High_entryExitZone = true;
    setLEDstate(led_bits);
  } else {
    High_entryExitZone = false;
    setLEDstate(led_bits);
  }

  if ((zoneStates & 0x40) == 0x40) //switch 7 means full set zone activated
  {
    High_fullSetZone = true;
    setLEDstate(led_bits);
  } else {
    High_fullSetZone = false;
    setLEDstate(led_bits);
  }
}

//reset lcd 
void Lcdset() {
  lcd.cls();
  lcd.locate(0, 0);
  lcd.printf("%s%s", Message_Print[2], Namesof_States[PRESENT_STATE]); // the lcd show the STATE:
  if (Position_digit == 4) //B option is shown when 4 digits given
  {
    lcd.locate(0, 1);
    lcd.printf("%s", Message_Print[1]); //message to enter B
  } else if (AvailablePassword[0] != '_') //password entered so far is displayed
  {
    lcd.locate(0, 1);
    lcd.printf("%s%s", Message_Print[0], Password_Seen); 
  }
}





int main() {
  initleds(); 
  Lcdset(); // clear lcd screen   
  PRESENT_STATE = UNSET; // the first or present state of alarm system.
  Inspect_Keypad.attach( & KeypadInput, 0.1); //poll keypad every 100ms to check for any input
  Zones_Monitor.attach( & Switches_Read, 0.1); //poll zones every 100ms to inspect any change in zones.
   while (1) {
    switch (PRESENT_STATE) //finite state machine structure
    {
    case UNSET:
      State_Unset();
      break;

    case EXIT:
      State_Exit();
      break;

    case SET:
      State_SET();
      break;

    case ENTRY:
      State_Entry();
      break;

    case ALARM:
      State_Alarm();
      break;

    case REPORT:
      State_Report();
      break;
    }
  }
}
