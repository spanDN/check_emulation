//**************************************************************//
//  Name    : Check control emulation BMW E32/E34               //
//  Author  : Pancheko Sergii                                   //
//  Date    : 3 May, 2016                                       //
//  Version : 1.0                                               //
//  Notes   :                                                   //
//          :                                                   //
//****************************************************************


byte data[7] = {0b10111111, 0b01111100, 0b00001100, 0b00000001, 0b11111111, 0b11111111, 0b11111111};
enum t_state {WAIT, WAIT_LATCH, WAIT_CLOCK, CLOCK_LOW, SET_OUT};
#define PIN_CLOCK 3
#define PIN_LATCH 4
#define PIN_OUTPUT 5
#define BIT_TO_TRANSFER 56
enum t_state state = WAIT;
int index = -1;
boolean CurrentBit(int index);

void setup() 
{
  pinMode(PIN_OUTPUT, OUTPUT);
  pinMode(PIN_CLOCK, INPUT_PULLUP);
  pinMode(PIN_LATCH, INPUT_PULLUP);
}

void loop() {
  switch(state) {
      case WAIT:  
                  if(digitalRead(PIN_LATCH) == HIGH) {
                    state = WAIT_LATCH;
                  }
                  break;
      case WAIT_LATCH:
                  if(digitalRead(PIN_LATCH) == LOW) {
                    state = WAIT_CLOCK;
                    index = -1;
                  }
                  break;
      case WAIT_CLOCK:
                  if(digitalRead(PIN_CLOCK) == LOW) {
                    state = CLOCK_LOW;
                  }
                  break;
       case CLOCK_LOW:
                  if(digitalRead(PIN_CLOCK) == HIGH) {
                    state = SET_OUT;
                    if(index == -1){
                      index = 0;
                    } 
                    else {
                      if(index < BIT_TO_TRANSFER - 1) {
                        index++;
                      }
                      else {
                         index = -1;
                         state = WAIT;
                      }
                    }
                  }
                  break;
       case SET_OUT:
                  if(CurrentBit(index) == true) {
                    digitalWrite(PIN_OUTPUT, HIGH); 
                  } else {
                    digitalWrite(PIN_OUTPUT, LOW); 
                  }
                  state = WAIT_CLOCK;
                  break;          
      default:
              state = WAIT;
    }
    if(digitalRead(PIN_LATCH) == HIGH) {
      state = WAIT_LATCH;
      digitalWrite(PIN_OUTPUT, LOW);
      index = -1;
    }
}
/*--------------------------------------------------------------*/
 boolean CurrentBit(int index)
 {
   int byteToTransfer = index/8;
   int bitToTransfer = 7 - (index % 8);
   boolean bRet = false;
   
  if(  bitRead(data[byteToTransfer], bitToTransfer) == 1)
    bRet = true;
  else 
    bRet =  false;
    
  return bRet;
 }
 /*--------------------------------------------------------------*/

