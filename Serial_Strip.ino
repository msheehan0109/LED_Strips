#include <SoftwareSerial.h>
#define WAIT 0
#define READ 1
#define SEND 2
#define PIXELS 7
#define PIXEL_PORT  PORTD
#define PIXEL_DDR   DDRD
static const uint8_t onBits = 0b11111110;
#define T1H  814
#define T1L  438
#define T0H  312
#define T0L  936
#define RES 50000
#define NS_PER_SEC (1000000000L)
#define CYCLES_PER_SEC (F_CPU)
#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )
#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )
int count;
int state = WAIT;
static inline void sendBitx8(  const uint8_t row , const uint8_t colorbyte , const uint8_t onBits ) {
  asm volatile (
    "L_%=: \n\r"
    "out %[port], %[onBits] \n\t"
    "mov r0, %[bitwalker] \n\t"
    "and r0, %[colorbyte] \n\t"
    "breq OFF_%= \n\t"
    "nop \n\t  "
    "out %[port], %[row]   \n\t"
    "nop \n\t nop \n\t "
    "nop \n\t nop \n\t "
    "nop \n\t nop \n\t "
    "nop \n\t "
    "out %[port], __zero_reg__ \n\t"
    "ror %[bitwalker] \n\t"
    "brcs DONE_%= \n\t"
    "nop \n\t \n\t "
    "jmp L_%= \n\t"
    "OFF_%=: \n\r"
    "out %[port], __zero_reg__ \n\t"
    "ror %[bitwalker] \n\t"
    "brcs DONE_%= \n\t"
    "nop \n\t nop \n\t "
    "nop \n\t nop \n\t "
    "nop \n\t nop \n\t "
    "nop \n\t nop \n\t "
    "nop \n\t "
    "jmp L_%= \n\t"
    "DONE_%=: \n\t"

    ::
    [port]    "I" (_SFR_IO_ADDR(PIXEL_PORT)),
    [row]   "d" (row),
    [onBits]   "d" (onBits),
    [colorbyte]   "d" (colorbyte ),
    [bitwalker] "r" (0x80)

  );

}

void show() {
  delayMicroseconds( (RES / 1000UL) + 1);
}

static inline void sendRowRGB( uint8_t row ,  uint8_t r,  uint8_t g,  uint8_t b ) {

  sendBitx8( row , g , onBits);
  sendBitx8( row , r , onBits);
  sendBitx8( row , b , onBits);

}

#define FONT_WIDTH 5
#define INTERCHAR_SPACE 1
#define ASCII_OFFSET 0x20

