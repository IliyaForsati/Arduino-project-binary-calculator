#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0X27, 20, 4);

const int zero_but = 2;
const int one_but = 3;
const int left_parentheses_but = 4;
const int right_paranthese_but = 5;
const int add_but = 7;
const int sub_but = 6;
const int mul_but = 8;
const int div_but = 9;
const int equal_but = 10;
const int backspace_but = 11;
int laststate[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void setup() {
    lcd.begin(20, 4);
    lcd.backlight();
    lcd.setCursor(0, 0);

    for (int i = 0; i < 12; i++) {
      pinMode(i, INPUT_PULLUP);
    }
}

void zero_func() {
    // lcd.clear();
    lcd.print("0");
}
void one_func() {
    // lcd.clear();
    lcd.print("1");
}
void left_par_func() {
    // lcd.clear();
    lcd.print("(");
}
void right_par_func() {
    // lcd.clear();
    lcd.print(")");
}
void add_func() {
    // lcd.clear();
    lcd.print("+");
}
void sub_func() {
    // lcd.clear();
    lcd.print("-");
}
void mul_func() {
    // lcd.clear();
    lcd.print("x");
}
void div_func() {
    // lcd.clear();
    lcd.print("/");
}
void equal_func() {
    // lcd.clear();
    lcd.print("=");
}
void backspace_func() {
    // lcd.clear();
    lcd.print("Backspace");
}

void loop() {
    for (int i = 2; i < 12; i++) {
        int currentstate = digitalRead(i);
        if (currentstate == LOW && laststate[i] == HIGH) {
        switch (i) {
          case zero_but:
            zero_func();
          break;
          case one_but:
            one_func();
          break;
          case left_parentheses_but:
            left_par_func();
          break;
          case right_paranthese_but:
            right_par_func();
          break;
          case add_but:
            add_func();
          break;
          case sub_but:
            sub_func();
          break;
          case mul_but:
            mul_func();
          break;
          case div_but:
            div_func();
          break;
          case equal_but:
            equal_func();
          break;
          case backspace_but:
            backspace_func();
          break;
        }
      }
      laststate[i] = currentstate;
    } 
}
