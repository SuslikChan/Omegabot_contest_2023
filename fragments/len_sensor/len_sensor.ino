#define SENS_PIN A0 


float len_sensor(){ // Чтение дистанции с ИК дальномера

  float measur  = analogRead(SENS_PIN);     // Чтение значения с АЦП
  float voltage = measur * 5 / 1023;        // Преобразование АЦП в реальный вольтаж
  float dist    = 65 * pow(voltage, -1.10); // Преобразование волтажа в мм

  return dist;
}


void setup() {
  
  pinMode(SENS_PIN, INPUT);
  Serial.begin(9600);

}

void loop(){

  Serial.println(len_sensor());
  delay(500);
}