const uint8_t Font5x7[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, //
  0x00, 0x00, 0xfa, 0x00, 0x00, // !
  0x00, 0xe0, 0x00, 0xe0, 0x00, // "
  0x28, 0xfe, 0x28, 0xfe, 0x28, // #
  0x24, 0x54, 0xfe, 0x54, 0x48, // $
  0xc4, 0xc8, 0x10, 0x26, 0x46, // %
  0x6c, 0x92, 0xaa, 0x44, 0x0a, // &
  0x00, 0xa0, 0xc0, 0x00, 0x00, // '
  0x00, 0x38, 0x44, 0x82, 0x00, // (
  0x00, 0x82, 0x44, 0x38, 0x00, // )
  0x10, 0x54, 0x38, 0x54, 0x10, // *
  0x10, 0x10, 0x7c, 0x10, 0x10, // +
  0x00, 0x0a, 0x0c, 0x00, 0x00, // ,
  0x10, 0x10, 0x10, 0x10, 0x10, // -
  0x00, 0x06, 0x06, 0x00, 0x00, // .
  0x04, 0x08, 0x10, 0x20, 0x40, // /
  0x7c, 0x8a, 0x92, 0xa2, 0x7c, // 0
  0x00, 0x42, 0xfe, 0x02, 0x00, // 1
  0x42, 0x86, 0x8a, 0x92, 0x62, // 2
  0x84, 0x82, 0xa2, 0xd2, 0x8c, // 3
  0x18, 0x28, 0x48, 0xfe, 0x08, // 4
  0xe4, 0xa2, 0xa2, 0xa2, 0x9c, // 5
  0x3c, 0x52, 0x92, 0x92, 0x0c, // 6
  0x80, 0x8e, 0x90, 0xa0, 0xc0, // 7
  0x6c, 0x92, 0x92, 0x92, 0x6c, // 8
  0x60, 0x92, 0x92, 0x94, 0x78, // 9
  0x00, 0x6c, 0x6c, 0x00, 0x00, // :
  0x00, 0x6a, 0x6c, 0x00, 0x00, // ;
  0x00, 0x10, 0x28, 0x44, 0x82, // <
  0x28, 0x28, 0x28, 0x28, 0x28, // =
  0x82, 0x44, 0x28, 0x10, 0x00, // >
  0x40, 0x80, 0x8a, 0x90, 0x60, // ?
  0x4c, 0x92, 0x9e, 0x82, 0x7c, // @
  0x7e, 0x88, 0x88, 0x88, 0x7e, // A
  0xfe, 0x92, 0x92, 0x92, 0x6c, // B
  0x7c, 0x82, 0x82, 0x82, 0x44, // C
  0xfe, 0x82, 0x82, 0x44, 0x38, // D
  0xfe, 0x92, 0x92, 0x92, 0x82, // E
  0xfe, 0x90, 0x90, 0x80, 0x80, // F
  0x7c, 0x82, 0x82, 0x8a, 0x4c, // G
  0xfe, 0x10, 0x10, 0x10, 0xfe, // H
  0x00, 0x82, 0xfe, 0x82, 0x00, // I
  0x04, 0x02, 0x82, 0xfc, 0x80, // J
  0xfe, 0x10, 0x28, 0x44, 0x82, // K
  0xfe, 0x02, 0x02, 0x02, 0x02, // L
  0xfe, 0x40, 0x20, 0x40, 0xfe, // M
  0xfe, 0x20, 0x10, 0x08, 0xfe, // N
  0x7c, 0x82, 0x82, 0x82, 0x7c, // O
  0xfe, 0x90, 0x90, 0x90, 0x60, // P
  0x7c, 0x82, 0x8a, 0x84, 0x7a, // Q
  0xfe, 0x90, 0x98, 0x94, 0x62, // R
  0x62, 0x92, 0x92, 0x92, 0x8c, // S
  0x80, 0x80, 0xfe, 0x80, 0x80, // T
  0xfc, 0x02, 0x02, 0x02, 0xfc, // U
  0xf8, 0x04, 0x02, 0x04, 0xf8, // V
  0xfe, 0x04, 0x18, 0x04, 0xfe, // W
  0xc6, 0x28, 0x10, 0x28, 0xc6, // X
  0xc0, 0x20, 0x1e, 0x20, 0xc0, // Y
  0x86, 0x8a, 0x92, 0xa2, 0xc2, // Z
  0x00, 0x00, 0xfe, 0x82, 0x82, // [
  0x40, 0x20, 0x10, 0x08, 0x04, // (backslash)
  0x82, 0x82, 0xfe, 0x00, 0x00, // ]
  0x20, 0x40, 0x80, 0x40, 0x20, // ^
  0x02, 0x02, 0x02, 0x02, 0x02, // _
  0x00, 0x80, 0x40, 0x20, 0x00, // `
  0x04, 0x2a, 0x2a, 0x2a, 0x1e, // a
  0xfe, 0x12, 0x22, 0x22, 0x1c, // b
  0x1c, 0x22, 0x22, 0x22, 0x04, // c
  0x1c, 0x22, 0x22, 0x12, 0xfe, // d
  0x1c, 0x2a, 0x2a, 0x2a, 0x18, // e
  0x10, 0x7e, 0x90, 0x80, 0x40, // f
  0x10, 0x28, 0x2a, 0x2a, 0x3c, // g
  0xfe, 0x10, 0x20, 0x20, 0x1e, // h
  0x00, 0x22, 0xbe, 0x02, 0x00, // i
  0x04, 0x02, 0x22, 0xbc, 0x00, // j
  0x00, 0xfe, 0x08, 0x14, 0x22, // k
  0x00, 0x82, 0xfe, 0x02, 0x00, // l
  0x3e, 0x20, 0x18, 0x20, 0x1e, // m
  0x3e, 0x10, 0x20, 0x20, 0x1e, // n
  0x1c, 0x22, 0x22, 0x22, 0x1c, // o
  0x3e, 0x28, 0x28, 0x28, 0x10, // p
  0x10, 0x28, 0x28, 0x18, 0x3e, // q
  0x3e, 0x10, 0x20, 0x20, 0x10, // r
  0x12, 0x2a, 0x2a, 0x2a, 0x04, // s
  0x20, 0xfc, 0x22, 0x02, 0x04, // t
  0x3c, 0x02, 0x02, 0x04, 0x3e, // u
  0x38, 0x04, 0x02, 0x04, 0x38, // v
  0x3c, 0x02, 0x0c, 0x02, 0x3c, // w
  0x22, 0x14, 0x08, 0x14, 0x22, // x
  0x30, 0x0a, 0x0a, 0x0a, 0x3c, // y
  0x22, 0x26, 0x2a, 0x32, 0x22, // z
  0x00, 0x10, 0x6c, 0x82, 0x00, // {
  0x00, 0x00, 0xfe, 0x00, 0x00, // |
  0x00, 0x82, 0x6c, 0x10, 0x00, // }
  0x10, 0x10, 0x54, 0x38, 0x10, // ~
  0x10, 0x38, 0x54, 0x10, 0x10, // 
};

SoftwareSerial mySerial(10, 11); //RX, TX
static inline void sendChar( uint8_t c ,  uint8_t skip , uint8_t r,  uint8_t g,  uint8_t b ) {

  const uint8_t *charbase = Font5x7 + (( c - ' ') * FONT_WIDTH ) ;

  uint8_t col = FONT_WIDTH;

  while (skip--) {
    charbase++;
    col--;
  }

  while (col--) {
    sendRowRGB( pgm_read_byte_near( charbase++ ) , r , g , b );
  }

  col = INTERCHAR_SPACE;

  while (col--) {

    sendRowRGB( 0 , r , g , b );

  }

}

static inline void sendString( const char *s , uint8_t skip ,  const uint8_t r,  const uint8_t g,  const uint8_t b ) {

  unsigned int l = PIXELS / (FONT_WIDTH + INTERCHAR_SPACE);

  sendChar( *s , skip ,  r , g , b );

  while ( *(++s) && l--) {

    sendChar( *s , 0,  r , g , b );

  }

  show();
}



void setup() {
  mySerial.begin(4800);

  PIXEL_DDR |= onBits;
}

static char text[] = "D";


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
   //   break;
    case READ:
      if (mySerial.available()) {
        inChar = mySerial.read();
        text[count++] = inChar;
        if (inChar == '}') {
          text[--count] = '\0';
          state = SEND;
        }
      }
      break;
    case SEND:
      const char *m = text;

      while (*m) {

        for ( uint8_t step = 0; step < FONT_WIDTH + INTERCHAR_SPACE  ; step++ ) {
          cli();
          delay(10000);
          //13000
          sendString( m , step , 0x30, 0x30 , 0x0 );

          sei();


        }

        m++;

      }
      state = WAIT;
      break;
  }
}
