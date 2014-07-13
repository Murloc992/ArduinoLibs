#include <Arduino.h>

static bool drawFrame=true;

#include "buffers.h"
#include "maxcontrol.h"
#include "sounds.h"
#include "snake.h"

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

struct snakeBodypart
{
    unsigned short xpos;
    unsigned short ypos;
    unsigned short direction;
};

/// //////////////
/// SOME GLOBALS
/// //////////////
screenBuffer<unsigned char> *buf;
maxController *maxer;
screenBuffer<unsigned char> *sprite;

int x,y,ox,oy,xd,yd;
int ax,ay,ak;
song *tune;
streamableSound *snd;
bool playSounds;
snake* snk;
/// ////////////
///
/// ////////////

void setup ()
{
    x=4;
    y=2;
    xd=1;
    yd=1;
    maxer=new maxController(2,3,4,4);

    pinMode(13,OUTPUT);

    Serial.begin(9600);

    buf=new screenBuffer<unsigned char>(24,16);
    xActionTrigger(0,1);
    xActionTrigger(1,1);
    xActionTrigger(2,1);
    playSounds=true;

    sprite=new screenBuffer<unsigned char>(3,3);
    sprite->set(0,1,1);
    sprite->set(1,1,1);
    sprite->set(2,1,1);
    sprite->set(1,0,1);
    sprite->set(1,2,1);

    snd=new streamableSound(13,underworld_melody,sizeof(underworld_melody),underworld_tempo,sizeof(underworld_tempo),true);
    snk=new snake(4,4,2);
}

void updateLoop()
{
    //if(ak<12)
    //{
    //    playSounds=!playSounds;
    //    xActionTrigger(2,playSounds);
    //    ak=1024;
    //}
    ax=analogRead(0);
        ay=analogRead(1);
        ak=analogRead(2);

        if(ax>544&&snk->dir!=RIGHT)snk->dir=LEFT;
        else if(ax<478&&snk->dir!=LEFT)snk->dir=RIGHT;
        else if(ay>544&&snk->dir!=DOWN)snk->dir=UP;
        else if(ay<478&&snk->dir!=UP)snk->dir=DOWN;
    if(drawFrame)
    {
        snk->update();

    }

}

void renderLoop()
{
    maxer->draw(buf);
    snk->draw(buf);
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



























