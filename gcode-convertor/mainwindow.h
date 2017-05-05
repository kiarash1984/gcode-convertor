#pragma once

#include <QMainWindow>
#include "cnccommand.h"
#include "command.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>>
#include <QQuaternion>
#include <QVector3D>
#include <QtMath>
#include "progressionform.h"


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

    void on_btnConvert_clicked();

    void on_checkBox_clicked(bool checked);

private:

    void saveToFile(QString path);
    void saveCommand(QString line);
    void getDiemension(QString line);
    QString getComponentValue(QString component, QString line);
    QString checkType(QString line);
    void convertArcToLine();
    double getAngle(QVector3D vector, QVector3D axis);
    void writeToGcode(QString x, QString y);
    Command *tempCommand;
    double currentX;
    double currentY;
    double startXValue;
    double startYValue;
    double endXValue;
    double endYValue;
    double gcodeWidth;
    double gcodeHeight;
    double stockWidth;
    double stockHeight;
    double speed;
    double scaleFactor;
    QString path;
    bool isSolidWorkFile;

    QString lastGCommand;

    void calculatePoint(QVector3D vectorD, double angle, double radius);
    void noConvertNeeded();
    QString cncCode;

    void startConverting();
    void copyExactCode();
    Ui::MainWindow *ui;
    QVector3D tempVector1;

    ProgressionForm *progressionForm;
    int numberOfLines;

};

