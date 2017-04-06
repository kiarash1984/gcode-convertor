#pragma once

#include <QMainWindow>
#include <Eigen/Core>
#include "cnccommand.h"
#include "command.h"
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
    Command* saveCommand(QString line);
    QString getComponentValue(QString component, QString line);
    QString getComponentValueOLD(QString component, QString line);
    QString checkType(QString line);
    QString checkTypeOLD(QString line);
    void getDoubleValue(QString component, QString line);

    CNCCommand *temp;
    Command *tempCommand;
    double *tempX;
    double *tempY;
    double *tempZ;
    double *tempI;
    double *tempJ;

    double *tempDoubleValue;

    double currentX;
    double currentY;

    QString cncCode;


    void copyExactCode();
    Ui::MainWindow *ui;
    CNCCommand *tempCNCCommand;
    Eigen::Vector3f m_lastKnownPosOfTip;
};

