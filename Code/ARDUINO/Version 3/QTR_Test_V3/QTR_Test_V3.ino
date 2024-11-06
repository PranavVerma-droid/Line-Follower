#include <QTRSensors.h>
#include <U8g2lib.h>
QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

#define Kp 2 
#define Kd 40
#define MaxSpeed 200
#define BaseSpeed 150
#define SpeedOnTurn 100

#define SDA_PIN 11
#define SCL_PIN 12

//int STBY = 10; 

int PWMA = 5;
int AIN1 = 6;
int AIN2 = 7;

int PWMB = 3;
int BIN1 = 9;
int BIN2 = 8;

int lastError=0;

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL_PIN, /* data=*/ SDA_PIN, /* reset=*/ U8X8_PIN_NONE);

void setup() {   
    u8g2.begin(); u8g2.clearBuffer(); u8g2.setFont(u8g2_font_6x12_tr); u8g2.drawStr(0, 16, "Display Initialization Complete."); u8g2.sendBuffer();
    delay(2000);

    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6,A7}, SensorCount);
    // qtr.setEmitterPin(2); pinMode(STBY, OUTPUT);

    pinMode(PWMA, OUTPUT); pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
    pinMode(PWMB, OUTPUT); pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);

    delay(500); pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, HIGH);

    u8g2.clearBuffer(); u8g2.drawStr(0, 16, "Calibrating Sensors..."); u8g2.sendBuffer(); 
    delay(3000);

    int i;
    for (int i = 0; i < 100; i++)
    {   
       /* if ( i  < 25 || i >= 75 ) 
        {   move(0,SpeedOnTurn, 1);
            move(1,SpeedOnTurn, 0);
        }
        else
        {   move(0,SpeedOnTurn, 0);
            move(1,SpeedOnTurn, 1);
        } */
        qtr.calibrate(); delay(20);
    }
    u8g2.clearBuffer(); u8g2.drawStr(0, 16, "Calibration Complete."); u8g2.sendBuffer(); 
    delay(3000); 
}  

void loop()
{   
    uint16_t position = qtr.readLineBlack(sensorValues);
    if(position>6700) {   
        move(1, SpeedOnTurn, 1);move(0, SpeedOnTurn, 0); 
        return;    
    }
    if (position<300) {  
        move(1, SpeedOnTurn, 0);move(0, SpeedOnTurn, 1); 
        return;
    }
    int error = position - 3500;
    int motorSpeed = Kp * error + Kd * (error - lastError);
    lastError = error;

    int rightMotorSpeed = BaseSpeed + motorSpeed;
    int leftMotorSpeed = BaseSpeed - motorSpeed;
  
    if (rightMotorSpeed > MaxSpeed ) rightMotorSpeed = MaxSpeed; 
    if (leftMotorSpeed > MaxSpeed ) leftMotorSpeed = MaxSpeed;
    if (rightMotorSpeed < 0)rightMotorSpeed = 0;    
    if (leftMotorSpeed < 0)leftMotorSpeed = 0;
    
    move(1, rightMotorSpeed, 1);
    move(0, leftMotorSpeed, 1);
}

void move(int motor, int speed, int direction)
{   
    boolean inPin1=HIGH, inPin2=LOW;
    if (direction == 1) {inPin1 = HIGH;inPin2 = LOW;}  
    if (direction == 0) {inPin1 = LOW; inPin2 = HIGH;}
    if (motor == 0) {   
        digitalWrite(AIN1, inPin1);digitalWrite(AIN2, inPin2);analogWrite(PWMA, speed);
    }
    if (motor == 1) {   
        digitalWrite(BIN1, inPin1);digitalWrite(BIN2, inPin2);analogWrite(PWMB, speed);
    }  
}
