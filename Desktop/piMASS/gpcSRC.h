#ifndef GPCSRC_H
#define GPCSRC_H

class gpcSRC
{
public:
    U1  *pA, *pB;
    U4
    gpcSRC();
    virtual ~gpcSRC();

protected:
private:
    gpcSRC(const gpcSRC& other){}
    gpcSRC& operator=(const gpcSRC& b)
    {
        if( this == &b )
            return *this; // handle self assignment
        //assignment operator
        return *this;
    }
};

#endif // GPCSRC_H
