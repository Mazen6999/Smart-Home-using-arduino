int Incoming_value = 0; // bluetooth
float T = 0; // bluetooth
long unsigned int t_old=0; // bluetooth
////////////////////////////////////////////////
// keypad
#include <LiquidCrystal.h>     
#include <Keypad.h> 
#include <Servo.h>

Servo DOOR ;
int pos = 0;    // variable to store the servo position
int doorflag=0;
 LiquidCrystal lcd(24,25,26,27,28,29);

const byte ROWS = 4; //four rows    // TAKE CARE KEYPAD HENA 4*3 W E7NA GAYBEEN 4*4
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {53, 52, 51, 50};      //pins connected to Keypad 
byte colPins[COLS] = {49, 48, 13}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );   //initilize Keypad
char key ;     //variable to get the input from user 


int order = 0 ;   //variable to change the screen on LCD (Display or set T or set H or set Pos)
bool flag = 0 ;
int i=0 ; 
bool check=0 ; 

int Test_array[10] ;  //array to store each char from the user 
int Test=0 ;       // convert array to a number

int saved_password[4]= {1,2,3,4}; 
int current_password[10] ;
int new_password[10] ;
 ////////////////// IR
int sensor1=22;
int flagT=0;
int enter=0;
int leave=0;
int count=0;
//////////////////PIR
#define LED 40
#define alarm 38
#define pirsen 7
bool pir_Reading=0 ;   // initialize the library with the numbers of the interface pins
///////////TEMP
 const int tempPin=A0;
int tempOutput=38;  
float temp;
///////////////// Garage door
Servo Garage;
Servo Road;
int const trigPin = 9;
int const echoPin = 8;
int const buzzPin = 38;
int doorstatus=0;
long duration;
int distance;
///////////////////waterlevel
int const trigPin2 = 6;
int const echoPin2= 5;
long duration2;
int distance2;



void setup() 
{  // bluetooth
  Serial.begin(9600);   // Serial monitor is used to see receive data, send data,print data and so on      
  pinMode(13, OUTPUT);   
  pinMode(9, OUTPUT);
  //////////keypad
  lcd.begin(16,2);      // initiaize lcd 
  DOOR.attach(12);
  DOOR.write(70);
  ////////////////IR
  digitalWrite(sensor1, HIGH);
  
  
 ///////////// PIR
  pinMode(LED, OUTPUT);
  pinMode(alarm, OUTPUT);
  ////////TEMP
  pinMode(tempOutput,OUTPUT);
////////////// Garage door
    pinMode(trigPin, OUTPUT); // trig pin will have pulses output
    pinMode(echoPin, INPUT); // echo pin should be input to get pulse width
    pinMode(buzzPin, OUTPUT); // buzz pin is output to control buzzering
Garage.attach(11);
Road.attach(2);
Garage.write(20);
Road.write(30);
////////////////waterlevel
  pinMode(trigPin2, OUTPUT); // trig pin will have pulses output
    pinMode(echoPin2, INPUT); // echo pin should be input to get pulse width
    pinMode(buzzPin, OUTPUT); // buzz pin is output to control buzzering


}

void loop()
{
 //bluetooth();
 keypad1();
 PIR();
 TEMP();
 Garagedoor();
 waterlevel();
}                         

