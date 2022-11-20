#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define IR_1_1 3
#define IR_1_2 4

#define IR_2_1 5
#define IR_2_2 6

#define IR_3_1 7
#define IR_3_2 8



#define Exit_IR 0

#define motor 2

#define ATM 9


#define out 10

#define LED 12

int pointer = 0;

float cost = 0.0;
unsigned long duration;

int threshold = 250; 

unsigned long IR11_Start;
unsigned long IR11_Finish;

unsigned long IR12_Start;
unsigned long IR12_Finish;


unsigned long IR21_Start;
unsigned long IR21_Finish;

unsigned long IR22_Start;
unsigned long IR22_Finish;

unsigned long IR31_Start;
unsigned long IR31_Finish;

unsigned long IR32_Start;
unsigned long IR32_Finish;



int present11 = 0 ;
int present12 = 0 ;
int present21 = 0 ;
int present22 = 0 ;
int present31 = 0 ;
int present32 = 0 ;

int ppm;

int f ;
int s ;

int index = 0;

static struct { int floorVal; int slotVal; int durationVal; int costVal; } tuple[6];

static void addTuple(int fv, int sv ,  int dv , int cv) {
    tuple[index].floorVal = fv;
    tuple[index].slotVal = sv;
    tuple[index].durationVal = dv;
    tuple[index].costVal = cv;
}

static void deleteTuple(int fv , int sv){
  int deleteindex ;
  for(int i = 0 ; i<6 ; i++){
    if(tuple[i].floorVal == fv && tuple[i].slotVal == sv){
      deleteindex = i;
      printf("%i\n",deleteindex);
      break;
    }
  }
  int switchindex = deleteindex;
  while(switchindex<6 && tuple[switchindex+1].floorVal != 0){
    tuple[switchindex].floorVal = tuple[switchindex+1].floorVal;
    tuple[switchindex].slotVal = tuple[switchindex+1].slotVal;
    tuple[switchindex].durationVal = tuple[switchindex+1].durationVal;
    tuple[switchindex].costVal = tuple[switchindex+1].costVal;
    switchindex++;
    }
  index = switchindex;
  for(int i = switchindex ; i < 6 ; i++){
      tuple[i].floorVal = 0;
      tuple[i].slotVal = 0;
      tuple[i].durationVal = 0;
      tuple[i].costVal = 0;
    }
  }



void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
pinMode(IR_1_1 , INPUT);
pinMode(IR_1_2 , INPUT);
pinMode(IR_2_1 , INPUT);
pinMode(IR_2_2 , INPUT);
pinMode(IR_3_1 , INPUT);
pinMode(IR_3_2 , INPUT);

pinMode(motor , OUTPUT);


pinMode(out , INPUT);


pinMode(LED , OUTPUT);
// set up the LCD's number of columns and rows:
lcd.begin(20 , 1);  
lcd.setCursor (0,0);
lcd.print("Ready to go ...");
delay(30);
}

void loop() {
  pollution_status();
  if(digitalRead(Exit_IR) == HIGH){
    park_status(); 
    exit_information();
    move_motor();
  }
  if(digitalRead(Exit_IR) == LOW){
    park_status();
    lcd.clear(); 
    delay(20);
    digitalWrite(motor,LOW);
  }
}

