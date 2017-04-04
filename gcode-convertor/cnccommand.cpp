#include "cnccommand.h"

CNCCommand::CNCCommand(double withX, double *y, double *z)
{


//    x = withX;
    this->y = y;
}


CNCCommand::CNCCommand(QString withType, double *withX, double *withY, double *withZ, double *withI, double *withJ) {
    this->type = withType;
    this->x = withX;
    this->y = withY;
    this->z = withZ;
    this->i = withI;
    this->j = withJ;
}

void CNCCommand::setCommand(QString withType, double *withX, double *withY, double *withZ, double *withI, double *withJ) {
    this->type = withType;
    this->x = withX;
    this->y = withY;
    this->z = withZ;
    this->i = withI;
    this->j = withJ;
}