////////////////


 
/////////////////////
void keypad1(){

 key = keypad.getKey();  // function to get a key from the keypad 
  
  Get_Order () ;      //function to get an order from the user (Display or set T or set H or set Pos)

  if ( order == 0 ) {             // if order=0   display date on LCD 
    Enter_Password();     //function to display data               
  }
     
  else if ( order == 1 ) {       //if order = 1    set Temperature
    Change_Password();         //function to set T
  }
  if(digitalRead(sensor1)==0 &&  doorflag==1){ // if door is opened
   lcd.clear();
   //IR();
    lcd.print("PEOPLE AT DOOR!!");
   delay(1000);
   lcd.clear();
 }
  
}
void Get_Order () {
if ( key == '*' && flag == 0 ){       //each time user press * the order number increases to change screen 
    order = order + 1 ;                                        //  Display or set T or set H or set Pos)
    flag = 1;
    lcd.clear();
    i=0;
  }
  else if ( key != '*' ){
    flag = 0 ; 
  }

  if ( order == 2 ) {
    order = 0 ;
  }
}

 void Enter_Password(){

    if(i==0) {
     lcd.setCursor(0,0);
     lcd.print("Enter Password : " );
     lcd.setCursor(0,1);
 
     lcd.print("- - - -" ); // print on lcd - - - -
        lcd.setCursor(0,1);
        
    }
    
    if(key=='0'||key=='1'||key=='2'||key=='3' 
    ||key=='4'||key=='5'||key=='6' 
    ||key=='7'||key=='8'||key=='9') {
    lcd.setCursor(2*i,1); // ex if you enter 1 (on lcd) -> 1 - - - (i stands for column number kaza) 2*i 3shan yenot el space el fel nos
    lcd.print(key);
    delay(300);
    lcd.setCursor(2*i,1);
    current_password[i]= (int)key - 48 ; // convert from ASCII to int ( el arkam el dakhla men el keypad betkon in ASCII fa baghyra le int 3shan at3ml ma3aha as-hal)
    i = i + 1 ; // password men 0->3 ya3ny 4 digits
       
    }

    if(key=='#'){
      if(i==4) { // katabt el password khalas el mafroud now a check lw sah aw ghalat
        if((current_password[0]==saved_password[0]) && (current_password[1]==saved_password[1]) &&
        (current_password[2]==saved_password[2]) && (current_password[3]==saved_password[3]) ){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Welcome");
            DOOR.write(-10);  // door is opened
             doorflag=1;
             digitalWrite(sensor1,LOW);
          

           // hena el mafroud aft7 el door ya3ny anady el servo ba2a
          
            delay(1000);
            lcd.clear();
            i=0 ;       
         }
        else { // password ghalat
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Incorrect Password");
             DOOR.write(70); // door is closed
             doorflag=0;
            delay(1000);
            lcd.clear();
             i=0 ;
             }
       }

     else if ( i!=4 ) { //dakhlt el # w el password maknsh khels lesa fa ghalat
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Incorrect Password");
      DOOR.write(70); // door is closed
      doorflag=0;
     delay(1000);
     lcd.clear();
     i=0 ;
    }
  
   }
 }


 void Change_Password(){

if(check==0){
    if(i==0) {
      lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Enter OLD Password :" );
     lcd.setCursor(0,1);
     lcd.print("- - - -" );
    }
    
    if(key=='0'||key=='1'||key=='2'||key=='3' 
    ||key=='4'||key=='5'||key=='6' 
    ||key=='7'||key=='8'||key=='9') {
    lcd.setCursor(2*i,1);
    lcd.print(key);
   //delay(300);
    lcd.setCursor(2*i,1);
    lcd.print("*");
    current_password[i]= (int)key - 48 ;
    i = i + 1 ; 
       
    }

    if(key=='#'){
      if(i==4) {
        if((current_password[0]==saved_password[0]) && (current_password[1]==saved_password[1]) &&
        (current_password[2]==saved_password[2]) && (current_password[3]==saved_password[3]) ){
          
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Enter New Password");
            lcd.setCursor(0,1);
            lcd.setCursor(0,1);
            lcd.print("- - - -" );
            check = 1; 
            i=0;
            while(key=='#'){key = keypad.getKey();} 
               }
         else{
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("Incorrect Password");
              DOOR.write(70); // door is closed
              doorflag=0;
             delay(1000);
             lcd.clear();
             i=0 ;
             }
          }        
       else{
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Incorrect Password");
        DOOR.write(70); // door is closed
        doorflag=0;
       delay(1000);
       lcd.clear();
       i=0 ;
       }
    }
 }


else if (check==1) { // dakhlt old password sah w 3ayz aghyro ba2a

            if(key=='0'||key=='1'||key=='2'||key=='3' 
               ||key=='4'||key=='5'||key=='6' 
               ||key=='7'||key=='8'||key=='9') {
                    lcd.setCursor(2*i,1);
                    lcd.print(key);
                    delay(300);
                    lcd.setCursor(2*i,1);
                    lcd.print("*");
                    new_password[i]= (int)key - 48 ;
                    i = i + 1 ; 
               }
            if(key=='#') { 
                      if(i==4){
                         lcd.clear();
                         lcd.setCursor(0,0);
                         lcd.print("Password Changed");
                         lcd.setCursor(0,1);
                         lcd.print("Welcome mahmoud");
                       
                          DOOR.write(-10); // door is closed
                          doorflag=1;
                         saved_password[0]=new_password[0];
                         saved_password[1]=new_password[1];
                         saved_password[2]=new_password[2];
                         saved_password[3]=new_password[3];
                         delay(1000);
                         lcd.clear();
                         i=0 ; 
                         check = 0 ;
                         order = 0 ;
                      }
                       
                     else {  
                       lcd.clear();
                       lcd.setCursor(0,0);
                       lcd.print("Please Enter");
                       lcd.setCursor(0,1);
                       lcd.print("4 digit password");
                       delay(1000);
                       lcd.clear();              
                       lcd.setCursor(0,0);
                       lcd.print("*Change Password*" );
                       lcd.setCursor(0,1);
                       lcd.print("Enter New Password");
                       lcd.setCursor(0,0);
                       lcd.print("and then press #" );
                       lcd.setCursor(0,1);
                       lcd.print("- - - -" );
                       i=0 ;   
                     }
                   }
       }
       delay(500);
 
 }
 
 ////////////////////////// PIR
