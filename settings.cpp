#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    cpuCores = QThread::idealThreadCount();
    if(cpuCores < 1) {
        cpuCores = 2;
    }

    resetValues();
}

void Settings::resetValues()
{
    compareChecksum = true;
    compareCreateDate = true;
    compareContents = true;

    minWidth = 10;
    minHeight = 10;

    primaryThreads = 1;
    secondaryThreads = cpuCores;

    primarySimultaneously = true;
    secondarySimultaneously = false;

    extensions = "jpg,jpeg,png,gif";

    secondaryType = Settings::Accurate;
}

Settings *Settings::clone()
{
    Settings *settings = new Settings();

    settings->compareChecksum = compareChecksum;
    settings->compareCreateDate = compareCreateDate;
    settings->compareContents = compareContents;

    settings->minWidth = minWidth;
    settings->minHeight = minHeight;

    settings->primaryThreads = primaryThreads;
    settings->secondaryThreads = secondaryThreads;

    settings->primarySimultaneously = primarySimultaneously;
    settings->secondarySimultaneously = secondarySimultaneously;

    settings->extensions = extensions;

    return settings;
}
bool Settings::getCompareContents() const
{
    return compareContents;
}

void Settings::setCompareContents(bool value)
{
    compareContents = value;
}
bool Settings::getCompareCreateDate() const
{
    return compareCreateDate;
}

void Settings::setCompareCreateDate(bool value)
{
    compareCreateDate = value;
}
bool Settings::getCompareChecksum() const
{
    return compareChecksum;
}

void Settings::setCompareChecksum(bool value)
{
    compareChecksum = value;
}
int Settings::getPrimaryThreads() const
{
    return primaryThreads;
}

void Settings::setPrimaryThreads(int value)
{
    if(value < 1) value = 1;
    else if(value > cpuCores) value = cpuCores;

    primaryThreads = value;
}
int Settings::getSecondaryThreads() const
{
    return secondaryThreads;
}

void Settings::setSecondaryThreads(int value)
{
    if(value < 1) value = 1;
    else if(value > (cpuCores + 2)) value = cpuCores + 2;

    secondaryThreads = value;
}

int Settings::getCpuCores() const
{
    return cpuCores;
}
QString Settings::getExtensions() const
{
    return extensions;
}

void Settings::setExtensions(const QString &value)
{
    extensions = value;
}
int Settings::getMinWidth() const
{
    return minWidth;
}

void Settings::setMinWidth(int value)
{
    minWidth = value;
}
int Settings::getMinHeight() const
{
    return minHeight;
}

void Settings::setMinHeight(int value)
{
    minHeight = value;
}
bool Settings::getPrimarySimultaneously() const
{
    return primarySimultaneously;
}

void Settings::setPrimarySimultaneously(bool value)
{
    primarySimultaneously = value;
}
bool Settings::getSecondarySimultaneously() const
{
    return secondarySimultaneously;
}

void Settings::setSecondarySimultaneously(bool value)
{
    secondarySimultaneously = value;
}
int Settings::getSecondaryType() const
{
    return secondaryType;
}

void Settings::setSecondaryType(int value)
{
    secondaryType = value;
}