void park_status(){
  //Avlin bari mashini vared jaygah mishavad
  if(digitalRead(IR_1_1) == HIGH and present11 == 0){
    IR11_Start = millis();
    present11 = 1;
    }
  if(digitalRead(IR_1_2) == HIGH and present12 == 0){
    IR12_Start = millis();
    present12 = 1;
    }
  if(digitalRead(IR_2_1) == HIGH and present21 == 0){
    IR21_Start = millis();
    present21 = 1;
    }
  if(digitalRead(IR_2_2) == HIGH and present22 == 0){
    IR22_Start = millis();
    present22 = 1;
    }
  if(digitalRead(IR_3_1) == HIGH and present31 == 0){
    IR31_Start = millis();
    present31 = 1;
    }
  if(digitalRead(IR_3_2) == HIGH and present32 == 0){
    IR32_Start = millis();
    present32 = 1;
    }

  //khoroj mashin
  if(digitalRead(IR_1_1) == LOW and present11 == 1){
    IR11_Finish = millis();
    present11 = 0;
    addTuple(1,1,calculateTime(IR11_Start , IR11_Finish),calculateCost(calculateTime(IR11_Start , IR11_Finish))); 
    f = 1;
    s = 1;
    }
  else if(digitalRead(IR_1_2) == LOW and present12 == 1){
    IR12_Finish = millis();
    present12 = 0;
    addTuple(1,2,calculateTime(IR12_Start , IR12_Finish),calculateCost(calculateTime(IR12_Start , IR12_Finish)));     
    f = 1;
    s = 2; 
    }
  else if(digitalRead(IR_2_1) == LOW and present21 == 1){
    IR21_Finish = millis();
    present21 = 0;
    addTuple(2,1,calculateTime(IR21_Start , IR21_Finish),calculateCost(calculateTime(IR21_Start , IR21_Finish))); 
    f = 2;
    s = 1;    
    }
  else if(digitalRead(IR_2_2) == LOW and present22 == 1){
    IR22_Finish = millis();
    present22 = 0;
    addTuple(2,2,calculateTime(IR22_Start , IR22_Finish),calculateCost(calculateTime(IR22_Start , IR22_Finish))); 
    f = 2;
    s =2;     
    }
  else if(digitalRead(IR_3_1) == LOW and present31 == 1){
    IR31_Finish = millis();
    present31 = 0;
    addTuple(3,1,calculateTime(IR31_Start , IR31_Finish),calculateCost(calculateTime(IR31_Start , IR31_Finish)));  
    f = 3;
    s = 1;    
    }
  else if(digitalRead(IR_3_2) == LOW and present32 == 1){
    IR32_Finish = millis();
    present32 = 0;
    addTuple(3,2,calculateTime(IR32_Start , IR32_Finish),calculateCost(calculateTime(IR32_Start , IR32_Finish))); 
    f = 3;
    s = 2;     
    }     
}
void exit_information(){
  if(tuple[0].floorVal==0){ 
    lcd.begin(20 , 2); 
    lcd.setCursor (0,0);
    lcd.print(" No One intends");
    lcd.setCursor (0,2);
    lcd.print("     to go");
   }
   else{
    lcd.setCursor (0,0);
    lcd.print("D:");
    lcd.print(tuple[0].durationVal);
    lcd.print("  f:");
    lcd.print(tuple[0].floorVal);
    lcd.print(" s:");
    lcd.print(tuple[0].slotVal);
    lcd.setCursor (0,1);
    lcd.print("Cost is : ");
    lcd.print(tuple[0].costVal);
   }
  delay(40);
}

void move_motor(){
   if(tuple[0].floorVal==0){
   } 
   else{
     lcd.clear();
    while(digitalRead(ATM) == LOW){
      lcd.setCursor (0,0);
      lcd.print(" Please Pay for");
      lcd.setCursor (0,1);
      lcd.print("    Parking");
      }
      lcd.clear();
      deleteTuple(tuple[0].floorVal,tuple[0].slotVal);
      lcd.setCursor (0,0);
      lcd.print("Have a Nice Day");
      digitalWrite(motor,HIGH);
      delay(30);
 }
}
unsigned long calculateTime(unsigned long base , unsigned long bound){
  return(bound - base) ;
  }
float calculateCost(unsigned long d){
  return(d*15.0);
  }

void pollution_status(){
  if (digitalRead(out)==HIGH)            // check is ppm is greater than threshold or not
    {   
      tone(LED,1000,1000);         //blink led with turn on time 1000mS, turn off time 200mS 
    }
  else
    {
      digitalWrite(LED,LOW);   //jump here if ppm is not greater than threshold and turn off LED
    }  
  delay (30);
}








  
