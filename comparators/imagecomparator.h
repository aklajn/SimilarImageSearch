#ifndef IMAGECOMPARATOR_H
#define IMAGECOMPARATOR_H

#include <QThread>
#include <QVector>
#include <QDebug>
#include <QObject>

#include "../fileinfo.h"
#include "../queue.h"

class ImageComparator : public QThread
{
    Q_OBJECT

public:
    ImageComparator();
    ImageComparator(QVector<FileInfo> *baseFiles, Queue<QString> *in, Queue<QString> *out = 0);

    void setInputQueue(Queue<QString> *in);
    void setOutputQueue(Queue<QString> *out);

    void setBaseFiles(QVector<FileInfo> *baseFiles);

signals:
    void found(QString sourceFile, QString file, int type);

protected:
    void accept(QString sourceFile, QString file, int type = 0);
    void reject(QString file);

    Queue<QString> *in;
    Queue<QString> *out;

    QVector<FileInfo> *baseFiles;
};

#endif // IMAGECOMPARATOR_H
