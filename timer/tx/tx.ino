#include <RCSwitch.h>

RCSwitch mySwitchTx = RCSwitch();
int value;

void setup() {
  mySwitchTx.enableTransmit(2);
  
  pinMode (5, OUTPUT);
  Serial.begin(9600);                          
}   
void loop(){
//Serial.println(analogRead (1));
  value = analogRead (1) ; // читаем состояния датчика
  if (value > 1000){
    Serial.print(1);
    mySwitchTx.send(B0100 , 4);
    digitalWrite(5, HIGH);
    delay(3000); 
    digitalWrite(5, LOW); 

   
    
    
  }

  
 
}
