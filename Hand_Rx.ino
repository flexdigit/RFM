//-------------------------------------------------------------------------
// Handsender ist der UNO on /dev/ttyACM0/1
// Some ideas coming from Nathan Chantrell. See http://nathan.chantrell.net
//-------------------------------------------------------------------------

#include <JeeLib.h> // https://github.com/jcw/jeelib

#define myNodeID  1              // RF12 node ID in the range 1-30
#define freq      RF12_868MHZ    // 868 MHz
#define network   212            // RF12 Network group

// for ATtiny84 purpose              // ATtiny84 Pin (phys.):
#define LED_GREEN_DOOR    7        // 10
#define LED_RED_DOOR      8        // 11
#define LED_GREEN_GATE    9        // 12
#define LED_RED_GATE      10       // 13

// for UNO purpose
//#define LED_GREEN_DOOR    3
//#define LED_RED_DOOR      4
//#define LED_GREEN_GATE    5
//#define LED_RED_GATE      6

typedef struct {
    boolean door;
    boolean gate;
} garage;

garage garage_data;

//-------------------------------------------------------------------------

void setup() {
    Serial.begin(9600);
    //Serial.println("Init UNO...");
    //Serial.println("Starting hand module.");
    
    rf12_initialize(myNodeID, freq, network); // Initialize RFM12 with settings defined above 
    //rf12_sleep(0);                            // put RF module to sleep
    
    // set the digital pins
    pinMode(LED_RED_DOOR,   OUTPUT);
    pinMode(LED_GREEN_DOOR, OUTPUT);
    pinMode(LED_RED_GATE,   OUTPUT);
    pinMode(LED_GREEN_GATE, OUTPUT);
    
    // to show that all LEDs are working a short flashing
    digitalWrite(LED_RED_DOOR,   HIGH);
    digitalWrite(LED_GREEN_DOOR, HIGH);
    digitalWrite(LED_RED_GATE,   HIGH);
    digitalWrite(LED_GREEN_GATE, HIGH);
    delay(400);
    digitalWrite(LED_RED_DOOR,   LOW);
    digitalWrite(LED_GREEN_DOOR, LOW);
    digitalWrite(LED_RED_GATE,   LOW);
    digitalWrite(LED_GREEN_GATE, LOW);
    //delay(400);
}

void loop() {
    //Serial.print(garage_data.door);
    //Serial.print(":");
    //Serial.println(garage_data.gate);
    //delay(200);

    // receive data?
    if (rf12_recvDone() && rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0)
    {
        //Serial.println("get data...");
        garage_data = *(garage*) rf12_data;
        //delay(500);
        
        // check the switches
        // Door //////////////////////////////
        if(garage_data.door == HIGH){            // HIGH == 1 == closed
            digitalWrite(LED_GREEN_DOOR, HIGH);
            digitalWrite(LED_RED_DOOR,   LOW);
        }
        else                                     // LOW  == 0 == open
        {
            digitalWrite(LED_GREEN_DOOR, LOW);
            digitalWrite(LED_RED_DOOR,   HIGH);
        }
        // Gate //////////////////////////////
        if(garage_data.gate == HIGH){
            digitalWrite(LED_GREEN_GATE, HIGH);
            digitalWrite(LED_RED_GATE,   LOW);
        }
        else
        {
            digitalWrite(LED_GREEN_GATE, LOW);
            digitalWrite(LED_RED_GATE,   HIGH);
        }
    }

}


