#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, Settings *settings) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setWindowTitle("Zaawansowane ustawienia");
    ui->saveButton->setStyleSheet("QPushButton {font-weight: bold;}");
    this->settings = settings;
    readSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::setSettings(Settings *settings)
{
    this->settings = settings;
    readSettings();
}

void SettingsWindow::readSettings()
{
    if(!settings) return;

    ui->primaryThreads->setMaximum(settings->getCpuCores());
    ui->secondaryThreads->setMaximum(settings->getCpuCores()+2);

    ui->primaryThreads->setValue(settings->getPrimaryThreads());
    ui->secondaryThreads->setValue(settings->getSecondaryThreads());

    ui->extensions->setText(settings->getExtensions());

    ui->minWidth->setValue(settings->getMinWidth());
    ui->minHeight->setValue(settings->getMinHeight());

    ui->primarySimultaneously->setChecked(settings->getPrimarySimultaneously());
    ui->secondarySimultaneously->setChecked(settings->getSecondarySimultaneously());

    ui->secondaryType->setCurrentIndex(settings->getSecondaryType());

}

void SettingsWindow::on_resetButton_clicked()
{
    if(!settings) return;
    settings->resetValues();
    readSettings();
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    deleteLater();
}

void SettingsWindow::on_saveButton_clicked()
{
    settings->setPrimaryThreads(ui->primaryThreads->value());
    settings->setSecondaryThreads(ui->secondaryThreads->value());

    settings->setExtensions(ui->extensions->text());

    settings->setMinWidth(ui->minWidth->value());
    settings->setMinHeight(ui->minHeight->value());

    settings->setPrimarySimultaneously(ui->primarySimultaneously->isChecked());
    settings->setSecondarySimultaneously(ui->secondarySimultaneously->isChecked());

    settings->setSecondaryType(ui->secondaryType->currentIndex());


    this->deleteLater();
}
