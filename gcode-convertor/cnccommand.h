#pragma once

#include <QString>


class CNCCommand
{

    public:
    QString type;
    double x;
    double *y;
    double *z;
    double i;
    double j;


    CNCCommand(double withX, double *y, double *z);
};

