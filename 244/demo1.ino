#define RED_PIN 2
#define YELLOW_PIN 3
#define GREEN_PIN 4

#define BUTTON 5 
#define BUZZER 7

#define RED_STATE 0
#define RED_YELLOW_STATE 1
#define YELLOW_STATE 2
#define GREEN_STATE 3

#define RED_MILLIS 10000
#define YELLOW_MILLIS 2000
#define GREEN_MILLIS 5000

int tl_state; // Traffic light state.
int buttonState=0;
unsigned long tmp;
unsigned long tl_timer; // Traffic light timer.
int key2change;
void setup() 
{

  // Configure LED pins as outputs.
  Serial.begin(9600);
  
  pinMode(RED_PIN,OUTPUT);
  pinMode(YELLOW_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  
  pinMode(BUTTON,INPUT);
  
  digitalWrite(YELLOW_PIN,LOW);
  digitalWrite(GREEN_PIN,LOW);
  digitalWrite(RED_PIN,LOW);
  
  // Initial state for states and timers..
  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;
  tmp=0;
  key2change = 0;
}



void loop() 
{
  // TL state machine.
  switch (tl_state) {
  case RED_STATE:
    Serial.println("red begin");
    
    // Turn red light on.
    digitalWrite(RED_PIN,HIGH);

    tone(BUZZER,50,250);
    if ( millis()>tl_timer ) 
    { 
      // Turn red light off.
      //Set timer for red-yellow state.
//      digitalWrite(RED_PIN,LOW);
      tl_state = RED_YELLOW_STATE;
      tl_timer = millis()+ YELLOW_MILLIS;
      Serial.println("red ends");
      noTone(BUZZER);
    }
    delay(500);
  break;
  
  case RED_YELLOW_STATE:
    Serial.println("red-yellow begin");
    digitalWrite(YELLOW_PIN,HIGH);
    
    while(millis()<tl_timer){
//      Serial.println(buttonState);
      if(buttonState == LOW){
        buttonState=digitalRead(BUTTON);
      }
    }
    //  buttonState=digitalRead(BUTTON);
    Serial.println(buttonState);
    
    digitalWrite(RED_PIN,LOW);
    digitalWrite(YELLOW_PIN,LOW);
    
    tl_state = GREEN_STATE;
    tmp=0;
    Serial.println("red-yellow ends");
    // Code for red-yellow state.
  break;
  
  case YELLOW_STATE:
    Serial.println("yellow begin");
    digitalWrite(YELLOW_PIN,HIGH);
    delay(2000);
    digitalWrite(YELLOW_PIN,LOW);
    tl_timer = millis() + RED_MILLIS;
    tl_state = RED_STATE;
    Serial.println("yellow ends");
    // Code for yellow state.
  break;
  
  case GREEN_STATE:
    Serial.println("green begin");
    //turn on green light
    digitalWrite(GREEN_PIN,HIGH);
    //turn on buzer on 500 off 1500
    
    
    if (millis()-tmp > 1500||tmp==0)
    {
      tmp = millis();
      tone(BUZZER,50,500);
      Serial.println(tmp);
    }
    
    
    if(buttonState == LOW)
      buttonState=digitalRead(BUTTON);
      Serial.println(buttonState);
    
    if (buttonState==HIGH&&key2change==0/*Timer expired AND touch-button was pressed*/ ) {
      // Turn green light off.
      // Set timer for yellow state.
      Serial.println("button is clicked");
      tl_timer = millis() + GREEN_MILLIS; 
      key2change = 1;
     }
     
    if (key2change == 1&&millis()>tl_timer)
    {
      buttonState=LOW;
      digitalWrite(GREEN_PIN,LOW);
      tl_timer = millis() + YELLOW_MILLIS;
      tl_state = YELLOW_STATE;
      noTone(BUZZER);
      Serial.println("green ends");
      tmp=0;
      key2change = 0;
    }
  break;
  }
  

}
