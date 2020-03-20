#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QTimer>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QCloseEvent>
#include <QFileDialog>

#include "searchimages.h"
#include "fileinfo.h"
#include "imageviewer.h"
#include "settings.h"
#include "matcachegenerator.h"
#include "comparators/metacomparator.h"
#include "comparators/contentcomparator.h"

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = 0, Settings *settings = 0);
    ~SearchWindow();

    bool setSearchDir(QString dir);
    void start();
    void setBaseFiles(QVector<FileInfo> *files);
    void setSettings(Settings *settings);

public slots:
    void refreshPrimary();
    void refreshSecondary();
    void newFile(QString sourceFile, QString file, int type);

private slots:
    void on_newSearchButton_clicked();
    void on_filesList_doubleClicked(const QModelIndex &index);

    void primaryThreadFinished();
    void secondaryThreadFinished();

    void matCacheReady(QVector<cv::Mat> *files);

    void on_saveButton_clicked();

    void on_abortButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void startPrimaryThread(ImageComparator *thread);
    void startSecondaryThread(ImageComparator *thread);

    void runPrimaryThreads(quint8 count, QVector<FileInfo> *baseFiles,
                           Queue<QString> *in, Queue<QString> *out = 0);
    void runSecondaryThreads(quint8 count, QVector<cv::Mat> *files,
                           Queue<QString> *in, Queue<QString> *out = 0);


private:
    Ui::SearchWindow *ui;

    QWidget *parentWindow;
    QDir searchDir;

    ImageViewer *imageViewer;
    QVector<FileInfo> *baseFiles;
    QVector<FileInfo> *foundFiles;
    Queue<QString> *queue1;
    Queue<QString> *queue2;
    QTimer *timer;
    SearchImages *searchImages;
    Settings *settings;

    int primaryThreadInstances;
    int secondaryThreadInstances;

};

#endif // SEARCHWINDOW_H
