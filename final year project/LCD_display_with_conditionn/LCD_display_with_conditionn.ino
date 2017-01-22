//pateint 2 MEGA AND ELA
// For communication protocloes
#include <math.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <LiquidCrystal.h>

// Blood pressure

LiquidCrystal lcd(11,42,5,4,3,2);
int potPin=A0; //To read the Blood Pressure sensor value from A0 pin
int readVal; //Declearing readValue variable
float Voltage;//Declearing our voltage variable
String unit= "V"; // To Hold the SI unit
//Body Temperature
double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1))); 
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;          
 Temp = (Temp * 9.0)/ 5.0 + 32.0; 
 return Temp;
}

RF24 radio(9,10);
const uint64_t pipes[3] = {0xF0F0F0F0E1LL,0xF0F0F0F0E2LL,0xF0F0F0F0E3LL};

void setup() {
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);

  pinMode(potPin,INPUT);  // A0 for Blood pressure sensor
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("Hello World !!");
  
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);//t nesscesrly have to match with the raspberry pi it is the power level
  radio.setChannel(0x76); // have to match with the raspberry pi 
  radio.openWritingPipe(pipes[2]);//iting to the raspberry pi 
  radio.enableDynamicPayloads();
  radio.powerUp();

  
}

void loop(void){
  // put your main code here, to run repeatedly:
  // Blood pressure
    readVal=analogRead(potPin);  //Read "potPin" and put value in readVal
    Voltage=(5.0/1023.0)*(readVal);
    Serial.print(Voltage);
    Serial.println(unit);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("V= ");
    lcd.print(Voltage);
    lcd.print(unit);
    //Body Temperature
    int val;                
    double temp;    
    val=analogRead(1);      
    temp=Thermistor(val); 
    Serial.print("Temperature = ");
    Serial.print(temp);  
    Serial.print(" F");
    lcd.setCursor(0,1);
    lcd.print("T= ");
    lcd.print(temp);
    lcd.print(" F");
    
    if((temp < 160) || (temp > 190) || (Voltage < 1) || (Voltage > 4))
    {
      digitalWrite(7,LOW);
      digitalWrite(6,HIGH);
    }
    else {  
      digitalWrite(7,HIGH);
      digitalWrite(6,LOW);
    }
    
    
    //Sending  Setup
    String both="1";
    both+=String(Voltage);
    both+=String(temp);
    Serial.print(both);
    char botht[11];
    both.toCharArray(botht,11);
    radio.write(&botht,sizeof(botht));
    //const char ela[]="1ela";
    //radio.write(&ela,sizeof(ela));
    delay(6000);
}
