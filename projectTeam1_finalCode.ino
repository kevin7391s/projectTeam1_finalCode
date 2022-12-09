// global code for the temperature reader

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 3     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.

DHT dht(DHTPIN, DHTTYPE);

// observe the sensor output status for the Infared sensor
int ledPin = 14;
int pirPin = 7;
int pirValue;
int sec = 0;

int person_tracker = 0;
int ppl_count = 0;

// variables for the temperature sensor
int DHpin = 8;
byte dat[5];

// led light
int led_pin = 13;

byte read_data() {
  byte data;
  for (int i = 0; i < 8; i++) {
    if (digitalRead(DHpin) == LOW) {
      //wait for 50us
      while (digitalRead(DHpin) == LOW)
        ;
      //determine the duration of high level to determine the data is '0 'or '1'
      delayMicroseconds(30);
      if (digitalRead(DHpin) == HIGH)
        //high front and low in the post
        data |= (1 << (7 - i));
      //data '1 ',wait for the next one receiver
      while (digitalRead(DHpin) == HIGH)
        ;
    }
  }
  return data;
}

void start_test() {

  //bus down, send start signal
  digitalWrite(DHpin, LOW);
  //delay greater than 18ms, so DHT11 start signal can be detected
  delay(30);
  digitalWrite(DHpin, HIGH);
  //Wait for DHT11 response
  delayMicroseconds(40);
  pinMode(DHpin, INPUT);
  if (digitalRead(DHpin) == HIGH)
    ;
  //DHT11 response, pulled the bus 80us
  delayMicroseconds(80);
  if (digitalRead(DHpin) == LOW)
    ;
  //DHT11 80us after the bus pulled to start sending data
  delayMicroseconds(80);
  //receive temperature and humidity data, the parity bit is not considered
  for (int i = 0; i < 4; i++)
    dat[i] = read_data();
  pinMode(DHpin, OUTPUT);
  //send data once after releasing the bus, wait for the host to open the next Start signal
  digitalWrite(DHpin, HIGH);
}

/*
 bool readTemp(float temp)
{
     if(temp>=100.0){
     digitalWrite(led_pin,HIGH);
     return True;
     }
     else{
      digitalWrite(led_pin, LOW);S
      return False;
     }
}*/


void setup() {
  // put your setup code here, to run once:

  // setup code for the temperature reader
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  // setup code for the infared sensor
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);

  // setup code for the temperature sensor
  Serial.begin(9600);
  pinMode(DHpin, OUTPUT);

  // setup code for led light
  pinMode(13, OUTPUT);
 
}

void loop() {
//Serial.print("hello world");


    // loop code for led light

  // digitalWrite(led_pin,HIGH);
  // put your main code here, to run repeatedly:

  // Loop code for the temperature reader

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  f = f + 8;

  sec += 1;
  Serial.print("Loop #:");
  Serial.print(sec);
  Serial.print(" Temp: ");
  Serial.print(f);
  
  //readTemp(f);
  
  if(f >= 80 && pirValue == 1){
    // loop code for led light
    digitalWrite(led_pin,HIGH);
  }
  if(f <= 80 || pirValue == 0){
    digitalWrite(led_pin, LOW);
  }

  if(pirValue == 1)
  {
    person_tracker ++;
  }

  //int pircount = 0;
  /*
  while(pirValue == 1)
  {
    pircount++;
    if(pircount % 5 == 0)
    {
      ppl_count += 1;
    }
  } */




  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
 
  lcd.setCursor(0,0);
  lcd.print("Ppl count: ");
  lcd.setCursor(10,0);
  lcd.print(person_tracker);
  lcd.setCursor(15,0);
  //lcd.print("");
 
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.setCursor(6,1);
  lcd.print(f);
  lcd.setCursor(11,1);
  lcd.print("F");

  // loop code for the infared sensor
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);
  //Observe the sensor output status
  
  Serial.print(sec);
  Serial.print(" PIR value(movement): ");
  Serial.print(pirValue);
  Serial.print('\n');
  //delay(1000);

  // loop code for the temperature sensor
  /*
  start_test();
  Serial.print("Current humdity =");
  //display the humidity-bit integer
  Serial.print(dat[0], DEC);
  //display the humidity decimal places;
  Serial.print('.');
  Serial.print(dat[1], DEC);
  Serial.println('%');
  */
  //Serial.print("Current temperature =");
  //display the temperature of integer bits
  //Serial.print(dat[2], DEC);
  //Serial.print('.');
  //display the temperature of decimal places
  //Serial.print(dat[3], DEC);
  //Serial.println('C');
  delay(3500);

  
  

  



}