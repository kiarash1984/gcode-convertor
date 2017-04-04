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
    temp = new CNCCommand("G0",tempX,tempY,tempZ,tempI,tempJ);
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

        if (saveCNCCommand(line) != NULL) {
            qDebug() << "NULL";
        }

        currentX = *temp->x;
        currentY = *temp->y;
    }

    file.close();
}

CNCCommand* MainWindow::saveCNCCommand(QString line) {
    QString type = checkType(line);
    if (type == NULL) {
        return NULL;
    } else {
        *tempX = double(getComponentValue("X", line).toDouble());
        *tempY = double(getComponentValue("Y", line).toDouble());
        *tempZ = double(getComponentValue("Z", line).toDouble());
        *tempI = double(getComponentValue("I", line).toDouble());
        *tempJ = double(getComponentValue("J", line).toDouble());
        temp->type = type;

        return temp;
    }
}

void MainWindow::copyExactCode() {


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
        return NULL;
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
    return NULL;
}
