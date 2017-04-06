#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentX = 0.0;
    currentY = 0.0;

    tempX = new double(0.0);
    tempY = new double(0.0);
    tempZ = new double(0.0);
    tempI = new double(0.0);
    tempJ = new double(0.0);
    tempDoubleValue = new double(0.0);
    temp = new CNCCommand("G0",tempX,tempY,tempZ,tempI,tempJ);
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

        qDebug() << tempCommand->type;
        qDebug() << tempCommand->x;
        qDebug() << tempCommand->y;
        qDebug() << tempCommand->z;
        qDebug() << tempCommand->i;
        qDebug() << tempCommand->j;

        saveCommand(line);
        currentX = *temp->x;
        currentY = *temp->y;
    }

    file.close();
}
Command* MainWindow::saveCommand(QString line) {
    tempCommand->type = checkType(line);
    tempCommand->x = getComponentValue("X",line);
    tempCommand->y = getComponentValue("Y",line);
    tempCommand->z = getComponentValue("Z",line);
    tempCommand->i = getComponentValue("I",line);
    tempCommand->j = getComponentValue("J",line);

}

CNCCommand* MainWindow::saveCNCCommand(QString line) {
    QString type = checkType(line);
    if (type == NULL) {
        return NULL;
    } else {
        this->getDoubleValue("X",line);
        tempX = tempDoubleValue;
        this->getDoubleValue("Y",line);
        tempY = tempDoubleValue;
        this->getDoubleValue("Z",line);
        tempZ = tempDoubleValue;
        this->getDoubleValue("I",line);
        tempI = tempDoubleValue;
        this->getDoubleValue("J",line);
        tempJ = tempDoubleValue;

        temp->type = type;


        qDebug() << temp->i;
        qDebug() << temp->x;
        qDebug() << temp->y;
        qDebug() << temp->z;
        qDebug() << temp->j;
//        CNCCommand *t2 = new CNCCommand("SAD",NULL,NULL,NULL,NULL,NULL);
//        qDebug() << t2->x;
        return temp;
    }
}

void MainWindow::getDoubleValue(QString component, QString line) {
    QString temp = getComponentValue(component, line);
    if (temp.count() > 0) {
        *tempDoubleValue = double(temp.toDouble());
    }
    tempDoubleValue = NULL;
}

void MainWindow::copyExactCode() {


}

QString MainWindow::checkTypeOLD(QString line) {
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
        return NULL;
    }
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
//    qDebug() << " current " << line;
    if (values.count() == 2) {
        QString s = values[1];
        QString variables = "";
        for(int i=0;i<s.length(); ++i)
        {
                if(!s[i].isLetter())
                        variables+=s[i];
                else
                    return variables;
        }
        return variables;

        QRegExp rx("[A-Z]"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
        QStringList query = line.split(rx);
        if (query.count() >= 2) {
            return QString(query.count());
        }
        return values[1];
    }
    return "NA";
}

QString MainWindow::getComponentValueOLD(QString component, QString line) {
    QStringList values = line.split(component);
//    qDebug() << " current " << line;
    if (values.count() == 2) {
        QString s = values[1];
        QString variables = "";
        for(int i=0;i<s.length(); ++i)
        {
                if(!s[i].isLetter())
                        variables+=s[i];
                else
                    return variables;
        }
        return variables;

        QRegExp rx("[A-Z]"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
        QStringList query = line.split(rx);
        if (query.count() >= 2) {
            return QString(query.count());
        }
        return values[1];
    }
    return NULL;
}
