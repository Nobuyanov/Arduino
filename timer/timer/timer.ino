
#include <Wire.h>


#include <RCSwitch.h>
#include <iarduino_RTC.h>                             
#include <LiquidCrystal_I2C.h> 
#include <MsTimer2.h>

int value;
char c  = ' ';
unsigned long start, finished, elapsed;
boolean runTimer = false;
boolean runTimerFinished = false;
boolean runTimerStart = false;

// Переменные для времени отладки кнопки
long lastButtonPressTime = 0; // последний раз, когда была нажата кнопка
long pauseButtonPressTime = 0; // последний раз, когда была нажата кнопка
long debounceDelay = 250; // время отладки; установите как можно меньше




RCSwitch mySwitchRx = RCSwitch();
LiquidCrystal_I2C lcd(0x3F,16,2);  
//LiquidCrystal_I2C lcd(0xA7,16,2);  
iarduino_RTC time(RTC_DS1302,8,6,7);         // Задаем правильно название нашего модуля, а также указываем к каким цифровым пинам его подключаем(в нашем случае – 8,6,7)

void setup() {
  mySwitchRx.enableReceive(0);  
  pinMode (5, INPUT); //Кнопка старт-стоп
  pinMode (4, INPUT); //Кнопка сброс
  pinMode(9, OUTPUT); //Динамик
  lcd.init();                      // Инициализация дисплея  
  lcd.backlight();                 // Подключение подсветки
  lcd.setCursor(4,0);              // Установка курсора в начало первой строки
  lcd.print(" READY");
  Serial.begin(9600);                              // устанавливаем скорость передачи данных с модулей в 9600 бод
  time.begin();                                       // функция служит для инициализации работы модуля
  time.settime(10,10,10,4,10,17,3);  // 10  сек, 10 мин, 10 час, 4 , октября, 2017 года, среда 
} 
  
void loop(){

  CheckReset();
  CheckStartStop();
  DisplayResult();
  //reciveSerialData(); 
  reciveRadioData(); 

}

void reciveSerialData(){

  
  
 if(Serial.available()){
   //Serial.println(mySwitchRx.getReceivedValue());
  char c = Serial.read();

    if (c == '1'){   
      Serial.println(c);  
      lcd.setCursor(4,0);              
      lcd.print("FINISH: ");
      digitalWrite(9, HIGH); 
      delay(500); 
      digitalWrite(9, LOW); 
      
    
       
      pauseButtonPressTime = millis();
      lastButtonPressTime = millis();
    }
   
  mySwitchRx.resetAvailable();
  }

}

void reciveRadioData(){

  
  
 if(mySwitchRx.available()){
   //Serial.println(mySwitchRx.getReceivedValue());
   int value = mySwitchRx.getReceivedValue();

    if (value == B0100){     
      lcd.setCursor(4,0);              
      lcd.print("FINISH: ");
      digitalWrite(9, HIGH); 
      delay(500); 
      digitalWrite(9, LOW); 
    runTimer = true; 
    delay(1500); 
      runTimer = false;  
      pauseButtonPressTime = millis();
      lastButtonPressTime = millis();
    }
   
   if( value == 00100 ){
   
     // tone(9, 2150, 500);    
     digitalWrite(9, HIGH); 
      delay(500); 
      digitalWrite(9, LOW); 
      
      runTimer = false;
      runTimerFinished = true;
      lcd.setCursor(4,0);              
      lcd.print("FINISHED");
   }
  mySwitchRx.resetAvailable();
  }

}

void CheckReset(){
  int resetButton = digitalRead (4);
  if ((millis() - lastButtonPressTime) > debounceDelay){
    if (resetButton == HIGH){     
      lcd.clear();
      lcd.setCursor(4,0);              
      lcd.print(" READY");
      runTimer = false; 
      runTimerStart = false; 
      runTimerFinished = false;
      pauseButtonPressTime = 0;
      lastButtonPressTime = 0;
      start = millis();
    }
  }
}

void CheckStartStop(){
  char c = Serial.read();
  int startStopButton = digitalRead (5);
  if ((millis() - lastButtonPressTime) > debounceDelay){
    if (startStopButton == HIGH && runTimer == true){     
      lcd.setCursor(4,0);              
      lcd.print(" PAUSE    ");
      runTimer = false;  
      pauseButtonPressTime = millis();
      lastButtonPressTime = millis();
    }
  if (c == '1'){     
      lcd.setCursor(4,0);              
      lcd.print(" FINISH    ");
      digitalWrite(9, HIGH); 
      delay(500); 
      digitalWrite(9, LOW); 
      c = '0';
      runTimer = true; 
      DisplayResult();
      runTimer = false; 
 } 

 

  }
  if ((millis() - lastButtonPressTime) > debounceDelay){
    if (startStopButton == HIGH && runTimer == false && runTimerFinished == false){
    lcd.clear();
    lcd.setCursor(4,0);              
    lcd.print("RUNNING");
    if (runTimerStart == false){
        start = millis();
      digitalWrite(9, HIGH); 
      delay(500); 
      digitalWrite(9, LOW);  
      // tone(9, 2150, 500); 
        runTimerStart = true;
    }
    lastButtonPressTime = millis();
    runTimer = true;
    }
  }
}


void DisplayResult(){
  
  if (runTimer == true){
    finished = millis(); // сохраняет время остановки, чтобы вычислить прошедшее время.
    // объявляем переменные
    float h, m, s, ms;
    unsigned long over;

    // время
    elapsed = finished - start;



    h = int(elapsed / 3600000);    
    over = elapsed % 3600000;
    
  //  m = int(over / 60000);
  //  over = over % 60000;
    s = int(over / 1000);
    ms = over % 1000;
    
    
    lcd.setCursor(0, 1); // показать результаты
//    lcd.print(m, 0);
//    lcd.print("m ");
//    lcd.print(s, 0);
//    lcd.print("s ");
//    if (h < 10){
//      lcd.print(ms, 0);
//      lcd.print("ms ");
//    }
    
    lcd.print(s, 0);
    lcd.print(".");
    if (h < 10 && ms < 10.00){
      lcd.print("00");
      lcd.print(ms, 0);
    }else if (h < 10 && ms < 100.00){
      lcd.print("0");
      lcd.print(ms, 0);
    }else {
      lcd.print(ms, 0);
    }
    lcd.print("s");
    //Serial.println (ms);
  }
  
}
