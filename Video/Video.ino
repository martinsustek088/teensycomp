const int ledPin = 13;

IntervalTimer videoTimer;
const int videoSyncPin = 14;
const int videoRedPin = 18;
const int videoGreenPin = 19;
const int videoBluePin = 17;
const int videoIntensityPin = 16;

uint16_t videoCurrentScanlineNumber = 1;

uint32_t videoBuffer[(320 * 4 / 32) * 200 + 40];
uint32_t videoBufferEmptyLine[40];

uint16_t videoTextPosX = 0;
uint8_t videoTextPosY = 0;
uint8_t videoColorForeground = 0xE;
uint8_t videoColorBackground = 0x0;
uint8_t videoInterlaced = 1;

unsigned char videoTextFont[][6] PROGMEM = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// sp 
  {0x00, 0x00, 0x00, 0x2f, 0x00, 0x00},// ! 
  {0x00, 0x00, 0x07, 0x00, 0x07, 0x00},// " 
  {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14},// # 
  {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12},// $ 
  {0x00, 0x62, 0x64, 0x08, 0x13, 0x23},// % 
  {0x00, 0x36, 0x49, 0x55, 0x22, 0x50},// & 
  {0x00, 0x00, 0x05, 0x03, 0x00, 0x00},// ' 
  {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00},// ( 
  {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00},// ) 
  {0x00, 0x14, 0x08, 0x3E, 0x08, 0x14},// * 
  {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08},// + 
  {0x00, 0x00, 0x00, 0xA0, 0x60, 0x00},// , 
  {0x00, 0x08, 0x08, 0x08, 0x08, 0x08},// - 
  {0x00, 0x00, 0x60, 0x60, 0x00, 0x00},// . 
  {0x00, 0x20, 0x10, 0x08, 0x04, 0x02},// / 
  {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E},// 0 
  {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00},// 1 
  {0x00, 0x42, 0x61, 0x51, 0x49, 0x46},// 2 
  {0x00, 0x21, 0x41, 0x45, 0x4B, 0x31},// 3 
  {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10},// 4 
  {0x00, 0x27, 0x45, 0x45, 0x45, 0x39},// 5 
  {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30},// 6 
  {0x00, 0x01, 0x71, 0x09, 0x05, 0x03},// 7 
  {0x00, 0x36, 0x49, 0x49, 0x49, 0x36},// 8 
  {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E},// 9 
  {0x00, 0x00, 0x36, 0x36, 0x00, 0x00},// : 
  {0x00, 0x00, 0x56, 0x36, 0x00, 0x00},// ; 
  {0x00, 0x08, 0x14, 0x22, 0x41, 0x00},// < 
  {0x00, 0x14, 0x14, 0x14, 0x14, 0x14},// = 
  {0x00, 0x00, 0x41, 0x22, 0x14, 0x08},// > 
  {0x00, 0x02, 0x01, 0x51, 0x09, 0x06},// ? 
  {0x00, 0x32, 0x49, 0x59, 0x51, 0x3E},// @ 
  {0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C},// A 
  {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36},// B 
  {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22},// C 
  {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C},// D 
  {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41},// E 
  {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01},// F 
  {0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A},// G 
  {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F},// H 
  {0x00, 0x00, 0x41, 0x7F, 0x41, 0x00},// I 
  {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01},// J 
  {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41},// K 
  {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40},// L 
  {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F},// M 
  {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F},// N 
  {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E},// O 
  {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06},// P 
  {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E},// Q 
  {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46},// R 
  {0x00, 0x46, 0x49, 0x49, 0x49, 0x31},// S 
  {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01},// T 
  {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F},// U 
  {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F},// V 
  {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F},// W 
  {0x00, 0x63, 0x14, 0x08, 0x14, 0x63},// X 
  {0x00, 0x07, 0x08, 0x70, 0x08, 0x07},// Y 
  {0x00, 0x61, 0x51, 0x49, 0x45, 0x43},// Z 
  {0x00, 0x00, 0x7F, 0x41, 0x41, 0x00},// [ 
  {0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55},// 55 
  {0x00, 0x00, 0x41, 0x41, 0x7F, 0x00},// ] 
  {0x00, 0x04, 0x02, 0x01, 0x02, 0x04},// ^ 
  {0x00, 0x40, 0x40, 0x40, 0x40, 0x40},// _ 
  {0x00, 0x00, 0x01, 0x02, 0x04, 0x00},// ' 
  {0x00, 0x20, 0x54, 0x54, 0x54, 0x78},// a 
  {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38},// b 
  {0x00, 0x38, 0x44, 0x44, 0x44, 0x20},// c 
  {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F},// d 
  {0x00, 0x38, 0x54, 0x54, 0x54, 0x18},// e 
  {0x00, 0x08, 0x7E, 0x09, 0x01, 0x02},// f 
  {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C},// g 
  {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78},// h 
  {0x00, 0x00, 0x44, 0x7D, 0x40, 0x00},// i 
  {0x00, 0x40, 0x80, 0x84, 0x7D, 0x00},// j 
  {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00},// k 
  {0x00, 0x00, 0x41, 0x7F, 0x40, 0x00},// l 
  {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78},// m 
  {0x00, 0x7C, 0x08, 0x04, 0x04, 0x78},// n 
  {0x00, 0x38, 0x44, 0x44, 0x44, 0x38},// o 
  {0x00, 0xFC, 0x24, 0x24, 0x24, 0x18},// p 
  {0x00, 0x18, 0x24, 0x24, 0x18, 0xFC},// q 
  {0x00, 0x7C, 0x08, 0x04, 0x04, 0x08},// r 
  {0x00, 0x48, 0x54, 0x54, 0x54, 0x20},// s 
  {0x00, 0x04, 0x3F, 0x44, 0x40, 0x20},// t 
  {0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C},// u 
  {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C},// v 
  {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C},// w 
  {0x00, 0x44, 0x28, 0x10, 0x28, 0x44},// x 
  {0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C},// y 
  {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44},// z 
  {0x14, 0x14, 0x14, 0x14, 0x14, 0x14},// horiz lines 
};



