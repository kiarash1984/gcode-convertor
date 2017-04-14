#include "command.h"





Command::Command(QString withType, QString withX, QString withY, QString withZ, QString withI, QString withJ)
{
    this->type = withType;
    this->x = withX;
    this->y = withY;
    this->z = withZ;
    this->i = withI;
    this->j = withJ;
}
