#include "searchwindow.h"
#include "ui_searchwindow.h"

SearchWindow::SearchWindow(QWidget *parent, Settings *settings) :
    QMainWindow(),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    parentWindow = parent;

    setWindowTitle("Wyszukiwanie");
    statusBar()->showMessage("Nie uruchomiony");

    timer = new QTimer(this);
    baseFiles = 0;
    queue1 = new Queue<QString>();
    queue2 = new Queue<QString>();
    foundFiles = new QVector<FileInfo>;

    imageViewer = new ImageViewer();
    imageViewer->setWindowTitle("Podgląd znalezionych obrazków");
    imageViewer->setFileList(foundFiles);

    this->settings = settings;

    ui->newSearchButton->hide();
    ui->saveButton->hide();

    primaryThreadInstances = 0;
    secondaryThreadInstances = 0;

    searchImages = 0;

    // TODO: przeniesc zarzadzanie watkami do osobnej klasy
    // generowanie tablicy Mat może robic niezaleznie


}

SearchWindow::~SearchWindow()
{
    timer->stop();
    queue1->stop();
    queue2->stop();
    imageViewer->deleteLater();

    delete foundFiles;
    delete settings;

    delete ui;
}

bool SearchWindow::setSearchDir(QString dir)
{
    QDir m_dir(dir);

    if(!m_dir.exists() || !m_dir.isReadable()) {
        return false;
    }

    searchDir = m_dir;
    ui->searchDir->setText(m_dir.absolutePath());
    return true;
}

void SearchWindow::start()
{

    if(!searchDir.exists()) {
        statusBar()->showMessage("Nie można przeszukać wskazanego katalogu");
        return;
    }

    if(!queue1 || !queue2 || !baseFiles || !settings) {
        statusBar()->showMessage("Niespodziewany błąd programu");
        return;
    }

    searchImages = new SearchImages(searchDir, settings->getExtensions(), queue1);
    searchImages->start();


    runPrimaryThreads(settings->getPrimaryThreads(), baseFiles, queue1, queue2);

    connect(timer, SIGNAL(timeout()), this, SLOT(refreshPrimary()));

    timer->setInterval(100);
    timer->start();

    statusBar()->showMessage("Trwa wyszukiwanie");


}

void SearchWindow::setBaseFiles(QVector<FileInfo> *files)
{
    baseFiles = files;
}

void SearchWindow::setSettings(Settings *settings)
{
    this->settings = settings;
}

void SearchWindow::refreshPrimary()
{
    if(!queue1) {
        statusBar()->showMessage("Błąd");
        return;
    }
    int all = queue1->countAll();
    int done = all - queue1->countCurrent();

    QString progress = QString::number(done) + " / " +
                       QString::number(all);
    QString found = QString::number(searchImages->countFound());

    ui->foundLabel->setText(found);
    ui->progressLabel1->setText(progress);
    ui->progressBar1->setMaximum(all);
    ui->progressBar1->setValue(done);


}

void SearchWindow::refreshSecondary()
{
    if(!queue2) {
        statusBar()->showMessage("Błąd, zatrzymano");
        return;
    }
    int all = queue2->countAll();
    int done = all - queue2->countCurrent();

    QString progress = QString::number(done) + " / " +
                       QString::number(all);

    ui->progressLabel2->setText(progress);
    ui->progressBar2->setMaximum(all);
    ui->progressBar2->setValue(done);
}

void SearchWindow::newFile(QString sourceFile, QString file, int type)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QString typeName("");
    switch(type) {
    case MetaComparator::CheckSum :
        typeName = "Suma kontrolna";
        break;
    case MetaComparator::CreateDate :
        typeName = "Data zrobienia";
        break;
    case ContentComparator::Content :
        typeName = "Zawartość";
        break;
    default:
        break;
    }


    item->setText(0, sourceFile);
    item->setText(1, file);
    item->setText(2, typeName);

    ui->filesList->addTopLevelItem(item);

    foundFiles->append(FileInfo(file));

}

void SearchWindow::on_newSearchButton_clicked()
{
    if(!parentWindow) {
        QMessageBox::warning(this,
                             "Błąd",
                             "Nie można wykonać akcji",
                             QMessageBox::Ok);
        return;
    }
    parentWindow->show();

    queue1->stop();
    queue2->stop();
    this->deleteLater();
}

void SearchWindow::on_filesList_doubleClicked(const QModelIndex &index)
{
    imageViewer->updateList();
    imageViewer->setCurrent(index.row()+1);
    imageViewer->show();
}

