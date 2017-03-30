#pragma once

#include <QMainWindow>
#include <Eigen/Core>
#include "cnccommand.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    CNCCommand* saveCNCCommand(QString line);
    QString getComponentValue(QString component, QString line);
    QString checkType(QString line);

    CNCCommand *temp;

    Ui::MainWindow *ui;
    CNCCommand *tempCNCCommand;
    Eigen::Vector3f m_lastKnownPosOfTip;
};

