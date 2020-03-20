#ifndef ADDFILES_H
#define ADDFILES_H

#include <QThread>
#include <QStringList>
#include <QDebug>

#include "fileinfo.h"


class AddFiles : public QThread
{
    Q_OBJECT
public:
    explicit AddFiles(QObject *parent = 0);
    void setFileList(QStringList &files);

signals:
    void newFile(FileInfo *fileInfo);
    void invalidFile(FileInfo *fileInfo);

public slots:
    void abort();

private:
    void run();

    QStringList files;
    volatile bool end;

};

#endif // ADDFILES_H
