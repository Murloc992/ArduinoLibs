#include <Arduino.h>

static bool drawFrame=true;

#include "buffers.h"
#include "maxcontrol.h"
#include "sounds.h"
#include "tune.h"

#include "res_sfx.h"

#ifdef ARDUSIM
#include <iostream>
#endif

typedef void (*FunctionPointer) ();
int xActionsFlags[] = {0,0,0};
void xActionTrigger(int id=0, int action=0)
{
    xActionsFlags[id] = action;
}

#define LEFT    0
#define UP      1
#define RIGHT   2
#define DOWN    3

struct snakeBodypart
{
    int xpos;
    int ypos;
    int direction;
};

/// //////////////
/// SOME GLOBALS
/// //////////////
screenBuffer<unsigned short> *buf;
maxController *maxer;

buffer<snakeBodypart> *snake;
screenBuffer<unsigned short> *sprite;

int x,y,ox,oy,xd,yd;
int ax,ay,ak;
song *tune;
streamableSound *snd;
bool playSounds;
/// ////////////
///
/// ////////////

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup()
{
    x=4;
    y=2;
    xd=1;
    yd=1;
    maxer=new maxController(2,3,4,4);

    pinMode(13,OUTPUT);

    Serial.begin(9600);
#ifdef ARDUSIM
    printf("Size of melody: %d Tempo: %d\n",sizeof(tune0),sizeof(tempo0));
#endif // ARDUSIM
    buf=new screenBuffer<unsigned short>(8,8);
    xActionTrigger(0,1);
    xActionTrigger(1,1);
    xActionTrigger(2,1);
    playSounds=true;

    sprite=new screenBuffer<unsigned short>(3,3);
    sprite->set(0,1,1);
    sprite->set(1,1,1);
    sprite->set(2,1,1);
    sprite->set(1,0,1);
    sprite->set(1,2,1);

    snd=new streamableSound(13,underworld_melody,sizeof(underworld_melody),underworld_tempo,sizeof(underworld_tempo),true);
}

void updateLoop()
{
    //if(ak<12)
    //{
    //    playSounds=!playSounds;
    //    xActionTrigger(2,playSounds);
    //    ak=1024;
    //}
    if(drawFrame)
    {
        ax=analogRead(0);
        ay=analogRead(1);
        ak=analogRead(2);

        ox=x;
        oy=y;
        if(ax>544)x--;
        else if(ax<478)x++;
        if(ay>544)y--;
        else if(ay<478)y++;
        if(x<=0)x=0;
        if(x+2>=7)x=7-2;
        if(y<=0)y=0;
        if(y+2>=7)y=7-2;
    }

}

void renderLoop()
{
    buf->reset();
    buf->blit(x,y,sprite);
    maxer->draw(buf);
}

void soundLoop()
{
    snd->playAsync();
    //tune->playAsync(13);
}

FunctionPointer xActions[] = {updateLoop,renderLoop,soundLoop};

int xActionsCount = sizeof(xActions)/sizeof(FunctionPointer);

void xDoActions()
{
    for(unsigned int j=0; j < xActionsCount; j++)
    {
        if( xActionsFlags[j] == 1 )
        {
            xActions[j]();
        }
    }
}
static unsigned long tick=0;
void loop()
{
    unsigned long start=millis();
    xDoActions();
    unsigned long end=millis();
    if(millis()>=tick+1000)
    {
        tick=millis();
        Serial.print(F("CYCLE TIME "));
        Serial.println(end-start);
        Serial.print(F("FREE RAM "));
        Serial.println(freeRam());
        Serial.print(F("USED RAM "));
        Serial.println(2048-freeRam());
        Serial.println();
    }
}



























