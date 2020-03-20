#ifndef QUEUE_H
#define QUEUE_H

#include <QList>
#include <QMutex>
#include <QWaitCondition>



template <class T>
class Queue
{


private:
    QList<T> list;     // elements
    QMutex mutex;
    QWaitCondition empty;
    QWaitCondition full;
    volatile unsigned int count;
    volatile unsigned int m_sum;
    volatile unsigned int max;
    volatile bool m_end;
    volatile bool m_finish;

public:

    Queue()
    {
        count = 0;
        max = -1;
        m_sum = 0;
        m_end = false;
        m_finish = false;
    }

    void enqueue(T const &value) {
        mutex.lock();
        if(count == max) full.wait(&mutex);
        if(m_end) {
            mutex.unlock();
            throw -1;
        }
        list.append(value);
        if(count==0) empty.wakeOne();
        ++count;
        ++m_sum;
        mutex.unlock();
    }

    T dequeue() {
        mutex.lock();
        if(count == 0 && !m_finish) empty.wait(&mutex);
        if(m_end || (m_finish && count == 0)) {
            mutex.unlock();
            throw -1;
        }
        T value = list.takeFirst();
        if(max==count) full.wakeOne();
        --count;
        mutex.unlock();
        return value;
    }

    /* Wylacza calkowicie kolejke i ja czysci*/
    void stop() {
        mutex.lock();
        m_end = true;
        list.clear();
        empty.wakeAll();
        full.wakeAll();
        mutex.unlock();
    }

    int countAll() {
        return m_sum;
    }

    int countCurrent() {
        return count;
    }

    /* Czeka z wylaczeniem kolejki do ostatniego elementu*/
    void finish() {
        mutex.lock();
        m_finish = true;
        empty.wakeAll();
        mutex.unlock();
    }


    bool isEnd() {
        mutex.lock();
        if(m_end || (m_finish && count == 0)) {
            mutex.unlock();
            return true;
        }
        mutex.unlock();
        return false;
    }
};


#endif // QUEUE_H
