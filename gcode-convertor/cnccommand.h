#pragma once

#include <QString>


class CNCCommand
{
//test

    public:
    QString type;
    double *x;
    double *y;
    double *z;
    double *i;
    double *j;

    ~CNCCommand();
    void setCommand(QString withType, double *withX, double *withY, double *withZ, double *withI, double *withJ);
    CNCCommand(double withX, double *y, double *z);
    CNCCommand(QString withType, double *withX, double *withY, double *withZ, double *withI, double *withJ);
};

