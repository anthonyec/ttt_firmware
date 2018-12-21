#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define OLED_RESET 4
#define TEXT_NUDGE_X 4
#define TEXT_NUDGE_Y 2
#define DEBUG false

Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);

int score[] = {0, 0};

String serialCommand = "";

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);
  display.setTextWrap(false);
  display.clearDisplay();
  display.setTextSize(2);
  display.setFont(&FreeSans18pt7b);

  display.drawLine(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, WHITE);
  display.display();
}

char* string2char(String command){
  if(command.length()!=0){
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}

void renderScoreNumber(int x, int y, int score, bool highlighted) {
  String scoreText = String(score);
  int16_t textX, textY;
  uint16_t textWidth, textHeight;

  if (highlighted) {
    display.fillRoundRect(x, y, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 4, WHITE);
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }

  display.getTextBounds(scoreText, x, y, &textX, &textY, &textWidth, &textHeight);

  if (DEBUG) {
    display.drawRect(x + (SCREEN_WIDTH / 2) - (textWidth / 2), y + (SCREEN_HEIGHT / 2 / 2) - (textHeight / 2), textWidth, textHeight, WHITE);
  }

  display.setCursor(x + (SCREEN_WIDTH / 2) - (textWidth / 2) - TEXT_NUDGE_X, y + (SCREEN_HEIGHT / 2 / 2) + (textHeight / 2) - TEXT_NUDGE_Y);
  display.print(scoreText);
}

void renderScoreScreen() {
  display.clearDisplay();

  renderScoreNumber(0, 0, score[0], true);
  renderScoreNumber(0, SCREEN_HEIGHT/2, score[1], false);

  display.display();
}

void loop() {
  if (Serial.available() > 0) {
    serialCommand = Serial.readString();
    Serial.print(serialCommand);

    if (serialCommand == "add") {
      addScoreForPlayer(0);
    }

    if (serialCommand == "sub") {
      subtractScoreForPlayer(0);
    }

    serialCommand = "";
  }

  renderScoreScreen();
}

void addScoreForPlayer(int playerIndex) {
  score[playerIndex] += 1;
}

void subtractScoreForPlayer(int playerIndex) {
  score[playerIndex] -= 1;
}

void undo() {

}
