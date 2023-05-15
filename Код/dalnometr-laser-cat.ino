#include <NewPing.h>

#define PIN_TRIG 12
#define PIN_ECHO 11

#define MAX_DISTANCE 200 // Константа для определения максимального расстояния, которое мы будем считать корректным.

// Создаем объект, методами которого будем затем пользоваться для получения расстояния.
// В качестве параметров передаем номера пинов, к которым подключены выходы ECHO и TRIG датчика

NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

void setup() {
  // Инициализируем взаимодействие по последовательному порту на скорости 9600
  Serial.begin(9600);
  pinMode (5, OUTPUT); // подключаем к пину светодиод
}

void loop() {
  unsigned int distance2=0;
  for (int i = 0;; i += 1) {
  // Стартовая задержка, необходимая для корректной работы.
  delay(50);

  // Получаем значение от датчика расстояния и сохраняем его в переменную
  unsigned int distance1 = sonar.ping_cm();
  if (distance1!=distance2) {digitalWrite(5,1);}
  distance2 = sonar.ping_cm();

  // Печатаем расстояние в мониторе порта
  Serial.print(distance1);
  Serial.println("см");
  }


}