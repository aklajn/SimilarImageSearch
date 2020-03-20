#ifndef FASTPAIRQUEUE_H
#define FASTPAIRQUEUE_H

#include <QObject>

class FastPairQueue
{
public:
    FastPairQueue(int size) {
        size = size * 2;
        buffer = new quint16[size];
        this->size = size;
        top = 0;
        n = 0;
    }

    ~FastPairQueue() {
        delete buffer;
    }

    inline void enqueue(const quint16 x, const quint16 y) {
        if(top >= size) return;
        buffer[top] = x;
        buffer[top+1] = y;
        top = top + 2;
    }

    inline void dequeue(quint16 &x, quint16 &y) {
        if(n >= size || n >= top) {
            x = 0;
            y = 0;
            return;
        }
        x = buffer[n];
        y = buffer[n+1];
        n = n + 2;
    }

    bool empty() {
        return n >= top;
    }

    void clear() {
        n = 0;
        top = 0;
    }

private:
    quint16 *buffer;
    int size;
    int top;
    int n;
};

#endif // FASTPAIRQUEUE_H


