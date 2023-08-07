#define LED_LOSE 13
#define LEDR 12
#define LEDY 11
#define LEDB 10
#define LEDG 9
#define LED_WIN 8

#define pb_red 7
#define pb_yellow 6
#define pb_blue 5
#define pb_green 4

int sequence[5] = {};
int response_seq[5] = {};
int num_round = 0;
int index = 0;
int Incorrect = 0; //player answer it wrong will turn this state to 1
int reset_state = 1; //state 1 indicate to reset the game
int show_sequence = 1; //time when it is showing sequence so player cant input during this time
int timeout = 0;

//define functions prototypes

void win() // happen when the player win
{
  for(int blink = 0; blink < 5; blink++)
  {
    digitalWrite(LED_WIN, HIGH);
    delay(500);
    digitalWrite(LED_WIN, LOW);
    delay(500); 
  }
  reset_state = 1;
}

void lose()// happen when the player lose
{
  for(int blink = 0; blink < 5; blink++)
  {
    digitalWrite(LED_LOSE, HIGH);
    delay(500);
    digitalWrite(LED_LOSE, LOW);
    delay(500); 
  }
  reset_state = 1;
}

/*
happen when reset_state = 1
when the reset function start, it will
reset the sequence to a random one and also 
reset the game.
*/ 
void reset()
{
  sequence[5] = {};
  response_seq[5] = {};
  num_round = 0;
  index = 0;
  Incorrect = 0;
  reset_state = 1;
  show_sequence = 1;
  timeout = 0;
  randomSeed(analogRead(A2));
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDY, HIGH);
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDG, HIGH);
  delay(2000);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDY, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
  delay(2000);
  generate(sequence);
}

void generate(int sequence[5])//generate new sequence
{
  for(int i = 0; i < 5; i++)
  {
    sequence[i] = random(4);
    
    Serial.println(sequence[i]);
  } 	
}

void response(int signal)// blink correspond to signal
{
  	delay(500);
	if(signal == 0) // red blink
    {
  	  digitalWrite(LEDR, HIGH);
      delay(500);
      digitalWrite(LEDR, LOW);
      delay(500);
    }
  	if(signal == 1) // yellow blink
    {
  	  digitalWrite(LEDY, HIGH);
      delay(500);
      digitalWrite(LEDY, LOW);
      delay(500);
    }
  	if(signal == 2) // blue blink
    {
  	  digitalWrite(LEDB, HIGH);
      delay(500);
      digitalWrite(LEDB, LOW);
      delay(500);
    }
  	if(signal == 3) // green blink
    {
  	  digitalWrite(LEDG, HIGH);
      delay(500);
      digitalWrite(LEDG, LOW);
      delay(500);
    }
}

void showSequence(int sequence[5], int index)
{
  for(int i = 0; i <= index; i++)
  {
  	response(sequence[i]);
    
    Serial.println(sequence[i]);
  }
}

//initialization
void setup()
{
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LED_LOSE, OUTPUT);
  pinMode(LED_WIN, OUTPUT);
  pinMode(pb_red, INPUT);
  pinMode(pb_yellow, INPUT);
  pinMode(pb_blue, INPUT);
  pinMode(pb_green, INPUT);
  Serial.begin(9600); 
   //configure pin modes (inputs, outputs) etc.
}


//main program loop
void loop()
{
  if(reset_state == 1) // reset sequence and game
  {
  	reset();
    reset_state = 0;
  }
  if(Incorrect == 0 && num_round < 5)// Receive your input
  {
    if(index <= num_round)
    {
      if(show_sequence == 1) // show player the sequence for the round
      {
        Serial.println("Round");
        showSequence(sequence, num_round);
        show_sequence = 0;
      }
      if(digitalRead(pb_red) == HIGH) // Receive your input
      {
        digitalWrite(LEDR, HIGH);
        delay(500);
        digitalWrite(LEDR, LOW);
        response_seq[index] = 0;
        Serial.println(response_seq[index]); 
        delay(500);
        timeout = 0; //reset time if player respond
        index++;
      }
      if(digitalRead(pb_yellow) == HIGH) // Receive your input
      {
        digitalWrite(LEDY, HIGH);
        delay(500);
        digitalWrite(LEDY, LOW);
        response_seq[index] = 1;
        Serial.println(response_seq[index]); 
        delay(500);
        timeout = 0; //reset time if player respond
        index++;
      }
      if(digitalRead(pb_blue) == HIGH) // Receive your input
      {
        digitalWrite(LEDB, HIGH);
        delay(500);
        digitalWrite(LEDB, LOW);
        response_seq[index] = 2;
        Serial.println(response_seq[index]); 
        delay(500);
        timeout = 0; //reset time if player respond
        index++;
      }
      if(digitalRead(pb_green) == HIGH) // Receive your input
      {
        digitalWrite(LEDG, HIGH);
        delay(500);
        digitalWrite(LEDG, LOW);
        response_seq[index] = 3;
        Serial.println(response_seq[index]); 
        delay(500);
        timeout = 0; //reset time if player respond
        index++;
      }  
      if(timeout > 10000) // Timeout when player didn't respond in roughly 12 seconds
      {
      	Incorrect = 1;
      }
      timeout++;
      delay(1);
    }
    else //Check if your input is correct
    {
      for(int i = 0; i <= num_round; i++)
      {
        Serial.println("Check");
        if(response_seq[i] != sequence[i])
        {
          Incorrect = 1;
        }
      }
      if(Incorrect == 0)
      {
       	index = 0;
      	num_round++;   
        show_sequence = 1;
        Serial.println("NIce");
        delay(1000);
      }
    }  
  }
  if(Incorrect == 0 && num_round >= 5) // Win the game
  {
    Serial.println("You WIN");
    win();
    reset_state = 1;
  }
  if(Incorrect == 1)
  {
  	Serial.println("You lose"); // Lose the game
    lose();
    reset_state = 1;
  }
}