#include "matcachegenerator.h"

MatCacheGenerator::MatCacheGenerator(QObject *parent) :
    QThread(parent)
{
    files = 0;
}

MatCacheGenerator::MatCacheGenerator(QObject *parent, QVector<FileInfo> *files) :
    QThread(parent)
{
    this->files = files;
}

QVector<cv::Mat> *MatCacheGenerator::getImagesMat(QVector<FileInfo> *files)
{
    int i;
    int fCount = files->count();
    QVector<cv::Mat> *imagesMat = new QVector<cv::Mat>();
    int minAvailableMemory = 200; // 200 MB

    if(getTotalMemory() < 800) { // mniej niz 800 MB ramu
        minAvailableMemory = 100; // 100MB
    }

    for(i=0;i<fCount;i++) {
        cv::Mat image = cv::imread(files->at(i).path.toUtf8().constData(),
                                   CV_LOAD_IMAGE_GRAYSCALE);
        if(getAvailableMemory() < minAvailableMemory) {
            imagesMat->clear();
            delete imagesMat;
            return 0;
        }
        imagesMat->append(image);
    }
    return imagesMat;
}

void MatCacheGenerator::setFiles(QVector<FileInfo> *files)
{
    this->files = files;
}

void MatCacheGenerator::run()
{
    QVector<cv::Mat> *imagesMat  = getImagesMat(files);
    emit imagesMatReady(imagesMat);

}



int MatCacheGenerator::getAvailableMemory()
{

#ifdef Q_OS_LINUX
    return getMemoryInfo("MemFree") + getMemoryInfo("Buffers") +
           getMemoryInfo("Cached");
#else
    int defaultValue = 512; // MB
    return defaultValue;
#endif
}

int MatCacheGenerator::getTotalMemory()
{
#ifdef Q_OS_LINUX
    return getMemoryInfo("MemTotal");

#else
    int defaultValue = 1024; // MB
    return defaultValue;
#endif
}

#ifdef Q_OS_LINUX
int MatCacheGenerator::getMemoryInfo(QString type)
{
    QFile file("/proc/meminfo");
    if(!file.open(QIODevice::ReadOnly)) return -1;

    QString data = QString::fromUtf8(file.read(2048));
    if(data.length() < 1) return -1;
    type.append(":");
    int pos = data.indexOf(type);
    if(pos < 0) return -1;
    data = data.mid(pos + type.length());
    pos = data.indexOf("kB");
    if(pos < 0) return -1;
    data = data.left(pos).trimmed();
    bool result;
    int value = data.toInt(&result);
    if(!result) return -1;
    return value / 1024;

}
#endif

