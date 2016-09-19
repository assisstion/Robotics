//Intro to Circuits and Arduino Project
//by Markus and Jordan

#include <OneWire.h> //Using someone else's library for temperature sensor

//Set the ports for the colors
int red = 10;
int blue = 9;
int green = 8;
int yellow = 7;

int DS18S20_Pin = 5; //DS18S20 Signal pin on digital 5

int rgb1 = 4;
int rgb2 = 3;
int rgb3 = 2;

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 5

int timeDelay = 100; // Delay in ms between each update

// Runs once when you press reset or power the board
void setup() {
  // Begins the Serial, allowing for printing to the serial monitor
  Serial.begin(9600);
  // Initialize the pins we're using to the correct pinMode
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(rgb1, OUTPUT);
  pinMode(rgb2, OUTPUT);
  pinMode(rgb3, OUTPUT);
  pinMode(A0, INPUT);
}

// Runs over and over again forever
void loop() {
  // Updates the LED state based on the input, 
  // then turn the LED on, then updates again, then turn it off.
  // Repeat this for every LED and every RGB color.
  updateFromInput();
  digitalWrite(red, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(timeDelay);              // wait for a second
  updateFromInput();
  digitalWrite(red, LOW);    // turn the LED off by making the voltage LOW
  delay(timeDelay);              // wait for a second
  updateFromInput();
  digitalWrite(yellow, HIGH);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(yellow, LOW);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(green, HIGH);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(green, LOW);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(blue, HIGH);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(blue, LOW);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(rgb1, HIGH);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(rgb1, LOW);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(rgb2, HIGH);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(rgb2, LOW);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(rgb3, HIGH);
  delay(timeDelay);
  updateFromInput();
  digitalWrite(rgb3, LOW);
  delay(timeDelay);
}

// Updates the LED states and timeDelay based on the input sources
void updateFromInput(){
  // Gets the temperature from the temperature sensor
  float temperature = getTemp();
  // Prints the temperature to the serial monitor
  Serial.println(temperature); 
  // If the temperature is below 28 degrees Celcius
  // Set the LED delay to 500 milliseconds
  if(temperature < 28){
    timeDelay = 500;
  }
  // If the temperature is above or equal to 28 degrees Celcius
  // Set the LED delay to 100 milliseconds
  else{
    timeDelay = 100;
  }

  // Gets the analog value from the photoresistor
  float photoReading = analogRead(A0);
  // Prints the analog reading to the serial monitor
  Serial.println(photoReading);
  // Turn all LEDs on if photo reading is low
  // Note that this setting is overriden by the 
  // currently blinking LED by design
  if(photoReading < 450){
    digitalWrite(red, HIGH);
    digitalWrite(yellow, HIGH);
    digitalWrite(blue, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(rgb1, HIGH);
    digitalWrite(rgb2, HIGH);
    digitalWrite(rgb3, HIGH); 
  }
  // Turn all LEDs off if photo reading is high
  // Note that this setting is overriden by the 
  // currently blinking LED by design
  else{
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(green, LOW);
    digitalWrite(rgb1, LOW);
    digitalWrite(rgb2, LOW);
    digitalWrite(rgb3, LOW);
  }
}

// Found from online source: http://bildr.org/2011/07/ds18b20-arduino/
// Special function for getting the temperature of the device
// You can ignore the code inside here
float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
}
