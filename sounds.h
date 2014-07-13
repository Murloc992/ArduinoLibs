#ifndef SONGS_H_INCLUDED
#define SONGS_H_INCLUDED
#include "pitches.h"

struct streamableSound
{
    unsigned short* melody,*tempo;
    unsigned short melody_len,tempo_len,cur_note,noteDuration,pauseBetweenNotes,noteTime;
    unsigned char musicPin;
    bool loop,played,playNote;

    streamableSound(unsigned char mpin,unsigned short *bufmel,unsigned short lenmel,unsigned short *buftmp,unsigned short lentmp,bool loop)
    {
        melody=bufmel;
        melody_len=lenmel;
        tempo=buftmp;
        tempo_len=lentmp;
        this->loop=loop;
        played=false;
        playNote=true;
        cur_note=0;
        musicPin=mpin;
    }

    void playAsync()
    {
        if(playNote)
        {
            if(cur_note>=melody_len/sizeof(unsigned short))
            {
                cur_note=0;
                played=true;
                return;
            }
            noteDuration = floor(1000.0/(float)pgm_read_word_near(tempo+cur_note));
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
            tone(musicPin, pgm_read_word_near(melody+cur_note),noteDuration);
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
};

/// DOES NOT STREAM FROM PROGMEM!
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
                    return;
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

#endif // SONGS_H_INCLUDED
