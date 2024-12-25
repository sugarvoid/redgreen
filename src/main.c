
#define RAYGUI_IMPLEMENTATION

#include "../include/raylib/raygui.h"
#include "../include/raylib/raylib.h"
#include "../include/raylib/raymath.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { SETUP, RED_L, GREEN_L, DONE } STATES;

const int redShortMin = 3;
const int redShortMax = 8;
const int redLongMin = 9;
const int redLongMax = 19;

const int greenShortMin = 4;
const int greenShortMax = 9;
const int greenLongMin = 7;
const int greenLongMax = 17;

const int FPS = 30;

int roundLength;
int gameLength = 99;
int rounds;
int currentRound;
int ticker;
int greenLength;
int redLength;

bool showMessageBox = false;
bool isGameRunning = false;

int greenMode;
int redMode;

int dboGreenActive = 0;
bool dboGreenEditMode = false;

int dboRedActive = 0;
bool dboRedEditMode = false;

int spnTotalTimeValue = 0;
bool spnTotalTimeEditMode = false;

Rectangle frameRec;
const Vector2 lightPosition = {400.0f, 50.0f};
int currentFrame = 0;

Color previewColor = GRAY;

Sound fxGo;
Sound fxGameOver;
Music fxWait;
Font monogram;

STATES currentState;

float GetRandomFloat(float min, float max) {
  return ((float)rand() / RAND_MAX) * (max - min) + min;
}

int GetRandomInt(int min, int max) {
  return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

const char *GetModeText(int mode) {
  if (mode == 0)
    return "Short";
  return "Long";
}

void StartGreen() {
  if (greenMode == 0) {
    // Short
    roundLength = GetRandomInt(greenShortMin, greenShortMax);
    greenLength = roundLength;
  } else {
    // Long
    roundLength = GetRandomInt(greenLongMin, greenLongMax);
    greenLength = roundLength;
  }
  currentState = GREEN_L;
  previewColor = GREEN;
  PlaySound(fxGo);
  StopMusicStream(fxWait);
  currentFrame = 2;
  TraceLog(LOG_DEBUG, TextFormat("Starting Green: %i seconds", greenLength));
}

void StartRed() {
  if (redMode == 0) {
    // Short
    roundLength = GetRandomInt(redShortMin, redShortMax);
    redLength = roundLength;
  } else {
    // Long
    roundLength = GetRandomInt(redLongMin, redLongMax);
    redLength = roundLength;
  }
  StopSound(fxGo);
  currentFrame = 1;
  PlayMusicStream(fxWait);
  currentState = RED_L;
  previewColor = RED;
  TraceLog(LOG_DEBUG, TextFormat("Starting Red: %i seconds", redLength));
}

void StartGame(int minutes) {
  if (minutes > 0) {
    gameLength = minutes * 60;
    TraceLog(
        LOG_DEBUG,
        TextFormat("Staring Game\nLength: %i\nGreen Mode: %s\nRed Mode: %s",
                   minutes, GetModeText(greenMode), GetModeText(redMode)));
    isGameRunning = true;
    StartGreen();
  }
}

void Update() {
  UpdateMusicStream(fxWait);
  if (isGameRunning) {
    ticker--;
    if (ticker <= 0) {
      Clamp(gameLength--, 0, 100);

      if (currentState == GREEN_L) {
        Clamp(greenLength--, 0, 100);
        if (greenLength == 0) {
          StartRed();
        }
      } else if (currentState == RED_L) {
        Clamp(redLength--, 0, 100);
        if (redLength == 0) {
          StartGreen();
        }
      }
      ticker = FPS;
    }
  }

  if (gameLength <= 0 && currentState != DONE) {
    isGameRunning = false;
    currentState = DONE;
    PlaySound(fxGameOver);
    showMessageBox = true;
  }
}

void Draw() {
  BeginDrawing();
  ClearBackground(DARKGRAY);

  if (currentState == SETUP) {
    if (GuiButton((Rectangle){24, 200, 300, 30}, "Start")) {
      greenMode = dboGreenActive;
      redMode = dboRedActive;
      StartGame(spnTotalTimeValue);
      previewColor = GREEN;
    }

    if (GuiSpinner((Rectangle){25, 30, 125, 30}, NULL, &spnTotalTimeValue, 0,
                   100, spnTotalTimeEditMode))
      spnTotalTimeEditMode = !spnTotalTimeEditMode;

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0xff0000ff);
    if (GuiDropdownBox((Rectangle){160, 100, 125, 30}, "Short;Long",
                       &dboRedActive, dboRedEditMode)) {
      dboRedEditMode = !dboRedEditMode;
      TraceLog(LOG_DEBUG, TextFormat("Selected Red: %i", dboRedActive));
    }

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0x008000FF);
    if (GuiDropdownBox((Rectangle){25, 100, 125, 30}, "Short;Long",
                       &dboGreenActive, dboGreenEditMode)) {
      dboGreenEditMode = !dboGreenEditMode;
      TraceLog(LOG_DEBUG, TextFormat("Selected Green: %i", dboGreenActive));
    }

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0x00000000);
  }

  if (currentState == GREEN_L || currentState == RED_L) {
    DrawRectangle(200, 40, 200, 200, previewColor);
  }

  if (showMessageBox) {
    int result =
        GuiMessageBox((Rectangle){85, 70, 270, 120}, "Alert", "Done", "Okay");

    if (result >= 0) {
      currentState = SETUP;
      showMessageBox = false;
    }
  }

  EndDrawing();
}
void CleanUp() {
  UnloadSound(fxGo);
  UnloadMusicStream(fxWait);
  UnloadSound(fxGameOver);
  UnloadFont(monogram);
}

int main() {
  InitWindow(600, 300, "Red Green");
  SetTargetFPS(FPS);
  InitAudioDevice();
  currentState = SETUP;
  ticker = FPS;

  SetTraceLogLevel(LOG_DEBUG);
  currentFrame = 0;
  srand(time(NULL));
  fxGo = LoadSound("res/inspectorj__bell-counter.wav");
  fxWait = LoadMusicStream("res/vattaaa__metronome.ogg");
  fxGameOver = LoadSound("res/davidbain__end_game_fail.wav");
  monogram = LoadFont("res/monogram.ttf");
  GuiSetFont(monogram);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 32);

  while (!WindowShouldClose()) {
    Update();
    Draw();
  }

  CleanUp();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
