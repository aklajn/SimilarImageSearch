#ifndef SEARCHIMAGES_H
#define SEARCHIMAGES_H

#include <QThread>
#include <QDirIterator>
#include <QDebug>

#include "queue.h"

class SearchImages : public QThread
{
    Q_OBJECT
public:
    SearchImages();
    SearchImages(QDir dir, QString ext, Queue<QString> *queue);

    void setDir(QDir dir);
    void setQueue(Queue<QString> *queue);
    void addExtension(QString ext);
    int countFound();
    void stop();

private:
    void run();

    QDir dir;
    Queue<QString> *queue;
    QStringList extensions;
    volatile int found;
    volatile bool m_stop;

signals:

public slots:

};

#endif // SEARCHIMAGES_H
