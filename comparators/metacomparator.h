#ifndef METACOMPARATOR_H
#define METACOMPARATOR_H

#include "imagecomparator.h"

class MetaComparator : public ImageComparator
{
    Q_OBJECT

public:
    enum Type {
        CheckSum = 101,
        CreateDate = 102
    };
    MetaComparator();
    MetaComparator(QVector<FileInfo> *baseFiles, Queue<QString> *in, Queue<QString> *out = 0);



private:
    void run();


};

#endif // METACOMPARATOR_H
