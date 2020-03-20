#include "addfiles.h"

AddFiles::AddFiles(QObject *parent) :
    QThread(parent)
{
    end = false;
}

void AddFiles::setFileList(QStringList &files)
{
    this->files = files;
}

void AddFiles::abort()
{
    end = true;
}

void AddFiles::run()
{
    FileInfo *info;

    while(!files.isEmpty()) {
        if(end) break;

        info = new FileInfo(files.takeFirst());
        if(!info->canLoadImage()) {
            emit invalidFile(info);
            continue;
        }
        info->extractExif();
        info->generateMD5();

        emit newFile(info);
    }
    deleteLater();
}

