#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Para conexion por software
// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK) lcd:5
// pin 6 - Serial data out (DIN) lcd:4
// pin 5 - Data/Command select (D/C) lcd:3
// pin 4 - LCD chip select (CS) lcd:2
// pin 3 - LCD reset (RST) lcd:1
// Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Para conexion por hardware
// Hardware SPI (faster, but must use certain hardware pins):
// SCK en Mega es pin 52 - Serial clock out (SCLK) lcd:5
// MOSI en Mega es pin 51 - Serial Data out (DIN) lcd:4
// pin 32 - Data/Command select (D/C) lcd:3
// pin 31 - LCD chip select (CS) lcd:2
// pin 30 - LCD reset (RST) lcd:1
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

//Conectado en HARDWARE SPI en los pines de arriba
Adafruit_PCD8544 display = Adafruit_PCD8544(32, 31, 30);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

#define JUGADOR1 A14
#define JUGADOR2 A15

int juego = 0;

void setup()   {
  Serial.begin(9600);

  pinMode(JUGADOR1, INPUT);
  pinMode(JUGADOR2, INPUT);
  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(60);

  //display.display(); // show splashscreen
  //delay(2000);
  display.clearDisplay();   // clears the screen and buffer


  menu(100);


  /*
    // draw many lines
    testdrawline();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw rectangles
    testdrawrect();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw multiple rectangles
    testfillrect();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw mulitple circles
    testdrawcircle();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw a circle, 10 pixel radius
    display.fillCircle(display.width()/2, display.height()/2, 10, BLACK);
    display.display();
    delay(2000);
    display.clearDisplay();

    testdrawroundrect();
    delay(2000);
    display.clearDisplay();

    testfillroundrect();
    delay(2000);
    display.clearDisplay();

    testdrawtriangle();
    delay(2000);
    display.clearDisplay();

    testfilltriangle();
    delay(2000);
    display.clearDisplay();

    // draw the first ~12 characters in the font
    testdrawchar();
    display.display();
    delay(2000);
    display.clearDisplay();

    // text display tests
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.println("Hello, world!");
    display.setTextColor(WHITE, BLACK); // 'inverted' text
    display.println(3.141592);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.print("0x"); display.println(0xDEADBEEF, HEX);
    display.display();
    delay(2000);

    // rotation example
    display.clearDisplay();
    display.setRotation(1);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.println("Rotation");
    display.setTextSize(2);
    display.println("Example!");
    display.display();
    delay(2000);

    // revert back to no rotation
    display.setRotation(0);

    // miniature bitmap display
    display.clearDisplay();
    display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
    display.display();

    // invert the display
    display.invertDisplay(true);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);

    // draw a bitmap icon and 'animate' movement
    testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT);

  */
}

typedef struct pos {
  int x;
  int y;
  struct pos *siguiente;
};


int ballX = 42;
int ballY = 20;
int ballDX = 1;
int ballDY = 1;
int ballSpeed = 2;

int scoreP1 = 5;
int scoreP2 = 5;

int paleta1, paleta2;

int scoreDivisor = 7;
int paletaAlto = 3;



void loop() {

  switch (juego) {
    case 0: {
        //PONG
        leerPotenciometros();
        drawScores();
        drawPaletas(paleta1, paleta2);
        drawPelota();
        actualizarPelota();
        checkWinner();
        display.display();
        delay(60);
        display.clearDisplay();
        break;
      }
    case 1: {

      }
  }

}

void menu(int k) {
  for (int i = 0; i < k; i++) {
    display.setTextSize(1);
    display.setCursor(10, 10);
    int selector = analogRead(JUGADOR1);
    if (selector <= 512) {
      juego = 0;
      display.clearDisplay();
      display.setTextColor(WHITE, BLACK);
      display.println("1.- PONG");
      display.setTextColor(BLACK);
      display.println("2.- SNAKE");
      display.display();
    } else {
      juego = 1;
      display.clearDisplay();
      display.setTextColor(BLACK);
      display.println("1.- PONG");
      display.setTextColor(WHITE, BLACK);
      display.println("2.- SNAKE");
      display.display();
    }
    delay(100);
  }
}

void checkWinner() {
  if (scoreP1 == 0) {
    drawWinner("JUGADOR 2!");
  }
  if (scoreP2 == 0) {
    drawWinner("JUGADOR 1!");
  }
}

void drawWinner(char* jug) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(10, 10);
  display.println("GANADOR");
  display.println(jug);a
  display.display();
  delay(4000);
}

void drawScores() {
  for (int i = 0; i < scoreP1; i++) {
    //puntaje jugador 1
    drawRectangulo((4 * i) + 2, 2, 2, 3);
    //display.fillRect((4*i)+2, i+2, 2, 3, BLACK);
  }
  for (int i = 0; i < scoreP2; i++) {
    //puntaje jugador 2
    drawRectangulo(84 - ((4 * i) + 2), 2, 2, 3);
    //display.fillRect(84-((4*i)+2), i+2, 2, 3, BLACK);
  }

  //linea separadora
  for (int i = 0; i <= 84; i++) {
    display.drawPixel(i, 7, BLACK);
    //display.drawLine(0, 7, 84, 1, BLACK);
  }

}

void drawRectangulo(int x, int y, int w, int h) {
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      display.drawPixel(x + i, y + j, BLACK);
    }
  }
}

void leerPotenciometros() {
  int jug1 = analogRead(JUGADOR1);
  int jug2 = analogRead(JUGADOR2);
  paleta1 = map(jug1, 0, 1023, 0, 40) + 7;
  paleta2 = map(jug2, 0, 1023, 0, 40) + 7;
  Serial.print("jug1: ");
  Serial.print(paleta1);
  Serial.print(" jug2: ");
  Serial.println(paleta2);
}

