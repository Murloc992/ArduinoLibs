#ifndef MAXCONTROL_H_INCLUDED
#define MAXCONTROL_H_INCLUDED

// define max7219 registers
static unsigned char max7219_reg_noop         =0x00;
static unsigned char max7219_reg_digit0       =0x01;
static unsigned char max7219_reg_digit1       =0x02;
static unsigned char max7219_reg_digit2       =0x03;
static unsigned char max7219_reg_digit3       =0x04;
static unsigned char max7219_reg_digit4       =0x05;
static unsigned char max7219_reg_digit5       =0x06;
static unsigned char max7219_reg_digit6       =0x07;
static unsigned char max7219_reg_digit7       =0x08;
static unsigned char max7219_reg_decodeMode   =0x09;
static unsigned char max7219_reg_intensity    =0x0a;
static unsigned char max7219_reg_scanLimit    =0x0b;
static unsigned char max7219_reg_shutdown     =0x0c;
static unsigned char max7219_reg_displayTest  =0x0f;

class maxController
{
private:
    char _dataPin,_loadPin,_clockPin,_maxCount;
    unsigned long frameTime,frameLimit;
public:
    maxController(char dataPin,char loadPin,char clockPin,char maxCount)
    {
        _dataPin=dataPin;
        _loadPin=loadPin;
        _clockPin=clockPin;
        _maxCount=maxCount;

        pinMode(_dataPin, OUTPUT);
        pinMode(_loadPin,  OUTPUT);
        pinMode(_clockPin,   OUTPUT);

        maxAll(max7219_reg_scanLimit, 0x07);
        maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
        maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
        maxAll(max7219_reg_displayTest, 0x00); // no display test
        cls();
        maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set

        drawFrame=true;
        frameTime=0;
        frameLimit=34;
    }

    void cls()
    {
        for (char e=1; e<=8; e++)      // empty registers, turn all LEDs off
        {
            maxAll(e,0);
        }
    }

    void putByte(char memory_block)
    {
        char i = 8;
        char mask;
        while(i > 0)
        {
            mask = 0x01 << (i - 1);      // get bitmask
            digitalWrite( _clockPin, LOW);   // tick
            if (memory_block & mask)             // choose bit
            {
                digitalWrite(_dataPin, HIGH);// send 1
            }
            else
            {
                digitalWrite(_dataPin, LOW); // send 0
            }
            digitalWrite(_clockPin, HIGH);   // tock
            --i;                         // move to lesser bit
        }
    }

    void maxSingle( char reg, char col)
    {
//maxSingle is the "easy"  function to use for a     //single max7219

        digitalWrite(_loadPin, LOW);       // begin
        putByte(reg);                  // specify register
        putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
        digitalWrite(_loadPin, LOW);       // and load da shit
        digitalWrite(_loadPin,HIGH);
    }

    void maxAll (char reg, char col)      // initialize  all  MAX7219's in the system
    {
        int c = 0;
        digitalWrite(_loadPin, LOW);  // begin
        for ( c =1; c<= _maxCount; c++)
        {
            putByte(reg);  // specify register
            putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
        }
        digitalWrite(_loadPin, LOW);
        digitalWrite(_loadPin,HIGH);
    }

    void maxOne(char maxNr, char reg, char col)
    {
//maxOne is for adressing different MAX7219's,
//whilele having a couple of them cascaded

        int c = 0;
        digitalWrite(_loadPin, LOW);  // begin

        for ( c = _maxCount; c > maxNr; c--)
        {
            putByte(0);    // means no operation
            putByte(0);    // means no operation
        }

        putByte(reg);  // specify register
        putByte(col);//((data & 0x01) * 256) + data >> 1); // put data

        for ( c =maxNr-1; c >= 1; c--)
        {
            putByte(0);    // means no operation
            putByte(0);    // means no operation
        }

        digitalWrite(_loadPin, LOW); // and load da shit
        digitalWrite(_loadPin,HIGH);
    }

    template<typename T>
    void draw(screenBuffer<T>* buf)
    {
        if(drawFrame)
        {
            int temp=0;
            for(int y=0; y<8; y++)
            {
                temp=0;
                for(int x=0; x<8; x++)
                {
                    if(buf->get(x,y)>0)
                    {
                        temp=temp+(1<<x);
                    }
                }
                maxSingle(y+1,temp);
            }
            drawFrame=false;
        }
        else
        {
            if(frameTime==0)
            {
                frameTime=millis()+frameLimit;
            }
            if(millis()>=frameTime)
            {
                frameTime=0;
                drawFrame=true;
            }
        }
    }

//    void drawAnim(int bytes[8][8][8])
//    {
//        for(int i=0; i<8; i++)
//        {
//            draw(bytes[i]);
//            delay(33.33334);
//        }
//    }
};

#endif // MAXCONTROL_H_INCLUDED
