#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

#include "fileinfo.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

    void setFileList(QVector<FileInfo> *list);
    void setCurrent(int idx);

public slots:
    void updateList();

private slots:
    void previous();

    void next();

    void currentChanged(int idx = -1);

private:
    void setImage(QString path);

protected:
    void keyPressEvent(QKeyEvent *e);
    void showEvent(QShowEvent *e);
    void resizeEvent(QResizeEvent *);

private:
    Ui::ImageViewer *ui;
    QVector<FileInfo> *fileList;
    QTimer *timer;
    int last;
};

#endif // IMAGEVIEWER_H
