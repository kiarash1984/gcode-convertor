#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentX = 0.0;
    currentY = 0.0;
    speed = 0.0;
    scaleFactor = 1.0;
    tempCommand = new Command("NA","NA","NA","NA","NA","NA");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    path = QFileDialog::getOpenFileName(this, "Open g-code file", QString("../../../../../../Desktop"));
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();

        getDiemension(line);

    }

    this->ui->startX->setText(QString::number(startXValue));
    this->ui->endX->setText(QString::number(endXValue));

    this->ui->startY->setText(QString::number(startYValue));
    this->ui->endY->setText(QString::number(endYValue));

    this->ui->lblWidth->setText(QString::number(gcodeWidth));
    this->ui->lblHeight->setText(QString::number(gcodeHeight));

    this->ui->btnConvert->setEnabled(true);

    file.close();
}

void MainWindow::saveToFile(QString path) {
    QStringList stringSplit = path.split("/");
    QString filename = "";
    for (int i = 0 ; i<stringSplit.count() - 1 ; i++) {
        filename = filename + stringSplit[i] + "/";
    }
    filename = filename + "gcode-converted.txt";

    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        qDebug() << "writing file" << "in path" << filename;
        QTextStream stream( &file );
        stream << cncCode << endl;
    } else {
        qDebug() << "error";
    }
}

void MainWindow::startConverting() {
    if (this->ui->inputWidth->text() != "" && this->ui->inputHeight->text() != "" && this->ui->btnScale->isChecked() == true) {
        double xDif = this->ui->inputWidth->text().toDouble() - gcodeWidth;
        double yDif = this->ui->inputHeight->text().toDouble() - gcodeHeight;
        if (xDif < 0.0 && yDif < 0) {
            if (xDif < yDif) {
                scaleFactor = this->ui->inputHeight->text().toDouble() / gcodeHeight;
            } else {
                scaleFactor = this->ui->inputWidth->text().toDouble() / gcodeWidth;
            }

        } else if (xDif < 0.0 && yDif > 0.0) {
            scaleFactor = this->ui->inputWidth->text().toDouble() / gcodeWidth;
        } else if (yDif < 0.0 && xDif > 0.0) {
            scaleFactor = this->ui->inputHeight->text().toDouble() / gcodeHeight;
        }
    }
    qDebug() << "SCALEFACTOR" << scaleFactor;

    if (this->ui->inputSpeed->text() != "") {
        speed = this->ui->inputSpeed->text().toDouble();
    }
}

void MainWindow::getDiemension(QString line) {
    tempCommand->type = checkType(line);
    if (tempCommand->type != "NA") {
        // set min and max X
        tempCommand->x = getComponentValue("X",line);
        if (tempCommand->x != "NA") {
            if (startXValue > tempCommand->x.toDouble()) {
                startXValue = tempCommand->x.toDouble();
            }
            if (endXValue < tempCommand->x.toDouble()) {
                endXValue = tempCommand->x.toDouble();
            }
        }
        // set min and max Y
        tempCommand->y = getComponentValue("Y",line);
        if (tempCommand->y != "NA") {
            if (startYValue > tempCommand->y.toDouble()) {
                startYValue = tempCommand->y.toDouble();
            }
            if (endYValue < tempCommand->y.toDouble()) {
                endYValue = tempCommand->y.toDouble();
            }
        }
    }
    gcodeWidth = qAbs(startXValue - endXValue);
    gcodeHeight = qAbs(startYValue - endYValue);
}

void MainWindow::saveCommand(QString line) {
    tempCommand->type = checkType(line);
    tempCommand->x = getComponentValue("X",line);
    tempCommand->y = getComponentValue("Y",line);
    tempCommand->z = getComponentValue("Z",line);
    tempCommand->i = getComponentValue("I",line);
    tempCommand->j = getComponentValue("J",line);
}

void MainWindow::noConvertNeeded() {
    this->writeToGcode(tempCommand->x, tempCommand->y);
    currentX = tempCommand->x.toDouble();
    currentY = tempCommand->y.toDouble();
}