inline void videoDelay(uint32_t n) {
  __asm__ volatile(
    "L_%=_videoDelay:"                      "\n\t"
    "subs   %0, #1"                         "\n\t"
    "bne    L_%=_videoDelay"                "\n"
    : "+r" (n) :
  );
}

inline void videoDrawScanlineLongPulses() {
  digitalWriteFast(videoSyncPin, LOW);
  videoDelay(875); // 27.3us
  digitalWriteFast(videoSyncPin, HIGH);
  videoDelay(150); // 4.7us
  digitalWriteFast(videoSyncPin, LOW);
  videoDelay(875); // 27.3us
  digitalWriteFast(videoSyncPin, HIGH);
}

inline void videoDrawScanlineShortPulses() {
  digitalWriteFast(videoSyncPin, LOW);
  videoDelay(75); // 2.35us
  digitalWriteFast(videoSyncPin, HIGH);
  videoDelay(950); // 29.65us
  digitalWriteFast(videoSyncPin, LOW);
  videoDelay(75); // 2.35us
  digitalWriteFast(videoSyncPin, HIGH);
}

inline void videoDrawScanlineBothPulses() {
  digitalWriteFast(videoSyncPin, LOW);
  videoDelay(875); // 27.3us
  digitalWriteFast(videoSyncPin, HIGH);
  videoDelay(150); // 4.7us
  digitalWriteFast(videoSyncPin, LOW);
  videoDelay(75); // 2.35us
  digitalWriteFast(videoSyncPin, HIGH);
}

