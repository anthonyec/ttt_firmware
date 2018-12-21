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

Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);

int score[] = {0, 0};

String serialCommand = "";

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);
  display.setTextWrap(false);
  display.setTextColor(WHITE);
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

  // display.drawRect(x, y, SCREEN_WIDTH, SCREEN_HEIGHT/2 - 4, WHITE);
  display.getTextBounds(scoreText, x, y, &textX, &textY, &textWidth, &textHeight);

  // display.setCursor((SCREEN_WIDTH / 2) - (textWidth / 2), 50);
  // display.println(scoreText);


  display.drawRect((SCREEN_WIDTH / 2) - (textWidth / 2), (SCREEN_HEIGHT / 2 / 2) - (textHeight / 2), textWidth, textHeight, WHITE);
  // display.drawRect(0, 0, textWidth, textHeight, WHITE);

  display.setCursor((SCREEN_WIDTH / 2) - (textWidth / 2) - TEXT_NUDGE_X, (SCREEN_HEIGHT / 2 / 2) + (textHeight / 2) - TEXT_NUDGE_Y);
  display.print(scoreText);
}

void renderScoreScreen() {
  display.clearDisplay();

  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

  renderScoreNumber(0, 0, score[0], false);
  // renderScoreNumber(0, SCREEN_HEIGHT/2 + 4, score[1], false);

  // display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT/2 - 4, WHITE);
  // display.drawRect(0, SCREEN_HEIGHT/2 + 4, SCREEN_WIDTH, SCREEN_HEIGHT/2 - 4, WHITE);

  // display.setCursor(0, 0);
  // display.println(score[0]);
  // display.setCursor(14, 125);
  // display.println(score[1]);

  display.drawLine(0, round(SCREEN_HEIGHT/2), SCREEN_WIDTH, round(SCREEN_HEIGHT/2), WHITE);

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
