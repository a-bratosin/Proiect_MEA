#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define MAX_SEATS 5
LiquidCrystal_I2C lcd(0x27,16,2); //adresa LCD la 0x27 pt 16 char si 2 lini

#define MAX_OPEN_DURATION 5000
#define UPDATE_INTERVAL 20

Servo entrance_servo, exit_servo;
const int entrance_servo_pin = 10;
const int exit_servo_pin = 11;

const int entrance_timer = MAX_OPEN_DURATION/UPDATE_INTERVAL;
const int exit_timer = MAX_OPEN_DURATION/UPDATE_INTERVAL;


const int entrance_sensor = 2;
const int exit_sensor = 3;

const int led_entrance = 4; 
const int led_exit = 13; 

int remaining_seats = MAX_SEATS; //nr total de locuri
int val_entrance = 0;
int val_exit = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(exit_sensor, INPUT);
  pinMode(entrance_sensor,INPUT);
  pinMode(13, OUTPUT);
  lcd.init(); // initializare lcd 
  lcd.backlight();  //open backlight
  lcd.setCursor(5,0);
  lcd.print("PARCARE");
  lcd.setCursor(4,1);
  lcd.print("AUTOMATA");
  

  entrance_servo.attach(entrance_servo_pin);
  exit_servo.attach(exit_servo_pin);
  entrance_servo.write(0);
  exit_servo.write(0);


  delay(1000);
  attachInterrupt(digitalPinToInterrupt(entrance_sensor), enters, FALLING);
  attachInterrupt(digitalPinToInterrupt(exit_sensor), exits, FALLING);

}
void enters(){

  noInterrupts();
  entrance_servo.write(90);
  interrupts();
  
  //delay(2000);

  //Serial.println("returning entrance servo");
  //noInterrupts();
  //entrance_servo.write(0);
  //interrupts();
  
  
}

void exits(){

    noInterrupts();
    exit_servo.write(90);
    interrupts();

    
    //delay(2000);
    
    //Serial.println("returning exit servo");
    
    //noInterrupts();
    //exit_servo.write(0);
    //interrupts();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  lcd.setCursor(5,0);
  lcd.print("PARCARE");
  lcd.setCursor(4,1);
  lcd.print("AUTOMATA");
  Serial.println("running...");
  exit_servo.write(0);
  entrance_servo.write(0);
}
