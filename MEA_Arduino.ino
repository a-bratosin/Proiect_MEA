#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define MAX_SEATS 5
LiquidCrystal_I2C lcd(0x27,16,2); //adresa LCD la 0x27 pt 16 char si 2 lini

#define MAX_OPEN_DURATION 5000
#define UPDATE_INTERVAL 100

Servo entrance_servo, exit_servo;
const int entrance_servo_pin = 10;
const int exit_servo_pin = 11;

int entrance_timer = 0;
int exit_timer = 0;


const int entrance_sensor = 2;
const int exit_sensor = 3;

//const int led_entrance = 4;
//const int led_exit = 5;

int remaining_seats = MAX_SEATS; //nr total de locuri
int val_entrance = 0;
int val_exit = 0;

int changed_val = 0;

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
  exit_servo.write(60);

  //pinMode(led_entrance, OUTPUT);
  //pinMode(led_exit, OUTPUT);
  //digitalWrite(led_entrance, LOW);
  //digitalWrite(led_exit, LOW);

  delay(1000);
  attachInterrupt(digitalPinToInterrupt(entrance_sensor), enters, FALLING);
  attachInterrupt(digitalPinToInterrupt(exit_sensor), exits, FALLING);

  lcd.clear();
  lcd.print("Locuri ramase:");
  lcd.setCursor(0, 1);
  lcd.print(remaining_seats);
}
void enters()
{
  // acționează motorul și decrementează/incrementează doar dacă deja este coborâtă bariera

  if(entrance_timer == 0 && remaining_seats != 0)
  {
    //digitalWrite(led_entrance, HIGH);
    noInterrupts();
    entrance_servo.write(55);
    interrupts();

    //delay(2000);
    if(remaining_seats > 0)
    {
      remaining_seats--;
    }
    changed_val = 1;
  }
  //resetez timer-ul
  entrance_timer = MAX_OPEN_DURATION/UPDATE_INTERVAL;
}

void exits(){
  
  if(exit_timer == 0)
  {
    //digitalWrite(led_exit, HIGH);
    noInterrupts();
    exit_servo.write(0);
    interrupts();

    //delay(2000);
    if(remaining_seats < 5)
    {
      remaining_seats++;
    }
    changed_val = 1;
  }
  //resetez timer-ul
  exit_timer = MAX_OPEN_DURATION/UPDATE_INTERVAL;
}

void clear_bottom_row(){
  lcd.setCursor(0,1);
  for(int i=0; i<16; i++){
    lcd.print(' ');
  }
}
void loop() {

  Serial.print("entrance timer: ");
  Serial.print(entrance_timer);
  Serial.print(" | exit timer: ");
  Serial.println(exit_timer);
  
  if(changed_val)
  {    
    clear_bottom_row();
    lcd.setCursor(0, 1);
    if(remaining_seats == 0){
      lcd.print("Parcare plina!");
    }
    else if(remaining_seats == MAX_SEATS){
      lcd.print("Parcare goala!");
    }
    else{
      lcd.print(remaining_seats);
    }
    changed_val = 0;
  }

  // cei doi senzori au semnalele invers: când detectează un obiect, au 0, și în rest 1
  if(entrance_timer == 0)
  {
    //Serial.println("returning entrance servo");
    entrance_servo.write(0);
    //digitalWrite(led_entrance, LOW);
  }
  else if((entrance_timer > 0) && digitalRead(entrance_sensor) ){
    entrance_timer--;
  }

  
  if(exit_timer == 0)
  {
    //Serial.println("returning exit servo");
    exit_servo.write(60);
    //digitalWrite(led_exit, LOW);
  }
  else if((exit_timer > 0)  && digitalRead(exit_sensor) ){
    exit_timer--;
  }
  delay(UPDATE_INTERVAL);
}