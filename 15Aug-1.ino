#define ENCA 18 // YELLOW
#define ENCB 19 // WHITE
#define ENCA1 17 // YELLOW
#define ENCB1 5

// the number of the Motor Pins
const int RM1 = 2; //left motor 1st terminal
const int RM2 = 15;  //left motor 1st terminal
const int LM1 = 0; //left motor 1st terminal
const int LM2 = 4;  //left motor 1st terminal

//Motor Channel
const int RM1_ch = 0; //PWM channel--timer based
const int RM2_ch = 1;
const int LM1_ch = 2; //PWM channel--timer based
const int LM2_ch = 3;

// setting PWM properties 
const int motor_freq = 5000; //Motor Frequency
const int motor_res = 8; //Motor Resoltion 8 bits

volatile int ticks = 0;
volatile int ticks1 = 0; 


    int prevticks = 0 ;
    int prevticks1 = 0;
    int pwm = 200;
    int pwm1 = 200 ;
    int motor_offset = 4;

    

void setup() {
  Serial.begin(115200);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT); 
  pinMode(ENCA1,INPUT);
  pinMode(ENCB1,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA1),readEncoder1,RISING);
  
  pinMode(RM1,OUTPUT);
  pinMode(RM2,OUTPUT);
  pinMode(LM1,OUTPUT);
  pinMode(LM2,OUTPUT);

  ledcSetup(RM1_ch, motor_freq, motor_res);
  ledcAttachPin(RM1, RM1_ch);
  ledcSetup(RM2_ch, motor_freq, motor_res);
  ledcAttachPin(RM2, RM2_ch);
  
  ledcSetup(LM1_ch, motor_freq, motor_res);
  ledcAttachPin(LM1, LM1_ch);
  ledcSetup(LM2_ch, motor_freq, motor_res);
  ledcAttachPin(LM2, LM2_ch);
}

void loop()
{
 driveStraight(ticks,ticks1);
 Serial.print(ticks);
 Serial.print('\t');
 Serial.println(ticks1);
}


void setmotor(int dir, int pwm, int pwm1)
{
  if(dir==1)
  {
   ledcWrite(RM1_ch, pwm);
   ledcWrite(RM2_ch, 0);
   ledcWrite(LM1_ch, pwm1);
   ledcWrite(LM2_ch, 0);
  }
  else if(dir==-1){
   ledcWrite(RM1_ch, 0);
   ledcWrite(RM2_ch, pwm);
   ledcWrite(LM1_ch, 0);
   ledcWrite(LM2_ch, pwm1);
  }
  else
  {
  ledcWrite(RM1_ch, 0);
  ledcWrite(RM2_ch, 0);  
  ledcWrite(LM1_ch, 0);
  ledcWrite(LM2_ch, 0);  
    }  
}




void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    ticks++;
  }
  else{
    ticks--;
  }
}

void readEncoder1(){
  int b = digitalRead(ENCB1);
  if(b > 0){
    ticks1++;
  }
  else{
    ticks1--;
  }
}


void driveStraight(int ticks,int ticks1) 
{

    // Number of ticks counted since last time
    int diff_ticks = ticks - prevticks;
    int diff_ticks1 = ticks1 - prevticks1;

    // Store current tick counter for next time
    prevticks = ticks;
    prevticks1 = ticks1;

    // If left is faster, slow it down and speed up right
    if ( diff_ticks > diff_ticks1 ) {
      pwm -= motor_offset;
      pwm1 += motor_offset;
    }

    // If right is faster, slow it down and speed up left
    else if ( diff_ticks < diff_ticks1 ) {
      pwm += motor_offset;
      pwm1 -= motor_offset;
    }
    
    setmotor(1,pwm,pwm1); 
  }
