#define ENCR 33// YELLOW
#define ENCR1 32 // GREEN
#define ENCL 34 // YELLOW
#define ENCL1 35 //GREEN

#define RM 25 // pwm right motor
#define LM 13 // pwm left motor

// the number of t he Motor Pins
const int RM1 = 26; //Right motor 1st terminal
const int RM2 = 27;  //Right motor 2nd terminal
const int LM1 = 14; //left motor 1st terminal
const int LM2 = 12;  //left motor 2nd terminal

//Motor Channel
const int RM_ch = 0; //PWM channel--timer based
const int LM_ch = 1; //PWM channel--timer based

// setting PWM properties 
const int motors_freq = 5000; //Motor Frequency
const int motors_res = 8; //Motor Resoltion 8 bits

volatile int ticksR = 0;
volatile int ticksL = 0; 


int prevticksR = 0 ;
int prevticksL = 0;
int pwmR = 170;
int pwmL = 170;
int motors_offset = 1;


void readEncoderR();
void readEncoderL();
void forward();
void backward();
void left();
void right();
void brake();
void drivestraight();

void setup() {
  
  Serial.begin(115200);
  
  pinMode(ENCR,INPUT_PULLUP);
  pinMode(ENCR1,INPUT_PULLUP);   
  pinMode(ENCL,INPUT_PULLUP);
  pinMode(ENCL1,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCR),readEncoderR,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL),readEncoderL,RISING);

  
  pinMode(RM,OUTPUT);
  pinMode(RM1,OUTPUT);
  pinMode(RM2,OUTPUT);
  pinMode(LM,OUTPUT);
  pinMode(LM1,OUTPUT);
  pinMode(LM2,OUTPUT);

  ledcSetup(RM_ch, motors_freq, motors_res);
  ledcAttachPin(RM, RM_ch);  
  ledcSetup(LM_ch, motors_freq, motors_res);
  ledcAttachPin(LM, LM_ch);

  
  delay(3000);
}

void loop()
{
 drivestraight(3000);
}



void drivestraight(int limit) 
{
  while( ticksR <= limit && ticksL <= limit)
  {
 
    // Number of ticks counted since last time
    int diff_ticksR = ticksR - prevticksR;
    int diff_ticksL = ticksL - prevticksL;

    // Store current tick counter for next time
    prevticksR = ticksR;
    prevticksL = ticksL;

    // If left is faster, slow it down and speed up right
    if ( diff_ticksR > diff_ticksL ) {
      pwmR -= motors_offset;
      pwmL += motors_offset;
    }

    // If right is faster, slow it down and speed up left
    else if ( diff_ticksR < diff_ticksL ) {
      pwmR += motors_offset;
      pwmL -= motors_offset;        
    }
    else
     {
      //do nothing
     }
    pwmR = constrain(pwmR, 100,200);
    pwmL = constrain(pwmL, 100,200);

    Serial.print(ticksR); Serial.print("(R)  -  ");Serial.print(ticksL);Serial.print("(L)"); Serial.print(" ---> ");Serial.println(ticksR-ticksL);
    forward(pwmR,pwmL);
    delay(20);
    
  }
  brake();
  delay(2000);
  Serial.print(ticksR); Serial.print("(R)  -  ");Serial.print(ticksL);Serial.print("(L)"); Serial.print(" ---> ");Serial.println(ticksR-ticksL);
}


// Motors Directions with PWM

void forward(int pwmr,int pwml)
{
   digitalWrite(RM1, HIGH);
   digitalWrite(RM2, LOW);
   ledcWrite(RM_ch, pwmr);  
   digitalWrite(LM1, HIGH);
   digitalWrite(LM2, LOW);  
   ledcWrite(LM_ch, pwml);
}

void backward(int pwmr,int pwml)
{
   digitalWrite(RM1, LOW);
   digitalWrite(RM2, HIGH);
   ledcWrite(RM_ch, pwmr);  
   digitalWrite(LM1, LOW);
   digitalWrite(LM2, HIGH);  
   ledcWrite(LM_ch, pwml);
}
void left(int pwmr,int pwml)
{
   digitalWrite(RM1, HIGH);
   digitalWrite(RM2, LOW);
   ledcWrite(RM_ch, pwmr);  
   digitalWrite(LM1, LOW);
   digitalWrite(LM2, HIGH);  
   ledcWrite(LM_ch, pwml);
}

void right(int pwmr,int pwml)
{
   digitalWrite(RM1, LOW);
   digitalWrite(RM2, HIGH);
   ledcWrite(RM_ch, pwmr);  
   digitalWrite(LM1, HIGH);
   digitalWrite(LM2, LOW);  
   ledcWrite(LM_ch, pwml);
}

void brake()
{
   digitalWrite(RM1, LOW);
   digitalWrite(RM2, LOW);
   ledcWrite(RM_ch, 0);  
   digitalWrite(LM1, LOW);
   digitalWrite(LM2, LOW);  
   ledcWrite(LM_ch, 0);
}

//Interrupt Functions for left and right motors

void readEncoderR(){
  int b = digitalRead(ENCR1);
  if(b > 0){
    ticksR++;
  }
  else{
    ticksR--;
  }
}

void readEncoderL(){
  int a = digitalRead(ENCL1);
  if(a > 0){
    ticksL++;
  }
  else{
    ticksL--;
  }
}
