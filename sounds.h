#ifndef SONGS_H_INCLUDED
#define SONGS_H_INCLUDED
#include "pitches.h"

enum
{
    WAIT_NEXT_ACTION = 0,
    PLAY_NEXT_NOTE = 1,
    PLAY_NOTHING
};

struct song
{
    buffer<unsigned short>* melody,*tempo;
    int cur_note;
    unsigned long noteTime,noteDuration,pauseBetweenNotes;
    bool playNote,played;

    uint8_t action;

    song(buffer<unsigned short>* tmelody,buffer<unsigned short>* ttemp)
    {
#ifdef ARDUSIM
        printf("Buffer sizes: %d %d\n",tmelody->getLength(),ttemp->getLength());
#endif // ARDUSIM
        melody=tmelody;
        tempo=ttemp;
        cur_note=0;
        playNote=true;
        played=false;
        noteTime=0;
        noteDuration=0;
        action=0;
    }

    void play(int musicPin)
    {
        int size = melody->getLength();
        for (int thisNote = 0; thisNote < size; thisNote++)
        {
            noteDuration = 1666/tempo->get(thisNote);

            tone(musicPin, melody->get(thisNote),noteDuration);

            int pauseBetweenNotes = noteDuration * 1.30;
            delay(pauseBetweenNotes);

            tone(musicPin, 0,noteDuration);
        }
    }

    void playAsync(int musicPin)
    {
        if(!played)
        {
            if(playNote)
            {
                if(cur_note>=melody->getLength())
                {
                    cur_note=0;
                    played=true;
                }

                noteDuration = floor(1000.0/(float)tempo->get(cur_note));
                if(noteTime==0)
                {
                    noteTime=millis()+noteDuration;
                }
                if(millis()>=noteTime)
                {
                    playNote=false;
                    noteTime=0;
                    return;
                }
                tone(musicPin, melody->get(cur_note),noteDuration);
            }
            else
            {
                pauseBetweenNotes = ceil((float)noteDuration * 1.30);
                if(noteTime==0)
                {
                    noteTime=millis()+pauseBetweenNotes;
                }
                if(millis()>=noteTime)
                {
                    playNote=true;
                    noteTime=0;
                    cur_note++;
                    tone(musicPin, 0,noteDuration);
                    return;
                }
            }
        }
    }
};

unsigned short overworld_theme[]=
{
    NOTE_E4, NOTE_E4, 0, NOTE_E4,
    0, NOTE_C4, NOTE_E4, 0,
    NOTE_G4, 0, 0,  0,
    NOTE_G3, 0, 0, 0,

    NOTE_C4, 0, 0, NOTE_G3,
    0, 0, NOTE_E3, 0,
    0, NOTE_A3, 0, NOTE_B3,
    0, NOTE_AS3, NOTE_A3, 0,

    NOTE_G3, NOTE_E4, NOTE_G4,
    NOTE_A4, 0, NOTE_F4, NOTE_G4,
    0, NOTE_E4, 0,NOTE_C4,
    NOTE_D4, NOTE_B3, 0, 0,

    NOTE_C4, 0, 0, NOTE_G3,
    0, 0, NOTE_E3, 0,
    0, NOTE_A3, 0, NOTE_B3,
    0, NOTE_AS3, NOTE_A3, 0,

    NOTE_G3, NOTE_E4, NOTE_G4,
    NOTE_A4, 0, NOTE_F4, NOTE_G4,
    0, NOTE_E4, 0,NOTE_C4,
    NOTE_D4, NOTE_B3, 0, 0
};

unsigned short overworld_theme_tempo[]=
{
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    9, 9, 9,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    9, 9, 9,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
};

unsigned short underworld_melody[] =
{
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
    NOTE_AS3, NOTE_AS4, 0,
    0,
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
    NOTE_AS3, NOTE_AS4, 0,
    0,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
    NOTE_DS3, NOTE_DS4, 0,
    0,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
    NOTE_DS3, NOTE_DS4, 0,
    0, NOTE_DS4, NOTE_CS4, NOTE_D4,
    NOTE_CS4, NOTE_DS4,
    NOTE_DS4, NOTE_GS3,
    NOTE_G3, NOTE_CS4,
    NOTE_C4, NOTE_FS4,NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
    NOTE_GS4, NOTE_DS4, NOTE_B3,
    NOTE_AS3, NOTE_A3, NOTE_GS3,
    0, 0, 0
};

unsigned short underworld_tempo[] =
{
    12, 12, 12, 12,
    12, 12, 6,
    3,
    12, 12, 12, 12,
    12, 12, 6,
    3,
    12, 12, 12, 12,
    12, 12, 6,
    3,
    12, 12, 12, 12,
    12, 12, 6,
    6, 18, 18, 18,
    6, 6,
    6, 6,
    6, 6,
    18, 18, 18,18, 18, 18,
    10, 10, 10,
    10, 10, 10,
    3, 3, 3
};

#endif // SONGS_H_INCLUDED