inline void videoDrawScanlineImage(uint16_t imageLine) {
  // sync
  digitalWriteFast(videoSyncPin, LOW);
  videoDelay(150); // 4.7us
  
  // back porch
  digitalWriteFast(videoSyncPin, HIGH);
  videoDelay(183); // 5.7us
  
  // text-unsafe
  videoDelay(165); // 5.175us
  
  // picture
  if (imageLine > 200) {
    imageLine = 200;
  }
  uint32_t *pixelPtr;
  uint32_t *endPtr;
  pixelPtr = videoBuffer + (imageLine * 40);
  endPtr = pixelPtr + 40;
  pixelPtr--;
  while (++pixelPtr < endPtr) {
    GPIOB_PDOR = (*pixelPtr) >> 28;
    asm("NOP"); // 0.208us*/
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    GPIOB_PDOR = (*pixelPtr) >> 24;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    GPIOB_PDOR = (*pixelPtr) >> 20;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    GPIOB_PDOR = (*pixelPtr) >> 16;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    GPIOB_PDOR = (*pixelPtr) >> 12;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    GPIOB_PDOR = (*pixelPtr) >> 8;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    GPIOB_PDOR = (*pixelPtr) >> 4;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    GPIOB_PDOR = (*pixelPtr) >> 0;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }
  
  // text-unsafe & front porch (end of line)
  digitalWriteFast(videoRedPin, LOW);
  digitalWriteFast(videoGreenPin, LOW);
  digitalWriteFast(videoBluePin, LOW);
  digitalWriteFast(videoIntensityPin, LOW);
}

void videoDrawScanline() {
  noInterrupts();
  
  /*  if (videoCurrentScanlineNumber <= 5) {
    if (videoCurrentScanlineNumber < 3) {
      videoDrawScanlineLongPulses();
    } else if (videoCurrentScanlineNumber > 3) {
      videoDrawScanlineShortPulses();
    } else {
      videoDrawScanlineBothPulses();
    }
  } else if (videoCurrentScanlineNumber >= 310) {
      videoDrawScanlineShortPulses();
  } else {
      videoDrawScanlineImage(videoCurrentScanlineNumber - 70);
  }*/
  
  
  switch (videoCurrentScanlineNumber) {
    case 1:
    case 2:
      videoDrawScanlineLongPulses();
      break;
    case 3:
      videoDrawScanlineBothPulses();
      break;
    case 4:
    case 5:
    case 310:
    case 311:
    case 312:
      videoDrawScanlineShortPulses();
      break;
      
    case 313:
    case 314:
      videoDrawScanlineLongPulses();
      break;
    case 315:
      videoDrawScanlineBothPulses();
      break;
    case 316:
    case 317:
    case 623:
    case 624:
    case 625:
      videoDrawScanlineShortPulses();
      break;
      
    default:
      videoDrawScanlineImage(videoCurrentScanlineNumber - 70);
      break;
  }
  
  videoCurrentScanlineNumber++;
  if (((videoCurrentScanlineNumber > 625)) || (videoCurrentScanlineNumber > 312) && (!videoInterlaced)) {
    videoCurrentScanlineNumber = 1;
  }
  
  interrupts();
}

void videoClearScreen() {
  for (uint16_t i = 0; i < 8000 + 40; i++) {
    videoBuffer[i] = 0;
  }
}

void videoTestPattern() {
  for (uint16_t i = 0; i < 8000; i++) {
    videoBuffer[i] = 0x0000E000;
    if (((i / 40) % 8) == 4) {
      videoBuffer[i] = 0xEEEEEEEE;
    }
 /*   if (i > 4000) {
      videoBuffer[i] = 0xE0E0E0E0;
      if (((i / 40) % 2) == 0) {
        videoBuffer[i] = 0x0E0E0E0E;
      }
    }*/
    //videoBuffer[i] = 0xE0E0E0E0;
  }
}



void videoTextSetPos(uint16_t x, uint8_t y) {
  videoTextPosX = x;
  videoTextPosY = y;
}

