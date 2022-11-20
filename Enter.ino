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

#define Set 9
#define Key1 10
#define Key2 11
#define Key3 12
#define Key4 13


#define Inter_IR 0

#define motor 2

int front = 0 ;

int intro_key = 0;


int car_array[6] = {0,0,0,0,0,0};

int Priority[6] = {0,0,0,0,0,0};


int EmptySlots = 0;
int counter;

static struct { int floorVal; int slotVal; } tuple[6];

static void editTuple(int fv, int sv , int index) {
    tuple[index].floorVal = fv;
    tuple[index].slotVal = sv;
}

int currentstate = 1;
int initialcounter = 0;


void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
pinMode(IR_1_1 , INPUT);
pinMode(IR_1_2 , INPUT);
pinMode(IR_2_1 , INPUT);
pinMode(IR_2_2 , INPUT);
pinMode(IR_3_1 , INPUT);
pinMode(IR_3_2 , INPUT);
pinMode(Set , INPUT);
pinMode(Key1 , INPUT);
pinMode(Key2 , INPUT);
pinMode(Key3 , INPUT);
pinMode(Key4 , INPUT);

pinMode(motor , OUTPUT);

// set up the LCD's number of columns and rows:
lcd.begin(20 , 2);  
lcd.setCursor (0,0);
lcd.print("   Welcome to ");
lcd.setCursor (0,1);
lcd.print("   Parking :) ");
delay(30);
lcd.clear(); 
}



void loop() {
    if(digitalRead(Inter_IR) == HIGH){
      if(currentstate==1 && initialcounter==0){
        initialize1();
        initialcounter = 1;
      }
      else if(currentstate==2 && initialcounter==0){
        initialize2();
        initialcounter = 1;
      }
      else if(currentstate==3 && initialcounter==0){
        initialize3(); 
        initialcounter = 1; 
    }    
      set_priority();
      park_status(); 
      calculate_capacity();
      intery_information(front,EmptySlots);
      move_motor(front,EmptySlots);
    }
    if(digitalRead(Inter_IR) == LOW){
      lcd.clear(); 
      digitalWrite(motor,LOW);
      if(digitalRead(Set)==LOW){
         change_algorithm();
    }
    }
}

