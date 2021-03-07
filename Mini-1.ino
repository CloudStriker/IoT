int merah = 23;
int kuning = 22;
int biru = 1;

unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 1000;
int ledState = LOW;

int intervalMerah = 1000;
unsigned long previousMerah = 0;

int intervalBiru = 2000;
unsigned long previousBiru = 0;

int intervalKuning = 3000;
unsigned long previousKuning = 0;


void setup(){
  pinMode(merah, OUTPUT);
  pinMode(biru, OUTPUT);
  pinMode(kuning, OUTPUT);
}



void checkMerah(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMerah >= intervalMerah) {
    //save this reading!
    previousMerah = currentMillis;

    //figure out if you should turn the LED on or off
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(merah, ledState);
  }
}

void checkKuning() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousKuning >= intervalKuning) {
    //save this reading!
    previousKuning = currentMillis;

    //figure out if you should turn the LED on or off
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(kuning, ledState);
  }
}

void checkBiru() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousBiru >= intervalBiru) {
    //save this reading!
    previousBiru = currentMillis;

    //figure out if you should turn the LED on or off
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(biru, ledState);
  }
}

void loop(){
    checkMerah();
    checkKuning();
    checkBiru();
}
