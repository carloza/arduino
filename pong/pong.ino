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


#define MAX_PONG_LIFES 12
#define PONG_LIFES 5

#define JUGADOR1 A14 //potenciometro 1
#define JUGADOR2 A15 //potenciometro 2

static const unsigned char img [] PROGMEM = {
  // 'screen-img, 84x48px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x03, 0xfe, 0x00, 
  0x00, 0x07, 0xff, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x06, 0x03, 0x80, 0x00, 0x07, 0xff, 0x00, 0x00, 
  0xe0, 0x00, 0x00, 0x08, 0x00, 0xc0, 0x00, 0x00, 0x70, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x10, 0x00, 
  0x40, 0x00, 0x00, 0x70, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x22, 0x04, 0x60, 0x00, 0x00, 0x77, 0x0d, 
  0xdc, 0xee, 0x0f, 0x80, 0x25, 0x0a, 0x20, 0x00, 0x00, 0x77, 0x0d, 0xfe, 0xff, 0x1f, 0xc0, 0x2f, 
  0xc4, 0x30, 0x00, 0x00, 0x77, 0x0d, 0xe6, 0xf3, 0xbf, 0xe0, 0x30, 0x00, 0x10, 0x00, 0x00, 0x77, 
  0x0d, 0xc0, 0xe1, 0xbd, 0xe0, 0x40, 0x00, 0x18, 0x00, 0x00, 0x77, 0x0d, 0xc0, 0xe1, 0xb8, 0xe0, 
  0x80, 0x00, 0x08, 0x00, 0x00, 0x77, 0x0d, 0xc0, 0xe1, 0xb8, 0xe1, 0x01, 0x80, 0x08, 0x00, 0x00, 
  0x77, 0x0d, 0xc0, 0xe1, 0xbd, 0xe3, 0xfe, 0x00, 0x08, 0x00, 0x00, 0x77, 0x9d, 0xc0, 0xf3, 0xbf, 
  0xe1, 0x00, 0x00, 0x08, 0x00, 0x00, 0x77, 0xfd, 0xc0, 0xff, 0x1f, 0xc0, 0xe0, 0x00, 0x08, 0x00, 
  0x00, 0x73, 0xed, 0xc0, 0xfe, 0x0f, 0x80, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xfe, 0x3f, 0x8e, 0x1c, 0xfe, 0x1f, 0xc7, 0x00, 0x38, 0x00, 0x03, 0xce, 0x7f, 0xcf, 
  0x1d, 0xe7, 0x3f, 0xe7, 0x00, 0x7c, 0x00, 0x07, 0x80, 0xf1, 0xef, 0x9d, 0xc0, 0x78, 0xf7, 0x00, 
  0x7c, 0x00, 0x07, 0x80, 0xe0, 0xef, 0xdd, 0xe0, 0x70, 0x77, 0x00, 0xee, 0x00, 0x07, 0x00, 0xe0, 
  0xef, 0xfc, 0x7c, 0x70, 0x77, 0x00, 0xc6, 0x00, 0x07, 0x00, 0xe0, 0xef, 0xfc, 0x1f, 0x70, 0x77, 
  0x01, 0xc7, 0x00, 0x07, 0x80, 0xe0, 0xee, 0xfc, 0x07, 0x70, 0x77, 0x01, 0xff, 0x00, 0x07, 0x80, 
  0xf1, 0xee, 0x7c, 0x07, 0x78, 0xf7, 0x01, 0xc7, 0x00, 0x03, 0xce, 0x7f, 0xce, 0x3d, 0xcf, 0x3f, 
  0xe7, 0xfb, 0x83, 0x80, 0x01, 0xfe, 0x3f, 0x8e, 0x1c, 0xfe, 0x1f, 0xc7, 0xfb, 0x83, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x3e, 0x03, 
  0xe0, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x7f, 0x07, 0xf0, 0x7f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x60, 0x70, 0xe3, 0x8e, 0x38, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0xe0, 0xc1, 
  0x8c, 0x18, 0xc1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc1, 0xc1, 0x9c, 0x19, 0xc1, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x81, 0x81, 0x98, 0x19, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf1, 
  0x81, 0x98, 0x19, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfb, 0x01, 0xb0, 0x1b, 0x01, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0x01, 0xb0, 0x1b, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x3b, 0x03, 0x30, 0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x03, 0x30, 0x33, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x7b, 0x06, 0x30, 0x63, 0x06, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc1, 0xfb, 0x86, 0x38, 0x63, 0x86, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe1, 0xfc, 0x1f, 0xc1, 
  0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0xf8, 0x0f, 0x80, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int juego = 0;

void mostrarLogos(){
  
  display.drawBitmap(0, 0, img, 84, 48, BLACK);
  display.display();
  delay(4000);
  
}



void setup()   {
  Serial.begin(9600);

  pinMode(JUGADOR1, INPUT);
  pinMode(JUGADOR2, INPUT);
  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(60);
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

int scoreP1 = PONG_LIFES;
int scoreP2 = PONG_LIFES;

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

  mostrarLogos();
  
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
    drawWinner("PLAYER 2 ");
  }
  if (scoreP2 == 0) {
    drawWinner("PLAYER 1 ");
  }
}

void drawWinner(char* jug) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.print(jug);
  display.println("WINS!  ");  
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
  
  int estelaX = x - (ballDX * 1);
  int estelaY = y - (ballDY * 1);
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