void PIR(){
   pir_Reading = digitalRead(pirsen);
     // read the input pin
   if(pir_Reading==1){
     if(doorflag==1){
     digitalWrite(LED, HIGH);
     delay(4000);}
   else if(doorflag==0){
        digitalWrite(alarm, HIGH);
        lcd.clear();
        lcd.print("SECURITY WARNING!");
        delay(3000);
        lcd.clear();}
    }
    pir_Reading = digitalRead(pirsen);
    if(pir_Reading==0){
    digitalWrite(alarm, LOW);
    digitalWrite(LED,LOW);}
    delay(100);
}

void TEMP(){
   temp=analogRead(tempPin)*(500.0/1024);
  if(temp>23){ 
   DOOR.write(-5);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("FIRE WARNING !!");
      digitalWrite(tempOutput,HIGH); // buzzer
   delay(1500);
   lcd.setCursor(0,1);
   lcd.print("temp : ");
   lcd.print(temp);
   lcd.print(" C");
   doorflag=1;
   lcd.clear();
  }
  else 
  digitalWrite(tempOutput,LOW);

}
void Garagedoor(){
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
//distance= duration*0.034/2;
  distance = (duration/2) / 29.1;
if(distance >0 && distance <=4){
  digitalWrite(buzzPin,HIGH);
}

 else if(distance>=4 && distance <12)
{ digitalWrite(buzzPin,LOW);
  Garage.write(20);
  doorstatus=0;
   Road.write(30);


}
   else if(distance>=15 && distance<=20 && doorstatus==0){
      digitalWrite(buzzPin,LOW);
       Garage.write(120);
         Road.write(130);
       delay(3000);
    }
  

 else{
 Garage.write(20);
   Road.write(30);}
}
void waterlevel(){


  digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin2, HIGH);
// Calculating the distance
//distance= duration*0.034/2;
  distance2 = (duration2/2) / 29.1;
if(distance2 >0 && distance2 <=2){
  digitalWrite(buzzPin,HIGH);
}
else {
  digitalWrite(buzzPin,LOW);}

}