#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDialog>

#include "addfiles.h"
#include "imageviewer.h"
#include "searchwindow.h"
#include "settingswindow.h"

#include "ui_invalidfiles.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void showInvalids();

public slots:
    void addFiles();
    void filesAdditionFinished();
    void invalidFile(FileInfo *fileInfo);
    void addFile(FileInfo *fileInfo);
    void selectDir();

private slots:
    void openImage(const QModelIndex &index);

    void on_clearButton_clicked();
    void on_startButton_clicked();
    void on_settingsButton_clicked();
    void on_compareCheckSum_clicked(bool checked);
    void on_compareDate_clicked(bool checked);
    void on_compareContent_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QVector<FileInfo> *baseFiles;
    QStringList invalidFiles;
    ImageViewer *imageViewer;
    Settings *settings;
};

#endif // MAINWINDOW_H
