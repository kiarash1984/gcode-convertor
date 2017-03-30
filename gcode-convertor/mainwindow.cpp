#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    double yValue = double(40);
    CNCCommand temp (20.0, &yValue, NULL);
//    if (temp.z != NULL) {
//        qDebug() << "salam";
//    }
//    qDebug() << temp.z;


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
        QString type = checkType(line);
        if (type != NULL) {
            qDebug() << saveCNCCommand(line);

        }
        QStringList fields = line.split("G2");
        for(int row=0; row < fields.count(); row++) {

 //            qDebug() << fields[row];

         }

    }

    file.close();
}

CNCCommand* MainWindow::saveCNCCommand(QString line) {
    QString type = checkType(line);
    if (type == NULL) {
        return NULL;
    } else {
        QString x = getComponentValue("X", line);
        QString y = getComponentValue("Y", line);
        QString z = getComponentValue("Z", line);
        QString i = getComponentValue("I", line);
        QString j = getComponentValue("J", line);


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
        return NULL;
    }
}

QString MainWindow::getComponentValue(QString component, QString line) {
    QStringList values = line.split("X");
    qDebug() << " current " << line;
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
