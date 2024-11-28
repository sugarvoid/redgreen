
#define RAYGUI_IMPLEMENTATION

#include "../include/raylib/raygui.h"
#include "../include/raylib/raylib.h"
#include "../include/raylib/raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef enum
{
  SETUP,
  RED_L,
  GREEN_L,
  NOT_PLAYING
} STATES;

typedef struct
{
  const char *key;
  int value;
} KeyValuePair;

KeyValuePair times[] = {{"red_short_min", 3}, {"red_short_max", 8}, {"green_short_min", 4}, {"green_short_max", 9}, {"red_long_min", 9}, {"red_long_max", 19}, {"green_long_min", 7}, {"green_long_max", 17}};

typedef struct
{
  bool IsStarted;
  int Lifetime;
  bool IsRepeat;
} Timer;

// start or restart a timer with a specific lifetime
void StartTimer(Timer *timer, int lifetime)
{
  if (timer != NULL)
  {
    timer->Lifetime = lifetime;
  }
}

void LowerTimer(Timer *timer)
{
  // subtract this frame from the timer if it's not already expired
  if (timer != NULL && timer->Lifetime > 0)
  {
    timer->Lifetime--;
  }
}

// check if a timer is done.
bool TimerDone(Timer *timer)
{
  if (timer != NULL)
  {
    return timer->Lifetime <= 0;
  }
  else
  {
    return false;
  }
}

Timer tmrMain;
Timer tmrRound;

float roundLength;
int gameLength = 99;
int rounds;
int currentRound;
int roundTicker = 30;
int roundTimeLeft = 10;
int timeLeft = 45;

int comboBoxActive = 1;

int dboGreenActive = 0;
bool dboGreenEditMode = false;

int dropdownBox001Active = 0;
bool dropDown001EditMode = false;

Color previewColor = GRAY;

Sound fxGo;
Sound fxWait;
Sound fxGameOver;

Font monogram;

STATES currentState = SETUP;

float GetRandomFloat(float min, float max)
{
  return ((float)rand() / RAND_MAX) * (max - min) + min;
}

int GetRandomInt(int min, int max)
{
  return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void StartGreen()
{
  previewColor = GREEN;
  float minSecs;
  float maxSecs;
  PlaySound(fxGo);
  currentState = GREEN_L;
  StopSound(fxWait);
  if (true)
  {
    // SHort
    roundLength = GetRandomInt(times[2].value, times[3].value);
  }
  else
  {
    // Long
    roundLength = GetRandomInt(times[6].value, times[7].value);
  }
}

void StartRed()
{
  previewColor = RED;
}

// Function to print the values for demonstration
void printTimes()
{
  int num_entries = sizeof(times) /
                    sizeof(times[0]); // Get the number of elements in the array
  for (int i = 0; i < num_entries; i++)
  {
    printf("%s: %d\n", times[i].key, times[i].value);
  }
}

void Update() {}
void Draw()
{
  switch (currentState)
  {

  SETUP:;
  GREEN_L:;
  RED_L:;
  NOT_PLAYING:;
  _:
    TraceLog(LOG_FATAL, "Start is not set");
  }
}
void CleanUp()
{
  UnloadSound(fxGo);
  UnloadSound(fxWait);
  UnloadSound(fxGameOver);
  UnloadFont(monogram);
}

void DrawLabels() {}

int main()
{
  InitWindow(600, 300, "Red Green");
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

  while (!WindowShouldClose())
  {

    // Update
    //----------------------------------------------------------------------------------
    roundTicker--;
    if (roundTicker <= 0)
    {
      Clamp(timeLeft--, 0, 100);
      // timeLeft--;
      roundTicker = 30;
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(DARKGRAY);

    if (GuiButton((Rectangle){24, 24, 300, 30}, "Start"))
    {
      previewColor = RED;
      PlaySound(fxGo);
    }

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0xff0000ff);
    if (GuiDropdownBox((Rectangle){160, 100, 125, 30}, "Short;Long", &dropdownBox001Active, dropDown001EditMode))
    {
      dropDown001EditMode = !dropDown001EditMode;
    }

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0x008000FF);
    if (GuiDropdownBox((Rectangle){25, 100, 125, 30}, "Short;Long", &dboGreenActive, dboGreenEditMode))
    {
      dboGreenEditMode = !dboGreenEditMode;
      TraceLog(LOG_INFO, TextFormat("Selected Green: %i", dboGreenActive));
    }

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0x00000000);

    GuiLabel((Rectangle){50, 50, 100, 50}, TextFormat("%03i", timeLeft));

    DrawRectangle(400, 100, 150, 150, previewColor);

    if (showMessageBox)
    {
      int result =
          GuiMessageBox((Rectangle){85, 70, 270, 120}, "Alert",
                        "Done", "Okay");

      if (result >= 0)
      {
        previewColor = GREEN;
        showMessageBox = false;
      }
    }

    if (gameLength <= 0)
    {
      showMessageBox = true;
    }

    EndDrawing();
  }

  CleanUp();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
