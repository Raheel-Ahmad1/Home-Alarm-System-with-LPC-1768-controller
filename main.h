
//Description: Home alarm System

#ifndef main_h
#define main_h

    #include <mbed.h>
    #include <TextLCD.h>
    #include <ctype.h>     
  
    extern TextLCD lcd(p15, p16, p17, p18, p19, p20);  
    extern SPI spi(p5, p6, p7);   
    extern DigitalOut cs(p8);     
    extern DigitalOut sounderLed(LED1);
    extern BusOut rows(p26,p25,p24);   
    extern BusIn cols(p14,p13,p12,p11);  
    extern Serial pc(USBTX, USBRX);   
    unsigned char   takeInput            (unsigned char newChar);   
    
     //keypad button array
    extern char Keytable[] = { 'F', 'E', 'D', 'C',    
                           '3', '6', '9', 'B',
                           '2', '5', '8', '0',
                           '1', '4', '7', 'A' };
    int             read_switch         ();    
    extern const char password[5] = {'1','2','3','4', '\0'};   //password is set by user
    extern char AvailablePassword[5] = {'_','_','_','_', '\0'};    // password that is entered by user
    extern char Password_Seen[5] = {'_','_','_','_', '\0'};    // password that is visible to the user
    extern volatile char Starting_char = ' ';
    extern volatile char finalize_debounce = ' ';
    
    extern unsigned char Position_digit = 0;
    extern int led_bits = 0;         
    extern bool Password_right = false;  
    extern bool High_fullSetZone = false;
     extern bool Attempts_lost = false;
    extern bool High_entryExitZone = false;
    extern bool State_NEW = false;
    extern unsigned char Attempts_Password = 0;
    extern bool Input_clear = false;
    extern bool debounce_initilize = false;
    extern bool Time_out = false;
    //all states of FSM
    extern char const* Namesof_States[] = {"UNSET", "EXIT", "SET", 
                                       "ALARM", "ENTRY", "REPORT"};
    //types of errors to display
    extern char const* Namesof_Errors[] = {"ENTRYEXIT", "FULLSET", 
                                       "CODEWRONG", "NOCODE"};
    
    // message that is shown on the lcd to the user
    extern char const* Message_Print[] = {"Password:", "Enter B to Set", 
                                               "STATE: ", "Error: ", 
                                               "Enter C to clear"};

   void            LCDStateCheck      ();
    void            initleds            ();      
    void            KeypadInput          ();
    void            onLED            ();
    void            Lcdset          ();  
    void            Inputclear   ();
    void            Switches_Read       (); 
    void            State_Exit           ();
    void            Inspect_debounce       ();
    void            checkpssword           ();
    void            setLEDstate           (int led_bits);
    void            State_Unset          ();
    void            StateVariablesInitilize ();
    void            State_SET            ();
    void            State_Alarm          ();
    void            State_Entry          ();
    void            countDownFlag       ();
    void            State_Report         ();

    Timeout counter_start;
    Ticker Inspect_Keypad;
    Ticker turnLEDon;   
    Ticker Zones_Monitor;
                                     
   typedef enum //enumric types for FSM States
    {
        UNSET,       
        EXIT,        
        SET,         
        ALARM,       
        ENTRY,       
        REPORT,      
    }stateType;
    
    typedef enum
    {
        ZONE_ENTRYEXIT,  //activation of entry/exit zone 
        ZONE_FULLSET,    //activation of full set zone 
        WRONG_password,    //3 times wrong password is entered
        NO_PASSWORD        //in timeframe no password is entered
    }errorType;
    
    errorType Cause_ERROR;   //present error
    stateType PRESENT_STATE;    //present state
    
    
                    
#endif
