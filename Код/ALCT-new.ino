
/*
Laser Tower for the CAT - LA FABRIQUE DIY
Pseudo-randomly moves a servo tower (on X and Y axis) and lights up a laser.
x_servo is attached to pin 7 and moves in the X plan
y_servo is attached to pin 9 and moves in the Y plan
Laser is on pin 5

HOW IT WORKS :
The program randomly choose a new position for the laser inside a square you can define below.
It checks the new position is different from the old one of at least "minimal_movement".
It moves the tower to the new position and stays still for a time between min_freeze and max_freeze
(this aims to reproduce the behaviour of an insect landing somewhere for a bit and then flying off,
that's the variable you need to increase if your cat is fat).
Ans starts the process over and over again.

Created 30 Sep 2016 by Lucas Berbesson
*/

//Подключаем нужные библиотеки
#include <Servo.h>
#include <NewPing.h>

 //Подключаем датчик дальности
#define PIN_TRIG 12 //пускает сигнал
#define PIN_ECHO 11 //принимает сигнал

#define MAX_DISTANCE 200 // Константа для определения максимального расстояния, которое мы будем считать корректным.

// Создаем объект, методами которого будем затем пользоваться для получения расстояния.
// В качестве параметров передаем номера пинов, к которым подключены выходы ECHO и TRIG датчика

NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

/*Можем менять начальные параметры, задания игровой площади */

// X - угол в горизонтальной плоскости  [min_x, max_x] - сервопривод пин 7
// Y - угол в вертикальной плоскости [min_y, max_y] - сервопривод пин 9

float min_x = 40;
float max_x = 140;
float min_y = 70;
float max_y = 120;
int min_freeze = 800;
int max_freeze = 4000;
float minimal_movement = 5;
int testmode = 0; // uncomment for testing
/* YOU SHOULD NOT HAVE TO MODIFY THE CODE BELOW THIS LINE */

// Находим центр игровой площади
int random_delay;
float x_position = min_x + (max_x - min_x)/2;
float y_position = min_y + (max_y - min_y)/2;
float x_old_position = x_position;
float y_old_position = y_position;
float x_new_position;
float y_new_position;
float x_speed;
float y_speed;
int movement_time;

// Подключаем сервоприводы
Servo x_servo;
Servo y_servo;
int pos = 0;

void setup() {
Serial.begin(9600);
y_servo.attach(9); // Сервопривод вертикальной плоскости Y - пин 9
x_servo.attach(7); // Сервопривод вертикальной плоскости X - пин 7
pinMode (5, OUTPUT);
digitalWrite (5, HIGH); // Включаем лазер

//Центровка - начальное состояние прибора
y_servo.write(y_position);
x_servo.write(x_position);

if (testmode > 0) {
for(int i=10; i<(10); i++) {
x_servo.write(i);
Serial.print("X=");
Serial.println(i);
delay(100);
}
x_servo.write(x_position);
for(int i=10; i<(10); i++) {
y_servo.write(i);
Serial.print("Y=");
Serial.println(i);
delay(100);
}
}
}

void loop() {

unsigned int distance2=0;
unsigned int time1=0;
unsigned int time2=0;
unsigned int time3=0;
for (int i = 0;; i += 1) {
  time1 = millis();
  delay(50); //Стартовая задержка для корректной работы

  // Получаем значение от датчика расстояния и сохраняем его в переменную
  unsigned int distance1 = sonar.ping_cm();

 //Если котик поймал лазер, т.е. расстояние до объекта изменилось -> меняем положение
  if (abs(distance1-distance2)>=8) {
   delay(30);
   basic();
   time2 = millis();
  }
 
 if (abs(time1-time2)>60000){
 delay(30);
 basic();
 time2 = millis();
 }
}
}

void basic(){
   //Выбираем параметры для рандомного поворота
    movement_time = random(20,50);
    x_new_position = random(min_x+minimal_movement, max_x-minimal_movement);
    y_new_position = random(min_y+minimal_movement, max_y-minimal_movement);

    if( (y_new_position > y_old_position) && (abs(y_new_position - y_old_position) < 5 )) {
    y_new_position = y_new_position + minimal_movement;
    } else if ( (y_new_position < y_old_position) && (abs(y_new_position - y_old_position) < 5 )) {
    y_new_position = y_new_position - minimal_movement;
    }

    if( (x_new_position > x_old_position) && (abs(x_new_position - x_old_position) < 5 )) {
    x_new_position = x_new_position + minimal_movement;
    } else if ( (x_new_position < x_old_position) && (abs(x_new_position - x_old_position) < 5 )) {
    x_new_position = x_new_position - minimal_movement;
    }

    x_speed = (x_new_position - x_old_position)/movement_time;
    y_speed = (y_new_position - y_old_position)/movement_time;
    
    //Пересчитываем координаты при повороте
    for (pos = 0; pos < movement_time; pos += 1) {
    x_position = x_position + x_speed;
    y_position = y_position + y_speed;
    x_servo.write(x_position);
    y_servo.write(y_position);
    delay(50);
    }
    x_old_position = x_new_position;
    y_old_position = y_new_position;
    //Запоминаем новое расстояние до объекта, пока котик не поймал лазер
    unsigned int distance2 = sonar.ping_cm();
}