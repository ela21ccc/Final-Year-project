//pateint 1 or ESKOCH AND UNO
// For communication protocloes
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(6,8,5,4,3,2);

//notification 


// Blood pressure

int potPin=A0; //To read the Blood Pressure sensor value from A0 pin
int readVal; //Declearing readValue variable
float Voltage;//Declearing our voltage variable
String unit= " V"; // To Hold the SI unit
//Body Temperature
const int Sensor=A1;  //To read Body Temperature Sensor value from A1 pin
float vout; // to store the voltage value from the sensor
float tempC;//to temperature in degree Celsius

RF24 radio(9,10);
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL,0xF0F0F0F0E2LL,0xF0F0F0F0E3LL };

void setup() {
  pinMode(potPin,INPUT);  // A0 for Blood pressure sensor
  pinMode(Sensor,INPUT); //A1 is an input pin for body temperature
  pinMode(1,OUTPUT);
  pinMode(7,OUTPUT);
  Serial.begin(9600);
  //LCD module
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Welcome to PMS");
  
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);//t nesscesrly have to match with the raspberry pi it is the power level
  radio.setChannel(0x76); // have to match with the raspberry pi 
  radio.openWritingPipe(pipes[1]);//iting to the raspberry pi 
  radio.enableDynamicPayloads();
  radio.powerUp();
  // 
  
}

void loop(void){
  
    //rED LED
  // Blood pressure
    readVal=analogRead(potPin);  //Read "potPin" and put value in readVal
    Voltage=(5.0/1023.0)*(readVal);
    Serial.print(Voltage);
    Serial.println(unit);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("V=");
    lcd.print(Voltage);
    lcd.print(unit);
     //Body Temperature
    vout=analogRead(Sensor); //reading analog input from A0
    vout=(vout*5000)/1023;
    Serial.print("Temp is= ");
    Serial.print(tempC);
    Serial.println("C"); 
    tempC=(vout-500)/10;  //Considering offset voltage of 500mv and 10mv/c 
    lcd.setCursor(0,1);
    lcd.print("T=");
    lcd.print(tempC);
    lcd.print(" C");
    // Dengerous conditions
    if((Voltage>4)||(Voltage<1)||(tempC>25)||(tempC<10))
    {
        digitalWrite(7,HIGH);
        delay(200);
       
         }
    else{
        digitalWrite(7,LOW);
        delay(200);
      }
    //Sending  Setup
    String both="0";
    both+=String(Voltage);
    both+=String(tempC);
    Serial.print(both);
    char botht[11];
    both.toCharArray(botht,11);
    radio.write(&botht,sizeof(botht));
    delay(4000);
}
