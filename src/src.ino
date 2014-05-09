

int inputPins[] = {9, 10, 11, 12};
int recordButton = 13;

int buttonState = 0;
int recordState = 0;

int latch = 3;
int data = 4;
int clock = 2;

int piezo = 6;

int blinkInterval = 100; //milliseconds

//From B3 to D5, inclusive, includes semitones, 15 notes total
int notes[] = {2025,1911,1804,1703,1607,1517,1432,1351,1276,1204,1136,1073,1012,956,902,851};

int recordedNotes[1000];
int recordedNoteTime[1000]; //In milliseconds

void setup() {
  
  for (int i=0; i<5; ++i){
    pinMode(inputPins[i], INPUT);
  }
  
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  
  Serial.begin(9600);
  //Apparently reading from an unconnected pin is random-ish for seed
  randomSeed(analogRead(0));
}


void changeLed(){
  
  //unsigned long time = millis();
  
  //if ((((time & B0100000)) >> 6) == 1){
    int randomNum = random(16);
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, randomNum);
    digitalWrite(latch, HIGH);
  //}
  
}

void playNote(int halfPeriod, long duration){ //in microsec
  
  for (long i = 0; i< duration; i+= halfPeriod*2){
    digitalWrite(piezo, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(piezo, LOW);
    delayMicroseconds(halfPeriod);
  }
}

int readInButtons()
{
  int temp;
  for (int i=0; i<4; ++i){
    if (digitalRead(inputPins[i])==LOW)
      temp |= 1<<i;
  }
  return temp;
}


void loop(){
  
  //random flashing here
  
  changeLed();
  
  //Currently 4 button
  //Unpressed is HIGH, pressed is LOW
  //For loop flips bits is unpressed is LOW, pressed is HIGH for button with bit n, from the right
  /*buttonState = readInButtons();
  
  Serial.println(buttonState);
  
  recordState = digitalRead(recordButton)==LOW; //need to flip bit
  
  //if (buttonState == 0)
    playNote(buttonState, 25000L); //default play 25millisec
  */
  /*else { //buttonState ==1f 
    unsigned long temptime= millis();
    int pressTime = 0;
    int currentButtonState;
    int numberOfNotes = 0;
    unsigned long currentTime = 0;
    
    while (digitalRead(recordButton)==LOW){
      
      currentButtonState = readInButtons();
      currentTime = millis();
      
      if (currentButtonState !=buttonState){
        
        pressTime = currentTime - temptime;
        
        recordedNotes[numberOfNotes] = currentButtonState;
        recordedNoteTime[numberOfNotes] = pressTime;
        
        numberOfNotes ++;
        
      }
      
      
    }
  }*/    
  delay(100);    
  
  
}
