#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#define LEFT    0
#define UP      1
#define RIGHT   2
#define DOWN    3
#define SCR_W   8
#define SCR_H   8
#define ALL_DOTS 64;

struct snake
{
    int *x,*y;

    int len;
    int dir;
    int apple_x,apple_y,apple_ox,apple_oy;

    bool moved,inGame;
    long mdel;

    snake(int px,int py,int l)
    {
        x=new int[64];
        y=new int[64];
        len=l;

        for (int z = 0; z < len; z++)
        {
            x[z] = px - z;
            y[z] = py;
        }
        moved=false;
        inGame=true;
        mdel=0;
        apple_x=0;
        apple_y=0;
        locateApple();
        dir=LEFT;
    }

    void checkCollision() {

        for (int z = len; z > 0; z--) {

            if ((z > 3) && (x[0] == x[z]) && (y[0] == y[z])) {
                inGame = false;
            }
        }

        if (y[0] > 7) {
            inGame = false;
        }

        if (y[0] < 0) {
            inGame = false;
        }

        if (x[0] > 7) {
            inGame = false;
        }

        if (x[0] < 0) {
            inGame = false;
        }
    }

    void move()
    {
        if(!moved)
        {
            for (int z = len; z > 0; z--)
            {
                x[z] = x[(z - 1)];
                y[z] = y[(z - 1)];
            }
            moved=true;
            mdel=millis()+250;
            if (dir==LEFT)
            {
                x[0] -= 1;
                return;
            }

            if (dir==RIGHT)
            {
                x[0] += 1;
                return;
            }

            if (dir==UP)
            {
                y[0] -= 1;
                return;
            }

            if (dir==DOWN)
            {
                y[0] += 1;
                return;
            }

        }
        else
        {
            if(millis()>mdel)
                moved=false;
        }
    }

    void checkApple() {

        if ((x[0] == apple_x) && (y[0] == apple_y))
        {
            len++;
            locateApple();
        }
    }

    void locateApple() {

        int r = rand()%8;
        apple_x = r;

        r = rand()%8;
        apple_y = r;
    }

    void update()
    {
        move();
        checkApple();
        checkCollision();
    }

    void draw(screenBuffer<unsigned char>* buf)
    {
        if(inGame)
        {
            int ox=x[len];
            int oy=y[len];
            for (int z = 0; z < len; z++)
            {
                buf->set(x[z],y[z],1);
            }
            buf->set(ox,oy,0);
            buf->set(apple_x,apple_y,1);
            if(apple_ox!=apple_x&&apple_oy!=apple_y)
            {
                buf->set(apple_ox,apple_oy,0);
                apple_ox=apple_x;
                apple_oy=apple_y;
            }
        }
        else
        {
            buf->reset();
        }
    }
};



#endif // SNAKE_H_INCLUDED