void SearchWindow::primaryThreadFinished()
{
    QObject *thread = sender();
    if(thread == 0) return;

    thread->deleteLater();
    --primaryThreadInstances;

    if(primaryThreadInstances < 1) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(refreshPrimary()));
        timer->stop();
        refreshPrimary();

        // bedzie nadpisany w przypadku uruchomienia drugiego zadania
        statusBar()->showMessage("Zakończono wyszukiwanie");

        // TODO: czytac z settings
        if(false) {
            ui->newSearchButton->show();
            ui->saveButton->show();
            ui->abortButton->hide();
        }

        if(queue1->countAll() < 1) {
            ui->progressBar1->setMaximum(1);
            ui->progressBar1->setValue(1);
        }

        // TODO: dodac w settingsach czy sprawdzac tresc obrazkow
        if(true) {
            queue2->finish();

            if(!queue2->isEnd()){ // jezeli nie jest zastopowana lub pusta

                MatCacheGenerator *generator;
                generator = new MatCacheGenerator(this, baseFiles);
                connect(generator, SIGNAL(imagesMatReady(QVector<cv::Mat>*)),
                        this, SLOT(matCacheReady(QVector<cv::Mat>*)));
                generator->start();
                statusBar()->showMessage("Przygotowanie bazy obrazków wzorcowych");
            }
        }
    }

}

void SearchWindow::secondaryThreadFinished()
{
    ContentComparator *thread = static_cast <ContentComparator *> (sender());
    if(!thread) return;

    --secondaryThreadInstances;
    if(secondaryThreadInstances < 1) {
        thread->deleteImagesMat();
    }
    thread->deleteLater();

    if(secondaryThreadInstances < 1) {
        timer->stop();
        statusBar()->showMessage("Zakończono wyszukiwanie");
        ui->newSearchButton->show();
        ui->saveButton->show();
        ui->abortButton->hide();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(refreshSecondary()));
        refreshSecondary();
        if(queue2->countAll() < 1) {
            ui->progressBar2->setMaximum(1);
            ui->progressBar2->setValue(1);
        }
    }

}

void SearchWindow::matCacheReady(QVector<cv::Mat> *files)
{
    if(!files) {
        statusBar()->showMessage("Za mało wolnej pamięci RAM. Zatrzymano");
        return;
    }
    statusBar()->showMessage("Trwa wyszukiwanie");

    runSecondaryThreads(settings->getSecondaryThreads(), files, queue2);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshSecondary()));
    timer->start();
}

void SearchWindow::closeEvent(QCloseEvent *event)
{
    queue1->stop();
    queue2->stop();
    event->accept();
}

void SearchWindow::startPrimaryThread(ImageComparator *thread)
{
    connect(thread, SIGNAL(finished()), this, SLOT(primaryThreadFinished()));
    connect(thread, SIGNAL(found(QString,QString,int)),
            this, SLOT(newFile(QString,QString,int)));

    ++primaryThreadInstances;
    thread->start();
}

void SearchWindow::startSecondaryThread(ImageComparator *thread)
{
    connect(thread, SIGNAL(finished()), this, SLOT(secondaryThreadFinished()));
    connect(thread, SIGNAL(found(QString,QString,int)),
            this, SLOT(newFile(QString,QString,int)));

    ++secondaryThreadInstances;
    thread->start();
}

void SearchWindow::runPrimaryThreads(quint8 count, QVector<FileInfo> *baseFiles,
                                     Queue<QString> *in, Queue<QString> *out)
{
    ImageComparator *imageComparator;

    for(int i=0;i<count;i++) {
        imageComparator = new MetaComparator(baseFiles, in, out);
        startPrimaryThread(imageComparator);
    }
}

void SearchWindow::runSecondaryThreads(quint8 count, QVector<cv::Mat> *files,
                                       Queue<QString> *in, Queue<QString> *out)
{
    ImageComparator *imageComparator;

    for(int i=0;i<count;i++) {
        imageComparator = new ContentComparator(files, baseFiles, in, out);
        startSecondaryThread(imageComparator);
    }
}

void SearchWindow::on_saveButton_clicked()
{
    QString path;
    path = QFileDialog::getSaveFileName(this,"Wybierz plik", QDir::homePath(),
                                        "CSV (*.csv)");
    if(path.length() < 1) return;

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)) return;


    int i;
    int iCount = ui->filesList->topLevelItemCount();
    QString line;

    line = "\"Oryginalny plik\", \"Znaleziony plik\", \"Podobieństwo\"\r\n";
    file.write(line.toUtf8());

    for(i=0;i<iCount;i++) {
        path = ui->filesList->topLevelItem(i)->text(0);
        line = "\"" + path.replace("\"", "\"\"") + "\", ";
        path = ui->filesList->topLevelItem(i)->text(1);
        line += "\"" + path.replace("\"", "\"\"") + "\", ";
        path = ui->filesList->topLevelItem(i)->text(2);
        line += "\"" + path.replace("\"", "\"\"") + "\"\r\n";
        file.write(line.toUtf8());
    }

    file.close();
    statusBar()->showMessage("Zapisano");
}

void SearchWindow::on_abortButton_clicked()
{
    if(searchImages) {
        searchImages->stop();
    }
    queue1->stop();
    queue2->stop();
}
