#ifndef FILEINFO_H
#define FILEINFO_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QCryptographicHash>
#include <QPixmap>

#include <exiv2/exiv2.hpp>
#include <opencv2/opencv.hpp>

class FileInfo
{
public:
    FileInfo();
    FileInfo(QString path);

    bool extractExif();
    bool generateMD5();
    bool canLoadImage();

    QString getPath() const;
    QString getChecksum() const;
    QString getDate() const;
    int getX() const;
    int getY() const;
    int getSize() const;
    bool success();



public:
    QString path;
    QByteArray checksum;
    QString date;
    int x;
    int y;
    int size;
};

#endif // FILEINFO_H
