#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentX = 0.0;
    currentY = 0.0;

    tempCommand = new Command("NA","NA","NA","NA","NA","NA");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Open g-code file", QString("../../../../../../Desktop"));
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
            qDebug() << "current line" << line;
            this->convertArcToLine();
        }
        if (tempCommand->x != QString("NA")) {
            currentX = tempCommand->x.toDouble();
        }
        if (tempCommand->y != QString("NA")) {
            currentY = tempCommand->y.toDouble();
        }

    }
//    qDebug() << cncCode;
    file.close();
}

Eigen::Vector2d MainWindow::rotateAroundVector(Eigen::Vector2d rotateThis, Eigen::Vector2d around)
    {
    around.normalize();

    /*
    Eigen::Matrix3d m;
    m = Eigen::AngleAxis(0.25*M_PI, Vector3f::UnitX())
      * Eigen::AngleAxis(0.5*M_PI,  Vector3f::UnitY())
      * Eigen::AngleAxis(0.33*M_PI, Eigen::Vector3f(0,0,0));
*/

    /*
    const Vec3& direction, const Vec3& up = Vec3(0,1,0)
        Vec3 xaxis = Vec3::Cross(up, direction);
        xaxis.normalizeFast();

        Vec3 yaxis = Vec3::Cross(direction, xaxis);
        yaxis.normalizeFast();

        column1.x = xaxis.x;
        column1.y = yaxis.x;
        column1.z = direction.x;

        column2.x = xaxis.y;
        column2.y = yaxis.y;
        column2.z = direction.y;

        column3.x = xaxis.z;
        column3.y = yaxis.z;
        column3.z = direction.z;
        */
    }

void MainWindow::saveCommand(QString line) {
    tempCommand->type = checkType(line);
    tempCommand->x = getComponentValue("X",line);
    tempCommand->y = getComponentValue("Y",line);
    tempCommand->z = getComponentValue("Z",line);
    tempCommand->i = getComponentValue("I",line);
    tempCommand->j = getComponentValue("J",line);

}

void MainWindow::convertArcToLine() {

    Eigen::Vector2d vectorA = Eigen::Vector2d(currentX, currentY);
    Eigen::Vector2d vectorB = Eigen::Vector2d(tempCommand->x.toDouble(), tempCommand->y.toDouble());
    Eigen::Vector2d translationVector = Eigen::Vector2d(-tempCommand->i.toDouble(), -tempCommand->j.toDouble());
    Eigen::Vector2d iVector = Eigen::Vector2d(tempCommand->i.toDouble(), tempCommand->j.toDouble());
    Eigen::Vector2d vectorD = vectorA + iVector;
    Eigen::Vector2d vectorE = vectorB - vectorD;

    double startPhi = this->getAngle(translationVector, Eigen::Vector2d(1.0,0.0));
    if (translationVector[1] < 0) {
        startPhi = -startPhi;
    }
    double endPhi = this->getAngle(translationVector, vectorE);
    if (tempCommand->type == "G2") {
        endPhi = startPhi + endPhi;
    } else {
        endPhi = startPhi - endPhi;
    }
    double radius = iVector.norm();
    if ((vectorA - vectorB).norm() > 2.0) {
        double step = (startPhi - endPhi) / 3.0;
        if ((startPhi - endPhi) < 0.5) {
            step = 1.0;
        }
    }

    qDebug() << "startphi" << startPhi;
    qDebug() << "endphi" << endPhi;

//    qDebug() << vectorA[0] << "," << vectorA[1];
//    qDebug() << vectorB[0] << "," << vectorB[1];
//    qDebug() << vectorD[0] << "," << vectorD[1];
}
double MainWindow::getAngle(Eigen::Vector2d vector, Eigen::Vector2d axis) {
    double dotProduct = vector.dot(axis);
    double length = vector.norm() + axis.norm();
    double result = dotProduct/length;
    if (result > 1.0) {
        result = 1.0;
    }
    if (result < -1.0) {
        result = -1.0;
    }
    return atan2(Eigen::Vector3d(vector, 0.0).cross(Eigen::Vector3d(axis, 0.0)),
            Eigen::Vector3d(vector, 0.0).dot(Eigen::Vector3d(axis, 0.0)));

    return acos(result);
}

Eigen::Vector2d MainWindow::getPointsForRotation(Eigen::Vector2d vectorD,Eigen::Vector2d distVector ,double radius, double angle) {

}

void MainWindow::copyExactCode() {

    cncCode = cncCode + tempCommand->type + " ";
    if (tempCommand->x != "NA") {
        cncCode = cncCode + "X" + tempCommand->x + " ";
    }
    if (tempCommand->y != "NA") {
        cncCode = cncCode + "Y" + tempCommand->y + " ";
    }
    if (tempCommand->z != "NA") {
        cncCode = cncCode + "Z" + tempCommand->z + " ";
    }
    cncCode = cncCode + "\t";
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
        return variables;
    }
    return "NA";
}


