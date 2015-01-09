
// the setup function runs once when you press reset or power the board

int totalLeds = 5;
int bottomLedPins[] = {13, 12, 11, 10, 9};
int topLedPins[] = {8, 7, 3, 5, 4};
long levelTimeouts[] = {4000,2000,1500,1000,700};

int ledDelay = 50;
int leftPin = 2;
int rightPin = 1;
int selectedLed = 0;
int enemyLed = 0;
boolean keyPressed = false;

int lastSingleKeyPressed = 0;
long lastEnemyChange = 0;
long lastSingleKey = 0;
long lastTimeInputRead = 0;
long readInputDelayInterval = 200;
long waitForSecondKeyInterval = 100;

int soundPin = 6;


int duration = 250;

int notes[] = 
{261, 293, 329, 349, 392, 440, 493, 523, 587, 659, 698, 783, 880};
// frequencies for musical notes - from middle C, D, E, F, G, A, B, C, D, E, F, G, A

int currentScore = 0;
int currentLevel = 1;

void setup() {
  for(int currentLed = 0; currentLed < totalLeds; currentLed ++)
  {
    pinMode(topLedPins[currentLed], OUTPUT);
  }
  
  for(int currentLed = 0; currentLed < totalLeds; currentLed ++)
  {
    pinMode(bottomLedPins[currentLed], OUTPUT);
  }

  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(soundPin, OUTPUT);
  displayLevel();
  toGameplay();
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(rightPin) == HIGH && digitalRead(leftPin) == HIGH)
   {
          // both clicked
        if(selectedLed == enemyLed)
          win();
        else
          fail();
   }
   else if(digitalRead(leftPin) == HIGH || digitalRead(rightPin) == HIGH)
   {
    
     if(keyPressed == false)
     {
       lastTimeInputRead = millis();
     }
     
     keyPressed = true;
     
     if(millis() - lastTimeInputRead > waitForSecondKeyInterval)
     {
       if(digitalRead(leftPin) == HIGH) moveLed(-1); 
       if(digitalRead(rightPin) == HIGH) moveLed(1);
       lastTimeInputRead = millis();
     }
   }
   else
   {
     keyPressed = false;
   }
  
   if(millis() - lastEnemyChange > getCurrentTimeout())
     fail();
}

void win()
{
   currentScore ++;
   tone(soundPin,880, 200);
   displayScore();
   
   if(currentScore == 5)
   {
     currentScore = 0;
     if(currentLevel == 5)
     {
       while(true) displayLevel();
     }
     currentLevel++;
     displayLevel();
   }
   toGameplay();
}

void fail()
{
   currentScore = 0;
   tone(soundPin,261, 200);
   displayScore();
   toGameplay();
}

void displayScore()
{
 hideAllLights(bottomLedPins);
 displayLights(bottomLedPins,currentScore);
 displayAllLights(topLedPins);
 hideLights(topLedPins, currentScore);
 delay(1000);
 clearDisplay();
}

long getCurrentTimeout()
{
  return levelTimeouts[currentLevel-1];
}

void displayLevel()
{
  clearDisplay();
  for(int i = 0; i < 4; i++)
  {
    tone(soundPin, notes[3*i], 200);
    delay(500);
    displayLights(bottomLedPins,currentLevel);
    displayLights(topLedPins,currentLevel);
    delay(500);
    clearDisplay();
  }
}

void clearDisplay()
{
 hideAllLights(bottomLedPins);
 hideAllLights(topLedPins);
}


void toGameplay()
{
  digitalWrite(bottomLedPins[selectedLed], HIGH);
  moveEnemy();
}

void moveEnemy()
{
  updateLedTop(random(0,totalLeds));
  lastEnemyChange = millis();
}

void moveLed(int byAmount)
{
  updateLedBottom((selectedLed + byAmount + totalLeds) % totalLeds);
}

void updateLedBottom(int newLed)
{
  digitalWrite(bottomLedPins[selectedLed], LOW);
  selectedLed = newLed;
  digitalWrite(bottomLedPins[selectedLed], HIGH);
}

void updateLedTop(int newLed)
{
  digitalWrite(topLedPins[enemyLed], LOW);
  enemyLed = newLed;
  digitalWrite(topLedPins[enemyLed], HIGH);
}

void displayLights(int lightset[], int count)
{
  for(int currentLed = 0; currentLed < count; currentLed ++)
  {
    digitalWrite(lightset[currentLed], HIGH);
  }
}

void hideLights(int lightset[], int count)
{
  for(int currentLed = 0; currentLed < count; currentLed ++)
  {
    digitalWrite(lightset[currentLed], LOW);
  }
}

void displayAllLights(int lightset[])
{
  displayLights(lightset,totalLeds);
}

void hideAllLights(int lightset[])
{
  hideLights(lightset,totalLeds);
}
