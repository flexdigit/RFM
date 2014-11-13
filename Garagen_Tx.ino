//-------------------------------------------------------------------------
// Garagensender is NANO on /dev/ttyUSB0/1
// Some ideas coming from Nathan Chantrell. See http://nathan.chantrell.net
//-------------------------------------------------------------------------

#include <JeeLib.h> // https://github.com/jcw/jeelib

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // interrupt handler for JeeLabs Sleepy power saving

#define myNodeID  1              // RF12 node ID in the range 1-30
#define freq      RF12_868MHZ    // 868 MHz
#define network   212            // RF12 Network group

// for UNO purpose          // ATtiny84 Pin (phys.)
#define SWITCH_DOOR    7    // 10
#define SWITCH_GATE    8    // 11

// for UNO purpose
//#define SWITCH_DOOR    3
//#define SWITCH_GATE    4

typedef struct {
    boolean door;
    boolean gate;
} garage;

garage garage_data;

//-------------------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    //Serial.println("Init NANO...");
    //Serial.println("Starting garage module.");

    rf12_initialize(myNodeID, freq, network);  // Initialize RFM12 with settings defined above
    rf12_sleep(0);                             // put RF module to sleep

    pinMode(SWITCH_DOOR, INPUT);
    pinMode(SWITCH_GATE, INPUT);
    
    ADCSRA &= ~ bit(ADEN); bitSet(PRR, PRADC); // Disable the ADC to save power
                                               // from Nathan...
}

void loop(){

    garage_data.door = digitalRead(SWITCH_DOOR);    // read door switch
    garage_data.gate = digitalRead(SWITCH_GATE);    // read gate switch
    //Serial.println(garage_data.door);
    //Serial.println(garage_data.gate);
    
    rfwrite();                                      // send data via RFM

    Sleepy::loseSomeTime(1000); //JeeLabs power save function: enter low power mode for 1 seconds (valid range 16-65000 ms)
                                // from Nathan...
}

//------------------------------------------------------------
// Send payload data via RF
//------------------------------------------------------------
static void rfwrite(){
    rf12_sleep(-1);        // wake up RF module
    //delay(200);
    while (!rf12_canSend())
        rf12_recvDone();
    rf12_sendStart(0, &garage_data, sizeof garage_data);
    rf12_sendWait(2);
    //Serial.print("door:");
    //Serial.print(garage_data.door);
    //Serial.print(" gate: ");
    //Serial.println(garage_data.gate);
    //delay(200);
    rf12_sleep(0);        // put RF module to sleep
}
