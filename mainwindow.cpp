#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->abortButton->hide();

    setWindowTitle("Wyszukaj obrazki");

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(ui->fileList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openImage(QModelIndex)));
    connect(ui->selectDir, SIGNAL(clicked()), this, SLOT(selectDir()));

    baseFiles = new QVector<FileInfo>;

    imageViewer = new ImageViewer();
    imageViewer->setWindowTitle("Podgląd obrazków bazowych");
    imageViewer->setFileList(baseFiles);

    settings = new Settings();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addFile(FileInfo *fileInfo)
{
    baseFiles->append(*fileInfo);
    ui->fileList->addItem(fileInfo->getPath());

    delete fileInfo;




}

void MainWindow::selectDir()
{
    QString dir;
    QString startPath = QDir::homePath();
    if(ui->dirPath->text().length() > 0) {
        startPath = ui->dirPath->text();
    }

    dir = QFileDialog::getExistingDirectory(
                       this,
                       "Wybierz katalog",
                        startPath);

    if(dir.size() < 1) return;

    ui->dirPath->setText(dir);
}

void MainWindow::invalidFile(FileInfo *fileInfo)
{
    invalidFiles.append(fileInfo->path);

    delete fileInfo;
}

void MainWindow::showInvalids()
{
    if(invalidFiles.isEmpty()) return;

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Informacja");
    Ui::InvalidFiles m_ui;
    m_ui.setupUi(dialog);

    m_ui.fileList->addItems(invalidFiles);
    connect(m_ui.closeButton, SIGNAL(clicked()), dialog, SLOT(accept()));

    dialog->exec();
    dialog->deleteLater();

}

void MainWindow::addFiles()
{
    QStringList files;
    files= QFileDialog::getOpenFileNames(
                        this,
                        "Wybierz pliki",
                        QDir::homePath(),
                        "obrazy (*.png *.jpeg *.jpg *.gif)");

    if(files.isEmpty()) return;

    ui->abortButton->show();
    ui->addButton->hide();
    ui->clearButton->hide();
    ui->startButton->setEnabled(false);

    AddFiles *addFiles = new AddFiles(this);
    addFiles->setFileList(files);
    connect(addFiles, SIGNAL(finished()), this, SLOT(filesAdditionFinished()));
    connect(addFiles, SIGNAL(newFile(FileInfo*)), this, SLOT(addFile(FileInfo*)));
    connect(addFiles, SIGNAL(invalidFile(FileInfo*)), this, SLOT(invalidFile(FileInfo*)));
    connect(ui->abortButton, SIGNAL(clicked()), addFiles, SLOT(abort()));
    addFiles->start();

}

void MainWindow::filesAdditionFinished()
{
    ui->abortButton->hide();
    ui->addButton->show();
    ui->clearButton->show();
    ui->startButton->setEnabled(true);

    if(ui->fileList->count() > 0) {
        ui->fileList->setCurrentRow(ui->fileList->count()-1);
    }

    imageViewer->updateList();

    showInvalids();
    invalidFiles.clear();
}



void MainWindow::openImage(const QModelIndex &index)
{
    imageViewer->updateList();
    imageViewer->setCurrent(index.row()+1);
    imageViewer->show();
}

void MainWindow::on_clearButton_clicked()
{
    if(ui->fileList->count() < 1 ) return;
    int reply = QMessageBox::question(this, "Pytanie",
                                  QString::fromUtf8("Czy na pewno chcesz wyczyścić listę plików"),
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply != QMessageBox::Yes) return;

    imageViewer->hide();
    ui->fileList->clear();
    baseFiles->clear();
}

void MainWindow::on_startButton_clicked()
{
    try {
        if(ui->fileList->count() < 1) {
            throw QString("Nie wybrano plików do wyszukania");
        }
        if(ui->dirPath->text().length() < 1) {
            throw QString("Nie wybrano katalogu do przeszukania");
        }
        QDir dir(ui->dirPath->text());
        if(!dir.exists()) {
            throw QString("Wybrany katalog nie istnieje");
        }
        if(!dir.isReadable()) {
            throw QString("Brak praw do odczytu katalogu");
        }

    } catch (QString err) {
        QMessageBox::warning(this,
                             "Błąd",
                             err,
                             QMessageBox::Ok);
        return;
    }

    SearchWindow *searchWindow = new SearchWindow(this, settings->clone());
    searchWindow->setSearchDir(ui->dirPath->text());
    searchWindow->setBaseFiles(baseFiles);
    searchWindow->show();
    searchWindow->start();

    this->hide();


}

void MainWindow::on_settingsButton_clicked()
{
    SettingsWindow *window = new SettingsWindow(this, settings);
    window->setWindowModality(Qt::ApplicationModal);
    window->show();
}

void MainWindow::on_compareCheckSum_clicked(bool checked)
{
    settings->setCompareChecksum(checked);
}

void MainWindow::on_compareDate_clicked(bool checked)
{
    settings->setCompareCreateDate(checked);
}

void MainWindow::on_compareContent_clicked(bool checked)
{
    settings->setCompareContents(checked);
}
