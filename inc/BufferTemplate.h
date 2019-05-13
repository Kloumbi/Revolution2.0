/*
 * BufferTemplate.h
 *
 *  Created on: Apr 24, 2019
 *      Author: rev
 */

#ifndef BUFFERTEMPLATE_H_
#define BUFFERTEMPLATE_H_

template <class T, uint16_t nSize>
class BufferTemplate {
public:
    BufferTemplate(){
        reset();
    }

    void reset(){
        in = 0;
        out = 0;
    }

    bool add(T e){
        if( (in + 1) != out )
        {
            data[in++ & (nSize-1)] = e;
            return 1;
        }
        return 0;
    }

    T rem(){
        if (!isEmpty())
        {
            return data[out++ & (nSize-1)] ;
        }
        return data[0];
    }

    bool isEmpty() const {
        return (out == in);
    }

private:
    unsigned int in ;
    unsigned int out;

    T data [nSize];
};




#endif /* BUFFERTEMPLATE_H_ */
