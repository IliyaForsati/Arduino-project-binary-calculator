#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include "parser.h"

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

const int MAX_SIZE = 100;
char input[MAX_SIZE];
char DString[MAX_SIZE];

void setup() {
    lcd.begin(20, 4);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.blink();

    for (int i = 0; i < 12; i++) {
      pinMode(i, INPUT_PULLUP);
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        input[i] = 0;
    }
}

void creatDString() {
  char result[MAX_SIZE] = "";
  char temp[MAX_SIZE];
  int tempIndex = 0;

  for (int i = 0; true; i++) {
      if (input[i] == '0' || input[i] == '1') {
          temp[tempIndex++] = input[i];
      } else {
          if (tempIndex > 0) {
              temp[tempIndex] = '\0';
              int decimal = strtol(temp, NULL, 2);
              char decimalStr[50];
              sprintf(decimalStr, "%d", decimal);
              strcat(result, decimalStr);
              tempIndex = 0;
          }
          strncat(result, &input[i], 1);
      }

      if (input[i] == '\0') break;
  }

  strcpy(DString, result);
}

void updateUI(char str[]) {
  // todo : send str for UI
}

void updateLcd() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(DString);
  lcd.setCursor(0, 0);
  lcd.print(input);

  updateUI(DString);
}

void zero_func() {
     strcat(input, "0");
     creatDString();

     updateLcd();
}
void one_func() {
    strcat(input, "1");
    creatDString();

    updateLcd();
}
void left_par_func() {
    strcat(input, "(");
    creatDString();
    updateLcd();
}
void right_par_func() {
    strcat(input, ")");
    creatDString();
    updateLcd();
}
void add_func() {
    strcat(input, "+");
    creatDString();
    updateLcd();
}
void sub_func() {
    strcat(input, "-");
    creatDString();
    updateLcd();
}
void mul_func() {
    strcat(input, "x");
    creatDString();
    updateLcd();
}
void div_func() {
    strcat(input, "/");
    creatDString();
    updateLcd();
}
void equal_func() {
    ParseTree* tree = parse(DString);

    if ((*tree).type == SUCCESS) {
      lcd.clear();
      lcd.print((*tree).value);

      char valueStr[MAX_SIZE];
      sprintf(valueStr, "%d", (*tree).value);
      updateUI(valueStr);
    } else {
      lcd.clear();
      lcd.print("invalid format!");

      updateUI("invalid format!");
    }
}
void backspace_func() {
    int lastIndex = strlen(input) - 1;
    if (lastIndex >= 0) {
      input[lastIndex] = 0;
      creatDString();
      updateLcd();
    }
}

void loop() {
    for (int i = 2; i < 12; i++) {
        int currentstate = digitalRead(i);
        if (currentstate == LOW) {
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
    } 

    delay(225);
}

