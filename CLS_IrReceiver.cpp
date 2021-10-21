

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include "CLS_IrReceiver.h"

//#define DEBUG_IR_RECEIVER_LOW_LEVEL
#define DEBUG_IR_RECEIVER_HIGH_LEVEL

uint16_t RECV_PIN = 14; // for ESP8266 micrcontroller D4=GPIO_2
IRrecv irrecv(RECV_PIN);
decode_results results;

CLS_IrReceiver::CLS_IrReceiver()
{
}

void CLS_IrReceiver::init()
{
    irrecv.enableIRIn();
}

void CLS_IrReceiver::debug()
{
#ifdef DEBUG_IR_RECEIVER_LOW_LEVEL
    Serial.print(" - address: ");
    Serial.print(results.address);
    Serial.print(" - bits: ");
    Serial.print(results.bits);
    Serial.print(" - command: ");
    Serial.print(results.command);
    Serial.print(" - decode_type: ");
    Serial.print(results.decode_type);
    Serial.print(" - overflow: ");
    Serial.print(results.overflow);
    //Serial.print(" - rawbuf: ");
    //Serial.print(results.rawbuf);
    Serial.print(" - rawlen: ");
    Serial.print(results.rawlen);
    Serial.print(" - repeat: ");
    Serial.print(results.repeat);
    //Serial.print(" - state: ");
    //Serial.print(results.state);
    Serial.print(" - Value: ");
    Serial.println((uint32_t)results.value & 0xFFFF, HEX); // print the second part of the message
    Serial.println("");
#endif
}

uint32_t CLS_IrReceiver::getIrRequest()
{
    uint32_t value = (uint32_t)ENUM_IR_CMD_REQUEST::IR_NONE;
    if (irrecv.decode(&results))
    {
        if (results.bits > 0)
        {
            /*
            Serial.print("decode_type: ");
            Serial.println(results.decode_type);
*/
            switch (results.decode_type)
            {
                /*
            case decode_type_t::UNKNOWN:

                Serial.println("decode_type: UNKNOWN");
                irrecv.resume(); // Receive the next value
                break;
            case decode_type_t::UNUSED:
                Serial.println("decode_type: UNUSED");
                irrecv.resume(); // Receive the next value
                break;
*/
            case decode_type_t::RC6:
                if (results.bits == 20)
                {
#ifdef DEBUG_IR_RECEIVER_LOW_LEVEL
                    Serial.println("decode_type: RC6 - ");
                    debug();
#endif
                    value = results.value & 0xFFFF;
#ifdef DEBUG_IR_RECEIVER_HIGH_LEVEL
                    Serial.print("decode_type: RC6 - ");
                    Serial.print(value, HEX);
#endif
                }
                irrecv.resume(); // Receive the next value
                break;
                /*
            case decode_type_t::NEC:
                Serial.println("decode_type: NEC - ");
                debug();
                irrecv.resume(); // Receive the next value
                break;
*/
            default:
                //Serial.println("decode_type: others - ");
                //debug();
                irrecv.resume(); // Receive the next value
                break;
            }
        }
        else
            irrecv.resume(); // Receive the next value
    }
    return value;
}