void actualizarPelota() {
  if ((ballY >= 47) || (ballY <= scoreDivisor)) {
    //colision con paredes arriba y abajo
    ballDY *= -1;
  }

  if ((ballX >= 3) && (ballX <= 4)) {
    //colision paleta izquierda
    if ((ballY >= paleta1 - paletaAlto) && (ballY <= paleta1 + paletaAlto)) {
      ballDX *= -1;
    }
  }

  if ((ballX >= 79) && (ballX <= 80)) {
    //colision paleta derecha
    if ((ballY >= paleta2 - paletaAlto) && (ballY <= paleta2 + paletaAlto)) {
      ballDX *= -1;
    }
  }

  if (ballX >= 84) {
    //gol de jugador 1
    scoreP2--;
    resetGame(false);
  }

  if (ballX <= 0) {
    //gol de jugador 2
    scoreP1--;
    resetGame(false);
  }

  //mover pelota
  ballX += ballDX * ballSpeed;
  ballY += ballDY * ballSpeed;
}



void resetGame(boolean total) {
  display.clearDisplay();
  delay(1000);
  ballX = 42;
  ballY = 20;
  paleta1 = 42;
  paleta2 = 42;
  if (total) {
    scoreP1 = 5;
    scoreP2 = 5;
    ballDX = 1;
    ballDY = 1;
  }
}

void drawPelota() {
  drawCursor(ballX, ballY);
}

void drawPaletas(int pos1, int pos2) {
  //paleta izquierda
  if (pos1 - paletaAlto >= scoreDivisor) {
    display.drawPixel(3, pos1 - 3, BLACK);
    display.drawPixel(3, pos1 - 2, BLACK);
    display.drawPixel(3, pos1 - 1, BLACK);

    display.drawPixel(4, pos1 - 3, BLACK);
    display.drawPixel(4, pos1 - 2, BLACK);
    display.drawPixel(4, pos1 - 1, BLACK);
  }
  display.drawPixel(3, pos1, BLACK);
  display.drawPixel(3, pos1 + 1, BLACK);
  display.drawPixel(3, pos1 + 2, BLACK);
  display.drawPixel(3, pos1 + 3, BLACK);

  display.drawPixel(4, pos1, BLACK);
  display.drawPixel(4, pos1 + 1, BLACK);
  display.drawPixel(4, pos1 + 2, BLACK);
  display.drawPixel(4, pos1 + 3, BLACK);

  //paleta derecha

  if (pos2 - paletaAlto >= scoreDivisor) {
    display.drawPixel(79, pos2 - 3, BLACK);
    display.drawPixel(79, pos2 - 2, BLACK);
    display.drawPixel(79, pos2 - 1, BLACK);

    display.drawPixel(80, pos2 - 3, BLACK);
    display.drawPixel(80, pos2 - 2, BLACK);
    display.drawPixel(80, pos2 - 1, BLACK);
  }
  display.drawPixel(79, pos2, BLACK);
  display.drawPixel(79, pos2 + 1, BLACK);
  display.drawPixel(79, pos2 + 2, BLACK);
  display.drawPixel(79, pos2 + 3, BLACK);

  display.drawPixel(80, pos2, BLACK);
  display.drawPixel(80, pos2 + 1, BLACK);
  display.drawPixel(80, pos2 + 2, BLACK);
  display.drawPixel(80, pos2 + 3, BLACK);
}

void drawCursor(int x, int y) {

  //dibujo estela
  int estelaX = x - (ballDX * 2);
  int estelaY = y - (ballDY * 2);

  display.drawPixel(estelaX - 1, estelaY - 1, BLACK);
  display.drawPixel(estelaX  , estelaY - 1, BLACK);
  display.drawPixel(estelaX + 1, estelaY - 1, BLACK);
  display.drawPixel(estelaX - 1, estelaY, BLACK);
  display.drawPixel(estelaX  , estelaY, BLACK);
  display.drawPixel(estelaX + 1, estelaY, BLACK);
  display.drawPixel(estelaX - 1, estelaY + 1, BLACK);
  display.drawPixel(estelaX  , estelaY + 1, BLACK);
  display.drawPixel(estelaX + 1, estelaY + 1, BLACK);


  //dibujo pelota de 3x3
  display.drawPixel(x - 1, y - 1, BLACK);
  display.drawPixel(x  , y - 1, BLACK);
  display.drawPixel(x + 1, y - 1, BLACK);
  display.drawPixel(x - 1, y, BLACK);
  display.drawPixel(x  , y, BLACK);
  display.drawPixel(x + 1, y, BLACK);
  display.drawPixel(x - 1, y + 1, BLACK);
  display.drawPixel(x  , y + 1, BLACK);
  display.drawPixel(x + 1, y + 1, BLACK);




}
void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
    //display.println();
  }
  display.display();
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, BLACK);
    display.display();
  }
}
void testfilltriangle(void) {
  uint8_t color = BLACK;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
void testdrawroundrect(void) {
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, BLACK);
    display.display();
  }
}
void testfillroundrect(void) {
  uint8_t color = BLACK;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
void testdrawrect(void) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, BLACK);
    display.display();
  }
}
void testdrawline() {
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, BLACK);
    display.display();
  }
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, BLACK);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, BLACK);
    display.display();
  }
  for (int8_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, BLACK);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, BLACK);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, BLACK);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, BLACK);
    display.display();
  }
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, BLACK);
    display.display();
  }
  delay(250);
}
