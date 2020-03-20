#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    fileList = NULL;
    ui->widget->setFocus();

    timer = new QTimer(this);
    timer->setInterval(5000);
    timer->start();

    connect(ui->previous, SIGNAL(clicked()), this, SLOT(previous()));
    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->current, SIGNAL(valueChanged(int)), this, SLOT(currentChanged(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateList()));

    QPalette Pal;
    Pal.setColor(QPalette::Background, QColor(0x64, 0x64, 0x64));
    ui->widget->setAutoFillBackground(true);
    ui->widget->setPalette(Pal);

    last = 1;

}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::setImage(QString path)
{
    QPixmap image(path);
    QWidget *widget = static_cast <QWidget*> (ui->imageLabel->parent());


    ui->filePath->setText(path);
    int x = widget->width();
    int y = widget->height();

    if(image.width() > x || image.height() > y) {
        image = image.scaled(x,y, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    int offset = 0;
    if(x> image.width()) offset = (x - image.width()) >> 1;
    ui->imageLabel->setGeometry(offset,0,x,y);

    ui->imageLabel->setPixmap(image);

}



void ImageViewer::keyPressEvent(QKeyEvent *e)
{

    if(e->key() == Qt::Key_Left) {
        previous();
        e->accept();
    }
    else if(e->key() == Qt::Key_Right) {
        next();
        e->accept();
    }
}

void ImageViewer::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    currentChanged();
}

void ImageViewer::resizeEvent(QResizeEvent *)
{
    currentChanged();
}

void ImageViewer::setFileList(QVector<FileInfo> *list)
{
    fileList = list;
}

void ImageViewer::setCurrent(int idx)
{
    ui->current->setValue(idx);
}

void ImageViewer::updateList()
{
    if(!fileList) return;
    int size = fileList->size();
    if(size < 1) {
        ui->imageLabel->setPixmap(QPixmap());
        ui->current->setMaximum(1);
        return;
    }
    if(size == ui->current->value()) return;
    ui->current->setMaximum(size);
    ui->count->setText(QString::number(size));
    if(last != ui->current->value()) {
        currentChanged();
    }
}

void ImageViewer::previous()
{
    ui->current->setValue(ui->current->value()-1);
}

void ImageViewer::next()
{
    ui->current->setValue(ui->current->value()+1);
}

void ImageViewer::currentChanged(int idx)
{
    if(!fileList) return;
    if(idx == -1) idx = ui->current->value();
    if(idx > fileList->size()) return;
    last = idx;
    idx--;
    setImage(fileList->at(idx).path);
    ui->widget->setFocus();
}
