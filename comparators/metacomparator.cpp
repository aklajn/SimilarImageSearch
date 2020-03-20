#include "metacomparator.h"



MetaComparator::MetaComparator() :
    ImageComparator()
{

}

MetaComparator::MetaComparator(QVector<FileInfo> *baseFiles, Queue<QString> *in, Queue<QString> *out) :
    ImageComparator(baseFiles, in, out)
{

}


void MetaComparator::run()
{
    if(!baseFiles || !in)  {
        return;
    }

    FileInfo *file;
    QString path;
    int bfLen = baseFiles->count();

    while(true) {
        bool accepted = false;
        try {
            path = in->dequeue();
        } catch (...) {
            // kolejka zostala wylaczana zatem konczymy watek
            //return;
            break;
        }

        file = new FileInfo(path);
        if(!file->generateMD5()) {
            delete file;
            continue;
        }
        file->extractExif();
        int i;
        for(i=0;i<bfLen;i++) {
            if(baseFiles->at(i).checksum == file->checksum) {
                accept(baseFiles->at(i).path, file->path, MetaComparator::CheckSum);
                accepted = true;
                break;
            }
            if(file->getDate().length() > 4 &&
               file->getDate() == baseFiles->at(i).getDate()) {
                accept(baseFiles->at(i).path, file->path, MetaComparator::CreateDate);
                accepted = true;
                break;
            }

        }
        if(!accepted) {
            reject(file->path);
        }
        delete file;

    }

}
