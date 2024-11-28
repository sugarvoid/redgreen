
#define RAYGUI_IMPLEMENTATION

#include "../include/raylib/raygui.h"
#include "../include/raylib/raylib.h"
#include "../include/raylib/raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { SETUP, RED_L, GREEN_L, NOT_PLAYING } STATES;

typedef struct {
  const char *key;
  float value;
} KeyValuePair;

KeyValuePair times[] = {{"red_short_min", 3.0f},   {"red_short_max", 8.0f},
                        {"green_short_min", 4.0f}, {"green_short_max", 9.0f},
                        {"red_long_min", 9.0f},    {"red_long_max", 19.0f},
                        {"green_long_min", 7.0f},  {"green_long_max", 17.0f}};

typedef struct {
  float Lifetime;
  bool Repeat;
} Timer;

// start or restart a timer with a specific lifetime
void StartTimer(Timer *timer, float lifetime) {
  if (timer != NULL)
    timer->Lifetime = lifetime;
}

// update a timer with the current frame time
void UpdateTimer(Timer *timer) {
  // subtract this frame from the timer if it's not already expired
  if (timer != NULL && timer->Lifetime > 0)
    timer->Lifetime -= GetFrameTime();
}

// check if a timer is done.
bool TimerDone(Timer *timer) {
  if (timer != NULL)
    return timer->Lifetime <= 0;

  return false;
}

Timer tmrMain;
Timer tmrRound;

float roundLength;
int gameLength;
int rounds;
int currentRound;
int roundTicker = 30;
int roundTimeLeft = 10;
int timeLeft = 45;

Sound fxGo;
Sound fxWait;
Sound fxGameOver;

Font monogram;

STATES currentState = SETUP;

float GetRandomFloat(float min, float max) {
  return ((float)rand() / RAND_MAX) * (max - min) + min;
}

void StartNewRound() {
  float minSecs;
  float maxSecs;
  // TODO: Change color to green
  PlaySound(fxGo);
  currentState = GREEN_L;
  StopSound(fxWait);
  // TODO: Check radio button
  if (true) {
    // SHort
    roundLength = GetRandomFloat(times[2].value, times[3].value);

  } else {
    // Long
    roundLength = GetRandomFloat(times[6].value, times[7].value);
  }
}

// Function to print the values for demonstration
void printTimes() {
  int num_entries = sizeof(times) /
                    sizeof(times[0]); // Get the number of elements in the array
  for (int i = 0; i < num_entries; i++) {
    printf("%s: %.1f\n", times[i].key, times[i].value);
  }
}

void Update() {}
void Draw() {
  switch (currentState) {

  SETUP:;
  GREEN_L:;
  RED_L:;
  NOT_PLAYING:;
  _:
    TraceLog(LOG_FATAL, "Start is not set");
  }
}
void CleanUp() {
  UnloadSound(fxGo);
  UnloadSound(fxWait);
  UnloadSound(fxGameOver);
  UnloadFont(monogram);
}

void DrawLabels() {}

int main() {
  InitWindow(600, 600, "Red Green");
  InitAudioDevice();
  srand(time(NULL));

  printTimes();

  fxGo = LoadSound("res/inspectorj__bell-counter.wav");
  fxWait = LoadSound("resources/vataaa__metronome.ogg");
  fxGameOver = LoadSound("resources/davidbain__end_game_fail.wav");
  monogram = LoadFont("res/monogram.ttf");
  GuiSetFont(monogram);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
  SetTargetFPS(30);

  bool showMessageBox = false;

  while (!WindowShouldClose()) {

    // Update
    roundTicker--;
    if (roundTicker <= 0) {
      Clamp(timeLeft--, 0, 100);
      // timeLeft--;
      roundTicker = 30;
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    // TODO: Change color of background based on round.
    ClearBackground(BLUE);

    if (GuiButton((Rectangle){24, 24, 300, 30}, "Show Message")) {
      showMessageBox = true;
      PlaySound(fxGo);
    }

    // if (GuiButton((Rectangle){24, 24, 300, 30}, "Show Message"))

    GuiLabel((Rectangle){50, 50, 100, 50}, TextFormat("%03i", timeLeft));
    // GuiLabel((Rectangle){50, 50, 100, 50}, TextFormat("%03i", timeLeft));

    // DrawTextPro(monogram, const char *text, Vector2 position, Vector2 origin,
    // float rotation, float fontSize, float spacing, Color tint)

    if (showMessageBox) {
      int result =
          GuiMessageBox((Rectangle){85, 70, 250, 100}, "#191#Message Box",
                        "Hi! This is a message!", "Nice;Cool");

      if (result >= 0)
        showMessageBox = false;
    }

    EndDrawing();
  }

  CleanUp();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