void videoTextNextLine() {
  videoTextPosX = 0;
  videoTextPosY += 8;
  if (videoTextPosY >= 200) {
    videoTextPosY = 0; // TODO: scroll instead?
  }
}

void videoTextNextChar() {
  videoTextPosX += 6;
  if (videoTextPosX >= 318) {
    videoTextNextLine();
  }
}

void videoSetPixel(uint16_t x, uint8_t y, uint8_t color) {
  uint16_t address = 40 * y + x / 8;
  uint8_t shift = (7 - (x % 8)) * 4;
  videoBuffer[address] &= ~(((uint32_t)0xF) << shift);
  videoBuffer[address] |= ((uint32_t)(color & 0xF)) << shift;
}

void videoTextDrawChar(const char ch) {
  char id = ch - 32;
  for (uint8_t column = 0; column < 6; column++) {
    uint8_t data = pgm_read_byte(&(videoTextFont[id][column]));
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t color = videoColorBackground;
      if ((data >> row) & 1) {
        color = videoColorForeground;
      }
      videoSetPixel(videoTextPosX + column, videoTextPosY + row, color);
    }
  }
}

void videoTextWriteChar(const char ch) {
  if (ch == '\n') {
    videoTextNextLine();
  } else {
    videoTextDrawChar(ch);
    videoTextNextChar();
  }
}

void videoTextWriteString(const char* ch) {
  while (*ch != 0) {
    videoTextWriteChar(*ch);
    ch++;
  }  
}


void setup() {
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(videoSyncPin, OUTPUT);
  pinMode(videoRedPin, OUTPUT);
  pinMode(videoGreenPin, OUTPUT);
  pinMode(videoBluePin, OUTPUT);
  pinMode(videoIntensityPin, OUTPUT);
  
  
//  asm("MRC p15, 0, r1, c1, c0, 1");
//asm("ORR r1, r1, #0b00000000001100101000000000000000");
//  asm("ORR r1, r1, #0b00000000000000000000000000000000");
//  asm("MCR p15, 0, r1, c1, c0, 1");
//  C1 |= ((1 << 21) | (1 << 20) | (1 << 17) | (1 << 15));
  
  videoClearScreen();
  videoTimer.begin(videoDrawScanline, 64);
  
  videoTestPattern();
  videoTextWriteString("Hello everybody!\nThis is amazing!\n");
  for (uint8_t i = 0; i < 16; i++) {
    videoColorForeground = i;
    videoTextWriteString("#");
  }
//  videoTimer.priority(0);
  
//  Serial.begin(9600);
}

elapsedMillis timer;

void loop() {
  videoTextPosX = 150;
  videoTextPosY = 0;
  videoTextDrawChar('0' + (timer / 10000000) % 10);
  videoTextNextChar();
  videoTextDrawChar('0' + (timer / 1000000) % 10);
  videoTextNextChar();
  videoTextDrawChar('0' + (timer / 100000) % 10);
  videoTextNextChar();
  videoTextDrawChar('0' + (timer / 10000) % 10);
  videoTextNextChar();
  videoTextDrawChar('0' + (timer / 1000) % 10);
  videoTextNextChar();
  videoTextDrawChar('.');
  videoTextNextChar();  
  videoTextDrawChar('0' + (timer / 100) % 10);
  videoTextNextChar();
  videoTextDrawChar('0' + (timer / 10) % 10);
  videoTextNextChar();
  videoTextDrawChar('0' + (timer / 1) % 10);

  /*digitalWriteFast(ledPin, HIGH);
  delay(50);
  digitalWriteFast(ledPin, LOW);
  delay(9950);*/

/*  for (uint16_t i = 0; i < 8000; i++) {
    if ((i % 40) == 0) {
      Serial.printf("\n");
    }
    Serial.printf("%8x", videoBuffer[i]);
  }*/
  
  //for(;;);
  
/*  delay(5000);
  videoInterlaced = 1;
  delay(5000);
  videoInterlaced = 0;*/
}
