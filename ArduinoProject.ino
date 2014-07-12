#include <Arduino.h>

static bool drawFrame=true;

#include "buffers.h"
#include "maxcontrol.h"
#include "sounds.h"

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
screenBuffer<unsigned int> *buf;
maxController *maxer;

buffer<snakeBodypart> *snake;
screenBuffer<unsigned int> *sprite;

int x,y,ox,oy,xd,yd;
int ax,ay,ak;
song *tune;
bool playSounds;
/// ////////////
///
/// ////////////

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
    buf=new screenBuffer<unsigned int>(8,8);
    //tune=new song(new buffer<int>(new data<int>(sizeof(overworld_theme),overworld_theme)),new buffer<int>(new data<int>(sizeof(overworld_theme_tempo),overworld_theme_tempo)));
    tune=new song(new buffer<unsigned short>(new memory_block<unsigned short>(sizeof(underworld_melody),underworld_melody)),new buffer<unsigned short>(new memory_block<unsigned short>(sizeof(underworld_tempo),underworld_tempo)));
    //tune=new song(new buffer(new data(sizeof(cocacola_melody),cocacola_melody)),new buffer(new data(sizeof(cocacola_tempo),cocacola_tempo)));
    //tune=new song(new buffer<unsigned short>(new memory_block<unsigned short>(sizeof(tune0),tune0)),new buffer<unsigned short>(new memory_block<unsigned short>(sizeof(tempo0),tempo0)));
    //tune=new song(tune1,sizeof(tune1)/sizeof(tune1[0]),tempo1,sizeof(tempo1)/sizeof(tempo1[0]));
    buf->set(0,0,1);
    buf->set(7,0,1);
    buf->set(0,7,1);
    buf->set(7,7,1);
    xActionTrigger(0,1);
    xActionTrigger(1,1);
    xActionTrigger(2,1);
    playSounds=true;

    buffer<int> *tbuf=new buffer<int>(2);
    tbuf->print();
    tbuf->push_back(10);
    tbuf->print();
    tbuf->clear();
    tbuf->push_front(10);
    tbuf->print();


    sprite=new screenBuffer<unsigned int>(3,3);
    sprite->set(0,1,1);
    sprite->set(1,1,1);
    sprite->set(2,1,1);
    sprite->set(1,0,1);
    sprite->set(1,2,1);
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
    tune->playAsync(13);
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
        Serial.println(end-start);
    }
//    tune->playAsync(13);
//    if(millis()>renderClk)
//    {
//        renderClk=millis()+60;
//        buf->get(0,0)==1?buf->set(0,0,0):buf->set(0,0,1);
//    }
////    buf->set(x,y,1);
//
//    maxer->draw(buf);
////    buf->set(x,y,0);
}



























