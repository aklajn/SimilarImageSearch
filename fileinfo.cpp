#include "fileinfo.h"

FileInfo::FileInfo()
{
    x = y = size = 0;
}

FileInfo::FileInfo(QString path)
{
    FileInfo();
    this->path = path;
}

bool FileInfo::extractExif()
{
    if(path.length() < 1) return false;
    QByteArray unicodePath = path.toUtf8();
    std::string file(path.toUtf8().constData(), unicodePath.size());

    Exiv2::Image::AutoPtr image;
    try {
        image = Exiv2::ImageFactory::open(file);
        if (!image.get()) return false;
        image->readMetadata();
    } catch (const std::exception&) {
        return false;
    }
    x = image->pixelWidth();
    y = image->pixelHeight();

    const Exiv2::ExifData& data = image->exifData();
    Exiv2::ExifData::const_iterator it = data.findKey(Exiv2::ExifKey("Exif.Image.DateTime"));
    if (it != data.end()) {
        date = QString::fromStdString(it->value().toString());
    }
    return true;
}

bool FileInfo::generateMD5()
{
    if(path.length() < 1) return false;
    QFile file(path);
    QByteArray data;

    if(!file.open(QIODevice::ReadOnly)) return false;
    size = file.size();
    data = file.read(10*1024); //TODO: czytac z konfigu

    checksum = QCryptographicHash::hash(data, QCryptographicHash::Md5);

    file.close();

    if(data.size() < 2*1024) { // TODO: czytac z konfigu
        return false;
    }
    return true;
}

bool FileInfo::canLoadImage()
{
    QPixmap pixmap(path);
    if(pixmap.width() > 0) return true;
    if(extractExif()) return true;
    cv::Mat img = cv::imread(path.toUtf8().constData());
    if(img.data) return true;
    return false;
}

QString FileInfo::getPath() const
{
    return path;
}

QString FileInfo::getChecksum() const
{
    return checksum;
}

QString FileInfo::getDate() const
{
    return date;
}

int FileInfo::getX() const
{
    return x;
}

int FileInfo::getY() const
{
    return y;
}

int FileInfo::getSize() const
{
    return size;
}

bool FileInfo::success()
{
    return x > 0 || y > 0 || date.length() > 0 || checksum.length() > 0;
}





