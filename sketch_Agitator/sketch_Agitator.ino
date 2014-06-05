
const int motor1Pin = 3;    // H-bridge leg 1 (pin 2, 1A)
const int motor2Pin = 4;    // H-bridge leg 2 (pin 7, 2A)

int switchPin = 8;
int ledPin = 13;
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean ledOn = false;
boolean initialised = false;
boolean started = false;

int counter = 0;
const int maxCountBeforeReset = 100;

void setup() {

  // set all the other pins you're using as outputs:
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);

}

boolean debounce(boolean last)
{
  boolean current = digitalRead(switchPin);
  if (last != current)
  {
    delay(5);
    current = digitalRead(switchPin);
  }
  return current;
}

void stopTray()
{
    //Stop the motor
    digitalWrite(motor1Pin, LOW);  // set leg 1 of the H-bridge high
    digitalWrite(motor2Pin, LOW);   // set leg 2 of the H-bridge low
}

void initialiseTray()
{
    counter = 0;
    
    //Resets the position of the tray
    digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low
    digitalWrite(motor2Pin, HIGH);  // set leg 2 of the H-bridge high
    delay(2000);                  // waits for a second          
      
    stopTray();
      
    initialised = true;
}

//Assumes that the tray is in the closed position
void moveTrayToStartPosition()
{
   //Move the tray out part way
    digitalWrite(motor1Pin, HIGH);  // set leg 1 of the H-bridge high
    digitalWrite(motor2Pin, LOW);   // set leg 2 of the H-bridge low
    delay(500);  
      
    stopTray();         
}

void loop() {

    if (!initialised) 
    {  
      initialiseTray();
    }
    
    
    currentButton = debounce(lastButton);
    if (lastButton == LOW && currentButton == HIGH)
    {
      ledOn = !ledOn;
      
      if (!ledOn)
      {
          initialiseTray();
          started = false;
      }
    } 
    lastButton = currentButton;
    digitalWrite(ledPin, ledOn);
  
    if (ledOn)
    {
      if (!started) 
      {
        moveTrayToStartPosition();  
        started = true;
      }
      
      counter = counter + 1;
      
      digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low
      digitalWrite(motor2Pin, HIGH);  // set leg 2 of the H-bridge high
      delay(550);                  // waits for a second
    
      digitalWrite(motor1Pin, HIGH);  // set leg 1 of the H-bridge high
      digitalWrite(motor2Pin, LOW);   // set leg 2 of the H-bridge low
      delay(555);  
      
      if (counter >= maxCountBeforeReset)
      {
        initialiseTray();
        moveTrayToStartPosition();  
      }
    }


}
