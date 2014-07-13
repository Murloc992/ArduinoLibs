#ifndef BUFFERS_H_INCLUDED
#define BUFFERS_H_INCLUDED

#ifndef ARDUSIM
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#else
int freeRam(){return -1;}
#endif

template<typename T>
struct memory_block
{
    T *mem;
    size_t sz;
    memory_block(size_t sizeBytes)
    {
        mem=(T*)malloc(sizeBytes);
        this->sz=sizeBytes;
        memset(mem,0,this->sz);
    }
    memory_block(size_t sizeBytes, T* data)
    {
        mem=(T*)malloc(sizeBytes);
        this->sz=sizeBytes;
        memcpy(mem,data,sizeBytes);
    }
    ~memory_block()
    {
        free(mem);
        sz=0;
    }
};

template<typename T>
class buffer
{
protected:
    memory_block<T> *_d;
    int _length;
public:
    buffer()
    {
        _length=0;
    }

    buffer(int length)
    {
        _length=length;
        _d=new memory_block<T>(_length*sizeof(T));
    }

    buffer(memory_block<T>* dt)
    {
        _length=dt->sz/sizeof(T);
        _d=dt;
    }

    buffer(const buffer& other)
    {
        *this=other;
    }

    buffer<T> operator = (const buffer<T>& other)
    {
        this->_d=other._d;
        _length=other._length;
        return *this;
    }

    ~buffer()
    {
        delete _d;
    }

    void print()
    {
        for(int i=0; i<_length; i++)
        {
            Serial.print(_d->mem[i]);
            Serial.print(" ");
        }
        Serial.print("\n");
    }

    void set(int index,T value)
    {
        if(index>_length)
            return;
        _d->mem[index]=value;
    }

    T get(int index)
    {
        if(index>_length)
            return -1;
        return _d->mem[index];
    }
    void clear()
    {
        delete _d;
        _d=new memory_block<T>(0);
    }

    void reset()
    {
        unsigned short sz=_d->sz;
        delete _d;
        _d=new memory_block<T>(sz);
    }

    void push_back(T item)
    {
        _d->mem=(T*)realloc(_d->mem,_d->sz+sizeof(T));
        _d->sz+=sizeof(T);
        _length=_d->sz/sizeof(T);
        set(_length-1,item);
    }

    void push_front(T item)
    {
        _d->mem=(T*)realloc(_d->mem,_d->sz+sizeof(T));
        _d->sz+=sizeof(T);
        _length=_d->sz/sizeof(T);
        _d->mem=(T*)memmove(_d->mem+1,_d->mem-1,_d->sz);
        set(0,item);
    }

    int getLength()
    {
        return _length;
    }
};

template<typename T>
class screenBuffer:public buffer<T>
{
public:
    int _sx,_sy;

    screenBuffer(int sx,int sy):buffer<T>(sx*sy)
    {
        _sx=sx;
        _sy=sy;
    }

    void set(int x,int y,T val)
    {
        buffer<T>::set(_sx*y+x,val);
    }

    int get(int x,int y)
    {
        return buffer<T>::get(_sx*y+x);
    }

    void blit(int startx,int starty,screenBuffer<T> *b)
    {
        int endx=(startx+b->_sx)<=_sx?(startx+b->_sx):_sx;
        int endy=(starty+b->_sy)<=_sy?(starty+b->_sy):_sy;
        int cx=0,cy=0;
        for(int i=starty; i<endy; i++, cy++)
        {
            cx=0;
            for(int j=startx; j<endx; j++, cx++)
            {
                memcpy(&this->_d->mem[_sx*i+j],&b->_d->mem[b->_sx*cy+cx],sizeof(T));
            }
        }
    }
};

#endif // BUFFERS_H_INCLUDED
