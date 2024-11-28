
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


const int redShortMin = 3;
const int redShortMax = 8;
const int redLongMin = 9;
const int redLongMax = 19;

const int greenShortMin = 4;
const int greenShortMax = 9;
const int greenLongMin = 7;
const int greenLongMax = 17;



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
const int FPS = 30;
float roundLength;
int gameLength = 99;
int rounds;
int currentRound;
int ticker = 30;
int roundTimeLeft = 10;
int timeLeft = 45;

bool showMessageBox = false;
bool hasGameStarted = false;

bool isGreenLong;
bool isRedLong;

int greenMode;
int redMode;

int comboBoxActive = 1;

int dboGreenActive = 0;
bool dboGreenEditMode = false;

int dboRedActive = 0;
bool dboRedEditMode = false;

int spnTotalTimeValue = 0;
bool spnTotalTimeEditMode = false;

Color previewColor = GRAY;

Sound fxGo;
//Sound fxWait;
Sound fxGameOver;

Music fxWait;

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

const char *GetModeText(int mode)
{
  if (mode == 0)
    return "Short";
  return "Long";
}



void StartGreen()
{
  currentState = GREEN_L;
  previewColor = GREEN;
  PlaySound(fxGo);
  StopMusicStream(fxWait);
  if (greenMode == 0)
  {
    // SHort
    roundLength = GetRandomInt(greenShortMin, greenShortMax);
  }
  else
  {
    // Long
    roundLength = GetRandomInt(greenLongMin, greenLongMax);
  }

  // TODO: start timer with roundLength
}

void StartRed()
{
  StopSound(fxGo);
  PlayMusicStream(fxWait);
  currentState = RED_L;
  previewColor = RED;
  if (redMode == 0)
  {
    // SHort
    roundLength = GetRandomInt(redShortMin, redShortMax);
  }
  else
  {
    // Long
    roundLength = GetRandomInt(redLongMin, redLongMax);
  }

  // TODO: start timer with roundLength
}

void StartGame(int minutes)
{
  if (minutes > 0)
  {
    gameLength = minutes * 60;
    TraceLog(LOG_INFO, TextFormat("Staring Game\nLength: %i\nGreen Mode: %s\nRed Mode: %s", minutes, GetModeText(greenMode), GetModeText(redMode)));
    hasGameStarted = true;
    StartGreen();
  }
}

void Update() {}
void Draw() {}
void CleanUp()
{
  UnloadSound(fxGo);
  UnloadMusicStream(fxWait);
  UnloadSound(fxGameOver);
  UnloadFont(monogram);
}

void DrawLabels() {}

int main()
{
  InitWindow(600, 300, "Red Green");
  SetTargetFPS(FPS);
  InitAudioDevice();
  srand(time(NULL));
  fxGo = LoadSound("res/inspectorj__bell-counter.wav");
  fxWait = LoadMusicStream("res/vattaaa__metronome.ogg");
  fxGameOver = LoadSound("res/davidbain__end_game_fail.wav");
  monogram = LoadFont("res/monogram.ttf");
  GuiSetFont(monogram);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 32);

  while (!WindowShouldClose())
  {
    //TraceLog(LOG_INFO, TextFormat("%d", IsMusicValid(fxWait)));
    //TraceLog(LOG_INFO, TextFormat("%d", IsMusicStreamPlaying(fxWait)));
    //TraceLog(LOG_INFO, TextFormat("%f", GetMusicTimePlayed(fxWait)));
    // Update
    //----------------------------------------------------------------------------------
    UpdateMusicStream(fxWait);
    if (hasGameStarted)
    {
      
      //if (currentState == GREEN_L && !IsSoundPlaying(fxWait))
      //{
      //  TraceLog(LOG_INFO, "Starting ticking sound again");
        //PlaySound(fxWait);
        //continue;
      //}
      ticker--;
      if (ticker <= 0)
      {
        Clamp(gameLength--, 0, 100);
        ticker = FPS;
      }
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(DARKGRAY);

    if (GuiButton((Rectangle){24, 200, 300, 30}, "Start"))
    {
      greenMode = dboGreenActive;
      redMode = dboRedActive;
      StartGame(spnTotalTimeValue);
      previewColor = GREEN;
    }

    if (GuiSpinner((Rectangle){25, 30, 125, 30}, NULL, &spnTotalTimeValue, 0, 100, spnTotalTimeEditMode))
      spnTotalTimeEditMode = !spnTotalTimeEditMode;

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0xff0000ff);
    if (GuiDropdownBox((Rectangle){160, 100, 125, 30}, "Short;Long", &dboRedActive, dboRedEditMode))
    {
      dboRedEditMode = !dboRedEditMode;
      TraceLog(LOG_INFO, TextFormat("Selected Red: %i", dboRedActive));
    }

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0x008000FF);
    if (GuiDropdownBox((Rectangle){25, 100, 125, 30}, "Short;Long", &dboGreenActive, dboGreenEditMode))
    {
      dboGreenEditMode = !dboGreenEditMode;
      TraceLog(LOG_INFO, TextFormat("Selected Green: %i", dboGreenActive));
    }

    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0x00000000);

    GuiLabel((Rectangle){50, 50, 100, 50}, TextFormat("%03i", gameLength));

    DrawRectangle(350, 40, 200, 200, previewColor);

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
