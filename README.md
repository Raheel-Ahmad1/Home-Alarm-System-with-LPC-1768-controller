# Home-Alarm-System-with-LPC-1768-controller
A home alarm system is constructed from various separate parts, which all connect to  a central control unit. This task  implements a basic control unit and the  functionality behind its user interface.

# Introduction to alarm systems
A home alarm system is constructed from various separate parts, which all connect to 
a central control unit. This project implement a basic control unit and the 
functionality behind its user interface. The complete specification of overall system are:-
A conventional alarm system normally contains:
1 Central control unit
2 Keypad entry point / user interface
3 Sounder unit
4 Sensors such as: magnetic contacts (reed switches), pressure mats, movement 
sensors like passive infrared sensors (PIRs).
In the case of the sensors, they all contain switches that are “closed” circuit under 
normal circumstances. This type of switch is known as a Normally Closed (NC) 
switch, the converse is known as Normally Open (NO).
Magnetic contacts are used to detect when doors and windows are opened. When a 
door is opened a magnet is moved away from the reed switch which causes the switch 
contacts to open. Likewise when a movement sensor is triggered or a pressure mat 
depressed the circuit is broken (the switch is opened).
The opening of a contact can be detected by using one of the microcontroller's parallel 
port inputs, connected to the top of the switch whose lower end is connected to GND. 
The top of the switch is also connected through a resistor to 3.3v (V+).
When the switch is closed the port input is connected to 0v (GND), and it will be 
pulled-up to 3.3v (V+) by the resistor when the sensor's switch opens. Therefore the 
microcontroller input is logic low (0v) when a door is closed / there is no movement, 
and logic high (3.3v) when the sensor is activated.
The alarm controller must detect these events and respond as specified later in this 
document. In this project the sensors are emulated using normally closed push to 
break switches.
Normal home alarm systems include multiple sensors located in various locations, 
each location can have its own circuit which allows identification of the triggered 
switch, i.e. it lets the user know where the break-in occurred. Each sensor circuit is 
known as a zone, and zones can have different behaviour
#  Functional Specification - Zone Behaviour

In this Project there are two types of zone: one Entry / Exit zone, and full set 
zones. Each zone has a corresponding LED to indicate its state.
This alarm has SIX states of operation set, unset, entry, exit, alarm, and report.
Initially it is in the unset state.

1 In the unset state, activation of any of the sensors should not cause the 
sounder unit to sound.
Entry of the correct four-digit code in the user interface (described later) 
followed by “B” should cause the system to change to the exit state.
If the user enters an invalid code three times, the alarm should change to the
alarm state.
Whilst in the unset state, the unset LED should be on.

2. When in the exit state, the user has a time interval called the exit period 
(1 minutes) in which to vacate their home. In the exit state, activation of any 
sensors in a full set zone should cause the alarm to enter the alarm state. 
Whilst in the exit state the sounder unit should sound for approximately 
250ms every 250ms.
If the user enters their four digit code followed by “B” within the period, the 
alarm should change to the unset state.
If an invalid code is entered three times, the alarm should change to the alarm
state.
If all the zones are inactive when the exit period (1 minutes) expires, the alarm 
should enter the set state.
Whilst in the exit state, the exit LED should be on.

3. In the set state, activation of any sensors in a full set zone should cause the 
alarm to enter the alarm state.
Activation of the entry / exit zone should change the state to entry state. 
Whilst in the set state, the set LED should be on.

4. The purpose of the entry state is to allow the user a period of time to gain 
access to their home so that they can unset the alarm, this duration is known as 
the entry period (2 minutes).
Whilst in the entry state, the sounder unit should sound for approximately 
250ms every 250ms, i.e. switch on, then off at 250ms intervals.
If the user enters their four-digit code followed by “B” within the period, the 
alarm should change to the unset state.
If the user fails to enter their correct code within the entry period, the alarm 
should change to the alarm state.
In the entry state, activation of any sensors in a full set zone should cause the 
alarm to enter the alarm state.
Whilst in the entry state, the entry LED should be on.

5. When in the alarm state, the sounder unit should be enabled all the time. The 
alarm LED should be switched on.
After 2 minutes, the sounder unit should be disabled.
If the user enters the correct code followed by “B”, the alarm should change to 
the report state, otherwise stay in the alarm state.

6. When in the report state, the LCD should show the zone numbers or code 
error information in the first line. In the second line it should show “C key to 
clear”. When an “C” is entered, the alarm should change to the unsetstate.

#  Functional Specification - User Interface

The user interface consists of:

1. a 4x4 keypad;

2. a 16 character by 2 line LCD display;

3. 6 LEDs for 6 states; (external led 8, 7, 6, 5, 4, 3 represent unset, exits, set, 
entry, alarm, report, respectively)

4. 2 LEDs for 2 zones; (external led 1 for entry/exit zone, external led 2 for full 
set zone)

5. a sounder unit (replaced with LED1);

6. 2 sensors for 2 zones (replaced with 2 switches, switch 1 for entry/exit zone, 
switch 2 for full set zone )


