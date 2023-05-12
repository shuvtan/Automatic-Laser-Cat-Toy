/*
  Laser Tower for the CAT - LA FABRIQUE DIY
  Pseudo-randomly moves a servo tower (on X and Y axis) and lights up a laser.
  x_servo is attached to pin 6 and moves in the X plan 
  y_servo is attached to pin 9 and moves in the Y plan 
  Laser is on pin 13

  HOW IT WORKS : 
  The program randomly choose a new position for the laser inside a square you can define below. 
  It checks the new position is different from the old one of at least "minimal_movement".
  It moves the tower to the new position and stays still for a time between min_freeze and max_freeze 
  (this aims to reproduce the behaviour of an insect landing somewhere for a bit and then flying off, 
  that's the variable you need to increase if your cat is fat).
  Ans starts the process over and over again. 
  
  Created 30 Sep 2016 by Lucas Berbesson
*/


#include <Servo.h>

/* YOU CAN CUSTOM THESE VARIABLES IF YOU WANT TO ALTER THE TOWER BEHAVIOUR */

// X servo angle will stay in [min_x, max_x] range
// Y servo angle will stay in [min_y, max_y] range
// to be ajsuted to the size of your living room

float min_x = 40;
float max_x = 140;
float min_y = 40;
float max_y = 70;
int min_freeze = 800;
int max_freeze = 4000;
float minimal_movement = 5;
int testmode = 0; // uncomment for testing
/* YOU SHOULD NOT HAVE TO MODIFY THE CODE BELOW THIS LINE */

// finding center of square for starting point
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

// Instantiating two servos
Servo x_servo;  
Servo y_servo;
int pos = 0;

void setup() {
  Serial.begin(9600);   
  y_servo.attach(9);  // attaches the y servo on pin 9 to the servo object
  x_servo.attach(7);  // attaches the x servo on pin 7 to the servo object
  pinMode (5, OUTPUT);
  digitalWrite (5, HIGH);  // switch on  the laser
  
  //Place the servos in the center at the beginning 
  y_servo.write(y_position); 
  x_servo.write(x_position);     

  if (testmode > 0) {
    for(int i=10; i<(180); i++) {
      x_servo.write(i);
      Serial.print("X=");
      Serial.println(i);      
      delay(100);
    }
    x_servo.write(x_position);         
    for(int i=10; i<(180); i++) {
      y_servo.write(i);
      Serial.print("Y=");
      Serial.println(i);      
      delay(100);
    }
  } 
}

void loop() {
  
  movement_time = random(20,50);
  random_delay = random(min_freeze, max_freeze);
  x_new_position = random(min_x+minimal_movement, max_x-minimal_movement);
  y_new_position = random(min_y+minimal_movement, max_y-minimal_movement);
  
  if( (y_new_position > y_old_position) && (abs(y_new_position - y_old_position) < 5 )) {
    y_new_position = y_new_position + minimal_movement;
  }  else if ( (y_new_position < y_old_position) && (abs(y_new_position - y_old_position) < 5 )) {
    y_new_position = y_new_position - minimal_movement;
  }
  
  if( (x_new_position > x_old_position) && (abs(x_new_position - x_old_position) < 5 )) {
    x_new_position = x_new_position + minimal_movement;
  }  else if ( (x_new_position < x_old_position) && (abs(x_new_position - x_old_position) < 5 )) {
    x_new_position = x_new_position - minimal_movement;
  }
  
  x_speed = (x_new_position - x_old_position)/movement_time;
  y_speed = (y_new_position - y_old_position)/movement_time;  
  
  for (pos = 0; pos < movement_time; pos += 1) { 
      x_position = x_position + x_speed;
      y_position = y_position + y_speed;
      x_servo.write(x_position);  
      y_servo.write(y_position);                    
    delay(10); 
  }
  x_old_position = x_new_position;
  y_old_position = y_new_position;
  delay(random_delay);
  

}

