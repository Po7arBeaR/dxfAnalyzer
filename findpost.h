
#ifndef FINDPOST_H
#define FINDPOST_H


#include <QObject>
#include <QList>
#include "dxflib/dl_creationadapter.h"
struct PostType
{
    PostType()
    {
        cx=0;
        cy=0;
        radius=0;
    }
    qreal cx;
    qreal cy;

    qreal radius;

};
class BatteryMark
{

public:
    BatteryMark();

public:
    PostType Top;
    PostType Bottom;
    int TopNumb;
    int BottomNumb;
    int Angel;
};

#endif // FINDPOST_H
