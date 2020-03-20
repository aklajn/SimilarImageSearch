#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QCloseEvent>

#include "settings.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0, Settings *settings = 0);
    ~SettingsWindow();

    void setSettings(Settings *settings);

private slots:
    void on_resetButton_clicked();

    void on_saveButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:

    void readSettings();

    Ui::SettingsWindow *ui;
    Settings *settings;
};

#endif // SETTINGSWINDOW_H
