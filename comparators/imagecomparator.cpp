#include "imagecomparator.h"

ImageComparator::ImageComparator() :
    QThread(0)
{
    this->in = 0;
    this->out = 0;
    this->baseFiles = 0;
}

ImageComparator::ImageComparator(QVector<FileInfo> *baseFiles, Queue<QString> *in, Queue<QString> *out) :
    QThread(0)
{
    this->in = in;
    this->out = out;
    this->baseFiles = baseFiles;
}

void ImageComparator::setInputQueue(Queue<QString> *in)
{
    this->in = in;
}

void ImageComparator::setOutputQueue(Queue<QString> *out)
{
    this->out = out;
}

void ImageComparator::setBaseFiles(QVector<FileInfo> *baseFiles)
{
    this->baseFiles = baseFiles;
}

void ImageComparator::accept(QString sourceFile, QString file, int type)
{
    emit found(sourceFile, file, type);
}

void ImageComparator::reject(QString file)
{
    if(out) {
        try {
            out->enqueue(file);
        } catch (...) {
            return;
        }
    }
}


