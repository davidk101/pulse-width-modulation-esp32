/*
 * 
 * Description: Pulse Width Modulation on an ESP32 MCU using hardware timers to control the time, ledc routines to control the PWM and interrupts to trigger the process.
 * Author: David Kumar 
 * 
 */
 
hw_timer_t * timer = NULL;
#define interrupt_pin 0
#define  GPIOPIN  18 // display blue as the PWM LED color 

int i = 0;
bool enable = false;
unsigned long count = 0;

void IRAM_ATTR run_it() {
  enable = true;
}

int PWM_FREQUENCY = 1000; 
int PWM_CHANNEL = 0; // 0-16 channels available 
int PWM_RESOUTION = 8; 

void IRAM_ATTR onTimer(){
 count++;
}

void setup() {

  attachInterrupt( interrupt_pin, run_it, FALLING); // FLASH button on the ESP32 is the interrupt 
  Serial.begin(115200); // baud rate for the serial monitor 
  pinMode(GPIOPIN, OUTPUT);
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
  
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOUTION);
  ledcAttachPin(GPIOPIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 255);
}

void loop() {

  if (enable){
    
    count = 0;
    
    while(count < 4000){ // increasing brightness for 4 seconds
      ledcWrite(PWM_CHANNEL, map(count,4000,0,0,256));
    }

    count = 0;

    while(count <= 6000){ // holding maximum brightness for 6 seconds 
      Serial.println(count); 
    }

    count = 0;
    while(count <= 2000){ // decreasing brightness for 2 seconds 
      ledcWrite(PWM_CHANNEL, map(count,2000,0,256,0));

    }

    enable=false;

  }

}
