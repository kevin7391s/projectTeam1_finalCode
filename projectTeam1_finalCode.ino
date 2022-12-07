// observe the sensor output status for the Infared sensor
int ledPin = 13;
int pirPin = 7;
int pirValue;
int sec = 0;

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
  while (digitalRead(DHpin) == HIGH)
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


void setup() {
  // put your setup code here, to run once:

  // setup code for the infared sensor
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);

  // setup code for the temperature sensor
  Serial.begin(9600);
  pinMode(DHpin, OUTPUT);

  // setup code for led light
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);
  delay(1000);
  digitalWrite(led_pin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  // loop code for the infared sensor
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);
  //Observe the sensor output status
  sec += 1;
  Serial.print("Second: ");
  Serial.print(sec);
  Serial.print("PIR value: ");
  Serial.print(pirValue);
  Serial.print('\n');
  delay(1000);

  // loop code for the temperature sensor
  start_test();
  Serial.print("Current humdity =");
  //display the humidity-bit integer
  Serial.print(dat[0], DEC);
  //display the humidity decimal places;
  Serial.print('.');
  Serial.print(dat[1], DEC);
  Serial.println('%');
  Serial.print("Current temperature =");
  //display the temperature of integer bits
  Serial.print(dat[2], DEC);
  Serial.print('.');
  //display the temperature of decimal places
  Serial.print(dat[3], DEC);
  Serial.println('C');
  delay(700);

  // loop code for led light

  digitalWrite(led_pin,HIGH);
  delay(500);

}