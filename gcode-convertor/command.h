#ifndef COMMAND_H
#define COMMAND_H


#include <QString>


class Command
{

    public:
    QString type;
    QString x;
    QString y;
    QString z;
    QString i;
    QString j;

    ~Command();

    Command(QString withType, QString withX, QString withY, QString withZ, QString withI, QString withJ);
};


#endif // COMMAND_H