void MainWindow::convertArcToLine() {

    QVector3D vectorA = QVector3D(currentX, currentY, 0.0);
    QVector3D vectorB = QVector3D(tempCommand->x.toDouble(), tempCommand->y.toDouble(), 0.0);
    QVector3D translationVector = QVector3D(-tempCommand->i.toDouble(), -tempCommand->j.toDouble(), 0.0);
    QVector3D iVector = QVector3D(tempCommand->i.toDouble(), tempCommand->j.toDouble(), 0.0);
    QVector3D vectorD = vectorA + iVector;
    QVector3D vectorE = vectorB - vectorD;

    double startPhi = this->getAngle(translationVector, QVector3D(1.0, 0.0, 0.0));
    if (translationVector[1] < 0) {
        startPhi = -startPhi;
    }
    double endPhi = this->getAngle(translationVector, vectorE);
    if (tempCommand->type == "G2") {
        endPhi = startPhi + endPhi;
    } else {
        endPhi = startPhi - endPhi;
    }
    double radius = iVector.length();
    if ((vectorA - vectorB).length() > 0.5) {
        // define step here
        /*
        double step = (startPhi - endPhi) / 3.0;
        if ((startPhi - endPhi) < 0.5) {
            step = 1.0;
        }
        */
        double step = qAbs(startPhi - endPhi) * radius / 2.0;


        /* converting here for G3 */
        if (tempCommand->type == "G3") {
            while (startPhi > endPhi) {
                /*qDebug() << "current angle" << startPhi;*/
               startPhi -= step;
               if (startPhi >= endPhi) {
                   this->calculatePoint(vectorD, startPhi, radius);
               } else {
                   this->writeToGcode(tempCommand->x, tempCommand->y);
                   // set current X and Y
                   currentX = tempCommand->x.toDouble();
                   currentY = tempCommand->y.toDouble();
                   /*qDebug() << "converted = " << "G1" << " X" << tempCommand->x << " " << "Y" << tempCommand->y;*/
               }
            }
        } else {
            // convert here for G2
            while (startPhi < endPhi) {
                /*qDebug() << "current angle" << startPhi;*/
               startPhi += step;
               if (startPhi <= endPhi) {
                   this->calculatePoint(vectorD, startPhi, radius);
               } else {
                   this->writeToGcode(tempCommand->x, tempCommand->y);
                   // set current X and Y
                   currentX = tempCommand->x.toDouble();
                   currentY = tempCommand->y.toDouble();
                   /*qDebug() << "converted = " << "G1" << " X" << tempCommand->x << " " << "Y" << tempCommand->y;*/
               }
            }
        }


    } else {
        this->noConvertNeeded();
    }


}

void MainWindow::writeToGcode(QString x, QString y) {
    cncCode = cncCode + "G1" + " X" + x + " Y" + y;
    if (speed != 0.0) {
        cncCode = cncCode + " F" + QString::number(speed);
    }
    cncCode = cncCode + "\n";
}

double MainWindow::getAngle(QVector3D vector, QVector3D axis) {
    return qAtan2(QVector3D().crossProduct(vector, axis).length(), QVector3D().dotProduct(vector, axis));
}

void MainWindow::calculatePoint(QVector3D vectorD, double angle, double radius) {
    double xValue = radius * qCos(angle) + vectorD.x();
    double yValue = radius * qSin(angle) + vectorD.y();
    this->writeToGcode(QString::number(xValue), QString::number(yValue));

    // set current X and Y
    currentX = xValue;
    currentY = yValue;
}


void MainWindow::copyExactCode() {

    cncCode = cncCode + tempCommand->type;
    if (tempCommand->x != "NA") {
        cncCode = cncCode + " X" + tempCommand->x;
    }
    if (tempCommand->y != "NA") {
        cncCode = cncCode + " Y" + tempCommand->y;
    }
    if (tempCommand->z != "NA") {
        cncCode = cncCode + " Z" + tempCommand->z;
    }
    if (speed != 0) {
        cncCode = cncCode + " F" + QString::number(speed);
    }
    cncCode = cncCode + "\n";
}

QString MainWindow::checkType(QString line) {
    QStringList g0Check = line.split("G0");
    QStringList g1Check = line.split("G1");
    QStringList g2Check = line.split("G2");
    QStringList g3Check = line.split("G3");

    if (g0Check.count() == 2) {
        return "G0";
    } else if (g1Check.count() == 2) {
        return "G1";
    } else if (g2Check.count() == 2) {
        return "G2";
    } else if (g3Check.count() == 2) {
        return "G3";
    } else {
        return "NA";
    }
}

QString MainWindow::getComponentValue(QString component, QString line) {

    QStringList values = line.split(component);
    if (values.count() == 2) {
        QString s = values[1];
        QString variables = "";
        for(int i=0;i<s.length(); i++)
        {
            if(s[i].isDigit()) {
                variables+=s[i];
            } else if(s[i] == "-" || s[i] == ".") {
                variables+=s[i];
            } else {
                break;
            }
        }
        if (component == "Z" || scaleFactor == 1.0) {
            return variables;
        } else {
            double value = variables.toDouble() * scaleFactor;
            return QString::number(value);
        }
    }
    return "NA";
}



void MainWindow::on_btnConvert_clicked()
{
    this->startConverting();

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();

        saveCommand(line);


        if (tempCommand->type == "G0" || tempCommand->type == "G1") {
            this->copyExactCode();
        } else if (tempCommand->type == "G2" || tempCommand->type == "G3") {
            /*qDebug() << "current line" << line;*/
            this->convertArcToLine();
        }
        if (tempCommand->x != QString("NA")) {
            currentX = tempCommand->x.toDouble();
        }
        if (tempCommand->y != QString("NA")) {
            currentY = tempCommand->y.toDouble();
        }

    }
    /*qDebug() << cncCode;*/
    this->saveToFile(path);


    file.close();
}
