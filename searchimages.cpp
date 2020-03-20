#include "searchimages.h"

SearchImages::SearchImages() :
    QThread(0)
{
    queue = 0;
    found = 0;
    m_stop = false;
}

SearchImages::SearchImages(QDir dir, QString ext, Queue<QString> *queue)
{
    this->dir = dir;
    this->addExtension(ext);
    this->queue = queue;
    found = 0;
    m_stop = 0;
}

void SearchImages::setDir(QDir dir)
{
    this->dir = dir;
}

void SearchImages::setQueue(Queue<QString> *queue)
{
    this->queue = queue;
}

void SearchImages::addExtension(QString ext)
{
    ext = ext.toLower();
    if(ext.indexOf(",") < 0) {
        extensions.append(ext);
        return;
    }
    QStringList exts = ext.split(',', QString::SkipEmptyParts);
    while(!exts.empty()) {
        ext = exts.takeFirst().trimmed();
        if(ext.at(0) == '.') {
            ext = ext.mid(1);
        }
        extensions.append(ext);
    }
}

int SearchImages::countFound()
{
    return found;
}

void SearchImages::stop()
{
    m_stop = true;
}

void SearchImages::run()
{

    if(!dir.exists()) return;
    if(!queue) {
        qDebug() << "Nie ustawiono kolejki";
        return;
    }



    if(extensions.count() < 1) {
        qDebug() << "Nie podano rozszerzeń obrazków";
        return;
    }


    QDirIterator it(dir, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        if(m_stop) break;
        QString path = it.next();
        if(path.right(2) == "..") continue;
        if(path.right(1) == ".") continue;

        ++found;
        QFileInfo file(path);

        if(file.isDir()) continue;
        if(file.size() < 2*1024) continue;
        if(!file.isReadable()) continue;

        if(extensions.indexOf(file.suffix().toLower()) < 0) continue;


        try {
            queue->enqueue(file.filePath());
        } catch (...) {
            break;
        }

    }
    queue->finish();

}
