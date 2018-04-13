#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
//int alpha = 1;

#define red 0xE11B1B
#define blue 0x1D1257
#define black 0x000000
#define green 0x031707

#define WAIT  0
#define READ  1
#define SEND  2

int test;
int row;
int column;
int z;
int DifChar = 0;
int x;
int y;
int count;

int state = WAIT;

//String thestring = "";
char thestring[100] = "";

char character_list[61][7] = {

  {0, 0, 0, 0, 0, 0, 0}, //SPACE
  {0, 0, 125, 0, 0, 0, 0},       //!
  {0, 96, 16, 96, 16, 0, 0},     // "
  {0, 20, 127, 20, 127, 20, 0},  //#
  {0, 0, 0, 0, 0, 0, 0}, //NUL
  {0, 50, 52, 8, 22, 38, 0},     //%
  {0, 0, 0, 0, 0, 0, 0}, //NUL
  {0, 96, 16, 0, 0, 0, 0},       //'
  {0, 0, 62, 65, 0, 0, 0},      // (
  {0, 0, 0, 65, 62, 0, 0},      // )
  {0, 0, 0, 0, 0, 0, 0}, //NUL
  {0, 8, 8, 62, 8, 8, 0},       // +
  {0, 8, 8, 8, 8, 8, 0},        // -
  {0, 3, 3, 0, 0, 0, 0},        // .
  {62, 65, 65, 65, 62, 0, 0},    //0
  {0, 33, 127, 1, 0, 0, 0},      //1
  {35, 69, 73, 73, 49, 0, 0},    //2
  {65, 65, 73, 73, 54, 0, 0},    //3
  {12, 20, 36, 127, 4, 0, 0},    //4
  {113, 73, 73, 73, 70, 0, 0},   //5
  {62, 73, 73, 73, 38, 0, 0},    //6
  {64, 64, 71, 72, 112, 0, 0},   //7
  {54, 73, 73, 73, 54, 0, 0},    //8
  {50, 73, 73, 73, 62, 0, 0},    //9
  {0, 0, 0, 54, 0, 0, 0},        // :
  {0, 0, 1, 54, 0, 0, 0},        // ;
  {0, 0, 8, 20, 34, 0, 0},       // <
  {0, 54, 54, 54, 54, 54, 0},    // =
  {0, 0, 34, 20, 8, 0, 0},       //>
  {48, 64, 69, 72, 48, 0, 0},    // ?
  // NUL
  {63, 72, 72, 72, 63, 0, 0},   //A
  {127, 73, 73, 73, 54, 0, 0},  //B
  {62, 65, 65, 65, 34, 0, 0},    //C
  {127, 65, 65, 65, 62, 0, 0},   //D
  {127, 73, 73, 73, 65, 0, 0},   //E
  {127, 72, 72, 72, 64, 0, 0},   //F
  {62, 65, 73, 73, 46, 0, 0},    //G
  {127, 8, 8, 8, 127, 0, 0},     //H
  {0, 65, 127, 65, 0, 0, 0},     //I
  {66, 65, 65, 65, 126, 0, 0},   //J
  {127, 8, 20, 34, 65, 0, 0},    //K
  {127, 1, 1, 1, 1, 0, 0},       //L
  {127, 32, 16, 32, 127, 0, 0},  //M
  {127, 16, 8, 4, 127, 0, 0},    //N
  {62, 65, 65, 65, 62, 0, 0},    //O
  {127, 72, 72, 72, 48, 0, 0},   //P
  {62, 65, 69, 66, 61, 0, 0},    //Q
  {127, 72, 76, 74, 49, 0, 0},   //R
  {49, 73, 73, 73, 70, 0, 0},    //S
  {32, 32, 127, 32, 32, 0, 0},   //T
  {126, 1, 1, 1, 126, 0, 0},     //U
  {124, 2, 1, 2, 124, 0, 0},     //V
  {126, 1, 6, 1, 126, 0, 0},     //W
  {99, 20, 8, 20, 99, 0, 0},     //X
  {96, 16, 15, 16, 96, 0, 0},    //Y
  {67, 69, 73, 81, 97, 0, 0},    //Z
  {0, 0, 0, 0, 0, 0, 0},         //SPACE
  {0, 0, 125, 0, 0, 0, 0},       //!
  {48, 64, 69, 72, 48, 0, 0},    //?
  {0, 3, 3, 0, 0, 0, 0},         // .
  {0, 5, 6, 0, 0, 0, 0},        // ,
};


SoftwareSerial mySerial(10, 11); //RX, TX

Adafruit_NeoPixel pixelStrings[] = {
  Adafruit_NeoPixel(8, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, 7, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, 8, NEO_GRB + NEO_KHZ800),
};

#define NUMSTRIPS (sizeof(pixelStrings)/sizeof(pixelStrings[0]))

void clearScreen()
{
  for (int row = 0; row < 7; row ++)
  {
    for (int column = 0; column <= 7; column++)
    {
      pixelStrings[row].setPixelColor(column, black);
      pixelStrings[row].show();
    }
  }
}

void call_char(int alpha, int multi)
{
  long color;
  for (column = 0; column < 5; column ++)
  {
    for (row = 0; row <= 6; row ++)
    {
      if (bitRead(character_list[alpha][column], row)) {
        color = green;
      }
      else
      {
        color = black;
      }
      pixelStrings[row].setPixelColor(column + 5 * multi, color);
      pixelStrings[row].show();
    }
  }
}

void call_text(int text, int column, int shift)
{
  int color;
  for (int col = 0; col < 5; col++) {
    for (int row = 0; row < 7; row++) {
      if (bitRead(character_list[text][col], row))
        color = green;
      else
        color = black;
      pixelStrings[row].setPixelColor(col + (5 * column - shift), color);
      pixelStrings[row].show();
    }
  }
}

void text_move(char str[]) {
  Serial.println(strlen(str));
  for (int shift = 0; shift < strlen(str) * 6; shift++) {
    for (int k = 0; k < strlen(str); k++) {
      int letter = str[k];
      if (isLowerCase(letter)) {
        letter -= 32;
      }
      if (isSpace(letter)) {
        letter = 0;
      }

      else {
        //letter -= 65;
        letter -= 51;
      }
      call_text(letter, k, shift);
    }
    delay(100);
    clearScreen();
  }


}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  mySerial.begin(4800);
  for (int i = 0; i <= 7; i++)
  {
    pixelStrings[i].begin();
    pixelStrings[i].show(); // Initialize all pixels to 'off'
  }
}
void loop() {

  switch (state) {
      char inChar;
    case WAIT:
      if (mySerial.available()) {
        inChar = mySerial.read();
        if (inChar == '{') {
          count = 0;
          state = READ;
        }
      }
      break;
    case READ:
      if (mySerial.available()) {
        inChar = mySerial.read();
        thestring[count++] = inChar;
        if (inChar == '}') {
          thestring[--count] = '\r';
          state = SEND;
        }
      }
      break;
    case SEND:
      Serial.print(strlen(thestring));
      Serial.print(thestring);
      // text_move("DOG");
      text_move(thestring);
      state = WAIT;
      break;
  }
  // delay(100);
}

