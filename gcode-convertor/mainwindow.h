#pragma once

#include <QMainWindow>
#include <Eigen/Core>
#include "cnccommand.h"
#include "command.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>>
#include <QQuaternion>


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
    Eigen::Vector2d rotateAroundVector(Eigen::Vector2d rotateThis, Eigen::Vector2d around);
    void saveCommand(QString line);
    QString getComponentValue(QString component, QString line);
    QString checkType(QString line);
    void convertArcToLine();
    double getAngle(Eigen::Vector2d vector, Eigen::Vector2d axis);
    Eigen::Vector2d getPointsForRotation(Eigen::Vector2d vectorD,Eigen::Vector2d distVector ,double radius, double angle);
    Command *tempCommand;
    double currentX;
    double currentY;

    QString cncCode;


    void copyExactCode();
    Ui::MainWindow *ui;
    Eigen::Vector3f tempVector1;

};

