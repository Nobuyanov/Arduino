


int trigPin = 10; 
int echoPin = 11;  
 
void setup() { 
  Serial.begin (115200); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}

 void noteOn(int cmd, unsigned long pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
 
void loop() { 
  int duration, distance;
  // для большей точности установим значение LOW на пине Trig
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  // Теперь установим высокий уровень на пине Trig
  digitalWrite(trigPin, HIGH);
  // Подождем 10 μs 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  // Узнаем длительность высокого сигнала на пине Echo
  duration = pulseIn(echoPin, HIGH); 
  // Рассчитаем расстояние
  distance = duration / 58;
  // Выведем значение в Serial Monitor
  if (distance < 50) {
  Serial.print(distance); 
  Serial.println(" cm"); 
  noteOn(0x90, (7), 0x7F);
  delay (50);
  }
  
}