void change_algorithm(){

  while(true){
  lcd.setCursor (0,0);
  lcd.print("1.DownUp2.UpDown");
  lcd.setCursor (0,1);
  lcd.print("3.Random4.Manual"); 
  delay(30);
  if(digitalRead(Set)==LOW){
    lcd.clear();
    delay(10);
    break; 
  }
  else if(digitalRead(Key1)==LOW){
    currentstate = 1;
    initialcounter = 0;
    delay(10);
    break;
    }
  else if(digitalRead(Key2)==LOW){
    currentstate = 2;
    initialcounter = 0;
    delay(10);
    break;
    }
  else if(digitalRead(Key3)==LOW){
    currentstate = 3;
    initialcounter = 0;
    delay(10);
    break;
    }
  else if(digitalRead(Key4)==LOW){
    currentstate = 4;
    initialcounter = 0;
    int outerloop = 0;
    int keyloop = 0;
    for(int i = 0; i<6 ; i++){
      outerloop = 0;
      keyloop = 0;
      lcd.clear();
      while(outerloop==0){
      //lcd.begin(20 , 2);
      lcd.setCursor (0,0);
      lcd.print("   Priority ");
      lcd.print(i+1);
      lcd.setCursor (0,1);
      lcd.print("Floor ? Slot ?");
      if(digitalRead(Key1)==LOW){
        tuple[i].floorVal = 1;
        while(keyloop==0){
        lcd.setCursor (0,0);
        lcd.print("   Priority ");
        lcd.print(i+1);
        lcd.setCursor (0,1);
        lcd.print("Floor 1 Slot ?");
        if(digitalRead(Key1)==LOW){
          tuple[i].slotVal = 1;
          lcd.setCursor (0,0);
          lcd.print("   Priority ");
          lcd.print(i+1);
          lcd.setCursor (0,1);
          lcd.print("Floor 1 Slot 1");
          delay(20);
          keyloop = 1;
          outerloop = 1;
          }
         else if(digitalRead(Key2)==LOW){
          tuple[i].slotVal = 2;
          lcd.setCursor (0,0);
          lcd.print("   Priority ");
          lcd.print(i+1);
          lcd.setCursor (0,1);
          lcd.print("Floor 1 Slot 2");
          delay(20);
          keyloop = 1;
          outerloop = 1;
          }       
        }
      } 
      else if(digitalRead(Key2)==LOW){
        tuple[i].floorVal = 2;
        while(keyloop==0){
        lcd.setCursor (0,0);
        lcd.print("   Priority ");
        lcd.print(i+1);
        lcd.setCursor (0,1);
        lcd.print("Floor 2 Slot ?");
        if(digitalRead(Key1)==LOW){
          tuple[i].slotVal = 1;
          lcd.setCursor (0,0);
          lcd.print("   Priority ");
          lcd.print(i+1);
          lcd.setCursor (0,1);
          lcd.print("Floor 2 Slot 1");
          delay(20);
          keyloop = 1;
          outerloop = 1;
          }
         else if(digitalRead(Key2)==LOW){
          tuple[i].slotVal = 2;
          lcd.setCursor (0,0);
          lcd.print("   Priority ");
          lcd.print(i+1);
          lcd.setCursor (0,1);
          lcd.print("Floor 2 Slot 2");
          delay(20);
          keyloop = 1;
          outerloop = 1;
          }       
        }
      } 
      else if(digitalRead(Key3)==LOW){
        tuple[i].floorVal = 3;
        while(keyloop==0){
        lcd.setCursor (0,0);
        lcd.print("   Priority ");
        lcd.print(i+1);
        lcd.setCursor (0,1);
        lcd.print("Floor 3 Slot ?");
        if(digitalRead(Key1)==LOW){
          tuple[i].slotVal = 1;
          lcd.setCursor (0,0);
          lcd.print("   Priority ");
          lcd.print(i+1);
          lcd.setCursor (0,1);
          lcd.print("Floor 3 Slot 1");
          delay(20);
          keyloop = 1;
          outerloop = 1;
          }
         else if(digitalRead(Key2)==LOW){
          tuple[i].slotVal = 2;
          lcd.setCursor (0,0);
          lcd.print("   Priority ");
          lcd.print(i+1);
          lcd.setCursor (0,1);
          lcd.print("Floor 3 Slot 2");
          delay(20);
          keyloop = 1;
          outerloop = 1;
          }       
        }
       } 
      }
    }
   }
 }
}

void initialize1(){
  editTuple(1,1,0);
  editTuple(1,2,1);
  editTuple(2,1,2);
  editTuple(2,2,3);
  editTuple(3,1,4);
  editTuple(3,2,5); 
}


void initialize2(){
  editTuple(3,2,0);
  editTuple(3,1,1);
  editTuple(2,2,2);
  editTuple(2,1,3);
  editTuple(1,2,4);
  editTuple(1,1,5); 
}

void initialize3(){
  for(int i=0 ; i<6 ; i++){
    int index = i;
    int f = (rand()%3)+1;
    int s= (rand()%2)+1;
    editTuple(f,s,i); 
    for(int j =0 ; j<index ; j++){
      if(tuple[j].floorVal==f && tuple[j].slotVal==s){
        i--;
        }
      }
    }
}


