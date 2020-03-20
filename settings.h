#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QThread>

class Settings : public QObject
{
    Q_OBJECT
public:

    enum {
        Accurate = 0,
        VeryAccurate = 1
    };

    explicit Settings(QObject *parent = 0);

    void resetValues();
    Settings *clone();

    bool getCompareContents() const;
    void setCompareContents(bool value);

    bool getCompareCreateDate() const;
    void setCompareCreateDate(bool value);

    bool getCompareChecksum() const;
    void setCompareChecksum(bool value);

    int getPrimaryThreads() const;
    void setPrimaryThreads(int value);

    int getSecondaryThreads() const;
    void setSecondaryThreads(int value);

    int getCpuCores() const;

    QString getExtensions() const;
    void setExtensions(const QString &value);

    int getMinWidth() const;
    void setMinWidth(int value);

    int getMinHeight() const;
    void setMinHeight(int value);

    bool getPrimarySimultaneously() const;
    void setPrimarySimultaneously(bool value);

    bool getSecondarySimultaneously() const;
    void setSecondarySimultaneously(bool value);

    int getSecondaryType() const;
    void setSecondaryType(int value);

signals:

public slots:

private:


    bool compareChecksum;
    bool compareCreateDate;
    bool compareContents;

    int primaryThreads;
    int secondaryThreads;

    int minWidth;
    int minHeight;

    bool primarySimultaneously;
    bool secondarySimultaneously;

    int secondaryType;

    QString extensions;

    /* TODO: Dodac profile
     * bardzo szybkie: jpg/jpeg, exif+md5
     * szybkie: jpg/jpeg/png/gif, exif+md5
     * typowe: *, exif+md5, dokładna treść
     * dokładne: *,*, bardzo dokładna treść
     */

    // TODO: porowynywac 100x100

    // tego nie ruszać
    int cpuCores;

};

#endif // SETTINGS_H
