#ifndef MATCACHEGENERATOR_H
#define MATCACHEGENERATOR_H

#include <QThread>
#include <QVector>
#include <QDebug>

#include <opencv2/opencv.hpp>


#include "fileinfo.h"

class MatCacheGenerator : public QThread
{
    Q_OBJECT
public:
    explicit MatCacheGenerator(QObject *parent = 0);
    MatCacheGenerator(QObject *parent, QVector<FileInfo> *files);

    static QVector<cv::Mat> *getImagesMat(QVector<FileInfo> *files);
    void setFiles(QVector<FileInfo> *files);



signals:
    void imagesMatReady(QVector<cv::Mat> *files);

public slots:

protected:
    void run();

private:

    static int getAvailableMemory();
    static int getTotalMemory();

#ifdef Q_OS_LINUX
    static int getMemoryInfo(QString type);
#endif


    QVector<FileInfo> *files;

};

#endif // MATCACHEGENERATOR_H
