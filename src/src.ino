int inputPins[] = {8, 9, 10, 11};
int recordButton = 12;

int latch = 3;
int data = 4;
int clock = 2;

int piezo = 6;

//From C4 to D5, inclusive, includes semitones, 15 notes total
int notes[] = {0, 1911,1804,1703,1607,1517,1432,1351,1276,1204,1136,1073,1012,956,902,851};
int notesDelayed[] = {2000, 1868,1761,1660,1564,1474,1389,1308,1233,1161,1093,1030,969,913,859,808}; //above array delayed by 43 microseconds
int recordedNote[100];
int recordedNoteTime[100]; //In milliseconds



void setup() {
  
  for (int i=0; i<4; ++i){
    pinMode(inputPins[i], INPUT);
  }
  pinMode(recordButton, INPUT);
  
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(piezo, OUTPUT);
  
  clearNoteMemory();
  
  Serial.begin(9600);
  //Apparently reading from an unconnected pin is random-ish for seed
  randomSeed(analogRead(0));
}



void clearNoteMemory(){ //self-explanatory
  for (int i=0; i<100; ++i){
    recordedNote[i]=0;
    recordedNoteTime[i]=0;
  }
}



void changeLed(){//generate random 4 bit integer, sends to display
  
  int randomNum = random(16);
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, randomNum);
  digitalWrite(latch, HIGH);  
  
}



void playNote(int halfPeriod, int halfPeriod2, long duration){ //in microsec
  
  //counter here to count calls to this method
  //every 64th call will blink
  static int counter = 0;
  counter++;
  
  //creates square wave approximation of sound wave
  //halfPeriod is time HIGH, halfPeriod2 is time LOW
  
  if (halfPeriod>0){
    for (long i = 0; i< duration; i+= halfPeriod*2){
      digitalWrite(piezo, HIGH);
      delayMicroseconds(halfPeriod);
      digitalWrite(piezo, LOW);
      
      if (counter & B01000000){
        halfPeriod2 -=264; //since this check causes a 264microsec delay, this compensates
        changeLed();
        counter = 0;
      }
      delayMicroseconds(halfPeriod2);
    }
  }
  else{ // delay here to keep timing correct
    duration = duration/1000; //delayMicroseconds() cannot handle huge numbers
    delay(duration);
    if (counter & B01000000){
      changeLed();
      counter = 0;
    }
  }
}



int readInButtons() //Unpressed is LOW, pressed is HIGH
{
  int temp = 0;
  for (int i=0; i<4; ++i){
    if (digitalRead(inputPins[i])==HIGH){
      temp |= 1<<i;
    }
  }
  return temp; //returns 4 bit integer, where each bit is a button's on/off state
}


void loop(){
  
  static int prevButtonState = 0;
  static int currentButtonState = 0;
  static boolean isRecording = 0;
  static long noteStartTime = 0;
  static int noteNum = 0;
  
  //random flashing here
  //changeLed();
  //calling flash moved to piezo method, since each call caused timing issues
  
  //read Recording button info
  isRecording = digitalRead(recordButton)==HIGH; 
  
  //play Recording if it exists, and then clear recording data
  if (!isRecording && noteNum>0){
    /*for (int i = 0; i<noteNum; ++i)
    {
      Serial.print(recordedNote[i], BIN);
      Serial.print(" time (msc): ");
      Serial.println(recordedNoteTime[i]);
    }*/
    
    for (int i = 0; i < noteNum; ++i)
      playNote(notes[recordedNote[i]], notes[recordedNote[i]], ((long)recordedNoteTime[i])*1000);
    noteNum=0;
    noteStartTime=0;
    clearNoteMemory();
  }
  
  //read 4 button's info
  currentButtonState = readInButtons();
  //Serial.println(currentButtonState,BIN);
  
  
  //If button is pressed, then perform recording logic
  if (currentButtonState!= prevButtonState && isRecording){
    if (noteStartTime>0){ //person must have pressed 2th or later note
      //store timing
      recordedNote[noteNum]=prevButtonState;
      recordedNoteTime[noteNum]=millis()-noteStartTime;
      noteNum++;
    }
    noteStartTime=millis();
  }
  
  prevButtonState=currentButtonState;
  
  //default play has LOW period compensated for timing, also has delay for blank note
  playNote(notes[currentButtonState], notesDelayed[currentButtonState], notesDelayed[currentButtonState]); 
  
}
