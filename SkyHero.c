#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <softTone.h>
#include <wiringPiI2C.h>

//gpio 핀 정의
#define TRIG 4   //16
#define ECHO 5   //18
#define SERVO 1  //12
#define BUZZER 2 //13
#define LED 6    //22

//LCD에 대한 I2C 통신 정의
#define I2C_ADDR 0x27
#define LCD_CHR 1
#define LCD_CMD 0
#define LINE1 0x80
#define LINE2 0xC0
#define LCD_BACKLIGHT 0x08
#define ENABLE 0b00000100

//LCD 관련 함수 선언
void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);

void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line);
void ClrLcd(void);
void typeln(const char* s);
void typeChar(char val);
int fd;

//초기 설정
void setup() {
    wiringPiSetup();
    softPwmCreate(SERVO, 0, 200); // 서보모터
    pinMode(BUZZER, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(LED, OUTPUT); // LED
}

//비상 상황 발생 시 경보음을 울리는 함수
//k는 거리에 반비례하여, 거리가 가까워질수록 고주파수의 경보음을 발생시킴
void warning(int k) {
    int j;
    for (j = 0; j < 3; j++) {
        digitalWrite(LED, HIGH);
        delay(10);
        softToneWrite(BUZZER, 100 * k);
        delay(30);
        digitalWrite(LED, LOW);
        delay(30);
        softToneWrite(BUZZER, 200*k);
        delay(30);
        digitalWrite(LED, HIGH);
        delay(30);
        softToneWrite(BUZZER, 300*k);
        delay(30);
        digitalWrite(LED, LOW);
        delay(30);
        softToneWrite(BUZZER, 200*k);
        delay(30);
        digitalWrite(LED, HIGH);
        delay(30);
        softToneWrite(BUZZER, 100 * k);
        delay(30);
        digitalWrite(LED, LOW);
        delay(30);       
    }

}

int main() {
    setup();
    printf("Set up Successful!!\n");
    if (wiringPiSetup() == -1) exit(1);
    fd = wiringPiI2CSetup(I2C_ADDR);
	
//lcd초기화
    lcd_init();

    int j;
    int dir = 1;
    int pos = 0;

    while (1) {
        //서보모터 (0,360) 범위
        pos += dir;
        if (pos < 0 || pos > 360) dir *= -1;

        //안전한 상황의 기본 문구 출력
        lcdLoc(LINE1);
        typeln("Peaceful!");
        lcdLoc(LINE2);
        typeln("We Are Happy!");

        //서보모터 동작
        softPwmWrite(SERVO, pos);
        delay(30);

        //초음파
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        while (digitalRead(ECHO) == LOW);
        long startTime = micros();
        while (digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;
        int distance = travelTime / 58;
        //distance 화면에 표시

        printf("Distance : %d\n", distance);
        if (distance <= 11) { //만일 11 이하면
            //적의 위치를 lcd에 띄운다.
            ClrLcd();
            lcdLoc(LINE1);
            typeln("Distance :");
            typeInt(distance);
            lcdLoc(LINE2);
            typeln("Enemies at ");
            typeInt(pos);
            typeln("degree!");
            int k;
            //경보음과 led를 작동시킨다.
            softToneCreate(BUZZER);
            if (distance >= 10) {
                warning(1);
                           }
            else if (distance >= 7) {
                warning(2);
            }
            else if (distance >= 6) {
                warning(3);
            }
            else if (distance >= 5) {
                warning(4);
            }
            else if (distance >= 4) {
                warning(5);
            }
            else if (distance >= 3) {
                warning(6);
            }
            else {
                warning(6);
            }
            softToneWrite(BUZZER, 0);
            delay(300);
            //부저를 끄고 500딜레이
            //다시 평화가 찾아왔다.
            ClrLcd();
            lcdLoc(LINE1);
            typeln("Peaceful!");
            lcdLoc(LINE2);
            typeln("We are happy!!");
        }
    }

    return 0;
}

//LCD 관련 함수들
void typeFloat(float myFloat) {
    char buffer[20];
    sprintf(buffer, "%4.2f", myFloat);
    typeln(buffer);
}

void typeInt(int i) {
    char array1[20];
    sprintf(array1, "%d", i);
    typeln(array1);
}

void ClrLcd(void) {
    lcd_byte(0x01, LCD_CMD);
    lcd_byte(0x02, LCD_CMD);
}

void lcdLoc(int line) {
    lcd_byte(line, LCD_CMD);
}

void typeChar(char val) {
    lcd_byte(val, LCD_CHR);
}

void typeln(const char* s) {
    while (*s) lcd_byte(*(s++), LCD_CHR);
}

void lcd_byte(int bits, int mode) {
    int bits_high;
    int bits_low;

    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    wiringPiI2CReadReg8(fd, bits_high);
    lcd_toggle_enable(bits_high);

    wiringPiI2CReadReg8(fd, bits_low);
    lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits) {
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits | ~ENABLE));
    delayMicroseconds(500);
}

void lcd_init() {
    lcd_byte(0x33, LCD_CMD);
    lcd_byte(0x32, LCD_CMD);
    lcd_byte(0x06, LCD_CMD);
    lcd_byte(0x0C, LCD_CMD);
    lcd_byte(0x28, LCD_CMD);
    lcd_byte(0x01, LCD_CMD);
    delayMicroseconds(500);
}