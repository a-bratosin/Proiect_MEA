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

const int led_entrance = 4; 
const int led_exit = 13; 

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
  exit_servo.write(0);


  delay(1000);
  attachInterrupt(digitalPinToInterrupt(entrance_sensor), enters, FALLING);
  attachInterrupt(digitalPinToInterrupt(exit_sensor), exits, FALLING);

  lcd.clear();
  lcd.print("Locuri ramase:");
  lcd.setCursor(0, 1);
  lcd.print(remaining_seats);
}
void enters(){

  // acționează motorul și decrementează/incrementează doar dacă deja este coborâtă bariera
  if(entrance_timer == 0){
    noInterrupts();
    entrance_servo.write(90);
    interrupts();

    //resetez timer-ul
  
    remaining_seats--;
    changed_val = 1;
  }
  //Serial.println("returning entrance servo");
  //noInterrupts();
  //entrance_servo.write(0);
  //interrupts();
  entrance_timer = MAX_OPEN_DURATION/UPDATE_INTERVAL;
  
}

void exits(){
  
  if(entrance_timer == 0){
    noInterrupts();
    exit_servo.write(90);
    interrupts();

    //delay(2000);
    remaining_seats++;
    //Serial.println("returning exit servo");
    changed_val = 1;
  }
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
  // put your main code here, to run repeatedly:
  /*
  delay(2000);
  lcd.setCursor(5,0);
  lcd.print("PARCARE");
  lcd.setCursor(4,1);
  lcd.print("AUTOMATA");
  Serial.println("running...");
  exit_servo.write(0);
  entrance_servo.write(0);
  */
  if(changed_val){    
    clear_bottom_row();
    lcd.setCursor(0, 1);
    if(remaining_seats == 0){
      lcd.print("Parcare plina!");
    }else if(remaining_seats == MAX_SEATS){
      lcd.print("Parcare goala!");
    }else
      lcd.print(remaining_seats);
    
    changed_val = 0;
  }

  // cei doi senzori au semnalele invers: când detectează un obiect, au 0, și în rest 1
  if(entrance_timer == 0){
    entrance_servo.write(0);
  }else if((entrance_timer > 0) && digitalRead(entrance_sensor) ){
    entrance_timer--;
  }

  
  if(exit_timer == 0){
    exit_servo.write(0);
  }else if((exit_timer > 0)  && digitalRead(exit_sensor) ){
    exit_timer--;
  }
  delay(UPDATE_INTERVAL);
}