void set_priority(){
  for(int i = 0 ; i<6 ; i++){
    if(tuple[i].floorVal == 1 && tuple[i].slotVal == 1){
      if(digitalRead(IR_1_1)==LOW)
        Priority[i] = 0;
      else
        Priority[i] = 1;
    }
    else if(tuple[i].floorVal == 1 && tuple[i].slotVal == 2){
      if(digitalRead(IR_1_2)==LOW)
        Priority[i] = 0;
      else
        Priority[i] = 1;
    }
    else if(tuple[i].floorVal == 2 && tuple[i].slotVal == 1){
      if(digitalRead(IR_2_1)==LOW)
        Priority[i] = 0;
      else
        Priority[i] = 1;
    }
    else if(tuple[i].floorVal == 2 && tuple[i].slotVal == 2){
      if(digitalRead(IR_2_2)==LOW)
        Priority[i] = 0;
      else
        Priority[i] = 1;
    }
    else if(tuple[i].floorVal == 3 && tuple[i].slotVal == 1){
      if(digitalRead(IR_3_1)==LOW)
        Priority[i] = 0;
      else
        Priority[i] = 1;
    }
    else if(tuple[i].floorVal == 3 && tuple[i].slotVal == 2){
      if(digitalRead(IR_3_2)==LOW)
        Priority[i] = 0;
      else
        Priority[i] = 1;
    }
  } 
}
void park_status(){
 if(Priority[0]==0){
  front = 0;
  car_array[0] = 0;
  if(Priority[1]==0)
    car_array[1] = 0;
  else
    car_array[1] = 1;
  if(Priority[2]==0)
    car_array[2] = 0;
  else
    car_array[2] = 1;
  if(Priority[3]==0)
    car_array[3] = 0;
  else
    car_array[3] = 1;
  if(Priority[4]==0)
    car_array[4] = 0;
  else
    car_array[4] = 1;
  if(Priority[5]==0)
    car_array[5] = 0;
  else
    car_array[5] = 1;
  }
 else{
  car_array[0] = 1;
  if(Priority[1]==0){
    front=1;
    car_array[1]=0;
    if(Priority[2]==0)
      car_array[2] = 0;
    else
      car_array[2] = 1;
    if(Priority[3]==0)
      car_array[3] = 0;
    else
      car_array[3] = 1;
    if(Priority[4]==0)
      car_array[4] = 0;
    else
      car_array[4] = 1;
    if(Priority[5]==0)
      car_array[5] = 0;
    else
      car_array[5] = 1;
    }
  else{
    car_array[1]=1;
    if(Priority[2]==0){
      front=2;
      car_array[2]=0;
      if(Priority[3]==0)
        car_array[3] = 0;
      else
        car_array[3] = 1;
      if(Priority[4]==0)
        car_array[4] = 0;
      else
        car_array[4] = 1;
      if(Priority[5]==0)
        car_array[5] = 0;
      else
        car_array[5] = 1;
      }
    else{
      car_array[2]=1;
      if(Priority[3]==0){
        front=3;
        car_array[3]=0;
        if(Priority[4]==0)
          car_array[4] = 0;
        else
          car_array[4] = 1;
        if(Priority[5]==0)
          car_array[5] = 0;
        else
          car_array[5] = 1;
        }
      else{
        car_array[3]=1;
        if(Priority[4]==0){
          front=4;
          car_array[4]=0;
           if(Priority[5]==0)
            car_array[5] = 0;
          else
            car_array[5] = 1;
          }
        else{
          car_array[4]=1;
          if(Priority[5]==0){
            front=5;
            car_array[5]=0;
            }
          else{
            car_array[5]=1;
            }
          }
        }
      }
    }
  }
}
void calculate_capacity(){
  counter = 0 ;
  for(int i = 0 ; i<6 ; i++){
    if(car_array[i] == 0)
      counter ++;
    }
  EmptySlots = counter ;
}
void intery_information(int f , int e){
if(e==0){
  lcd.begin(20 , 2);  
  lcd.setCursor (0,0);
  lcd.print("Capacity is Full");
  lcd.setCursor (0,1);
  lcd.print("    Sorry :(");  
  }
else{
  lcd.begin(20 , 2);  
  lcd.setCursor (0,0);
  lcd.print("Capacity is : ");
  lcd.print(e);
  lcd.setCursor (0,1);
  lcd.print("Floor ");
  if(f==0){
    lcd.print(tuple[0].floorVal);
    lcd.print("   Slot ");
    lcd.print(tuple[0].slotVal);
    }
  else if(f==1){
    lcd.print(tuple[1].floorVal);
    lcd.print("   Slot ");
    lcd.print(tuple[1].slotVal);
    }
  else if(f==2){
    lcd.print(tuple[2].floorVal);
    lcd.print("   Slot ");
    lcd.print(tuple[2].slotVal);
    }
  else if(f==3){
    lcd.print(tuple[3].floorVal);
    lcd.print("   Slot ");
    lcd.print(tuple[3].slotVal);
    }
  else if(f==4){
    lcd.print(tuple[4].floorVal);
    lcd.print("   Slot ");
    lcd.print(tuple[4].slotVal);
    }
  else if(f==5){
    lcd.print(tuple[5].floorVal);
    lcd.print("   Slot ");
    lcd.print(tuple[5].slotVal);
    }
  delay(10);
  }
}
void move_motor(int f , int e){
  if(e==0){
    digitalWrite(motor,LOW);
    } 
  else{
    digitalWrite(motor,HIGH);
    delay(10);
    }
}
  
  
