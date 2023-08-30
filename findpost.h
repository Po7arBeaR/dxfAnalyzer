
#ifndef FINDPOST_H
#define FINDPOST_H


#include <QObject>
#include <QList>
#include <QPointF>
#include <QDebug>
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
//    bool operator<(const BatteryMark& other) const
//    {
//        return Top.cy<other.Top.cy;
//    }
    PostType getTopScale()
    {
        PostType result;
        result.cx=Top.cx*Scale;
        result.cy=Top.cy*Scale;
        result.radius=Top.radius*Scale;
        return result;
    }
    PostType getBottomScale()
    {
        PostType result;
        result.cx=Bottom.cx*Scale;
        result.cy=Bottom.cy*Scale;
        result.radius=Bottom.radius*Scale;
        return result;
    }

    PostType GetCenterPoint()
    {
        PostType result;

            if(Top.cy==Bottom.cy)
            {
                result.cx=(Bottom.cx+Top.cx)/2;
                result.cy=Top.cy;
              //  qDebug()<<result.cy;
            }

            if(Top.cx==Bottom.cx)
            {
                result.cy=(Bottom.cy+Top.cy)/2;
                result.cx=Top.cx;
               // qDebug()<<result.cy;
            }

            result.radius=Top.radius;

        return result;
    }
public:
    PostType Top;
    PostType Bottom;
 //   QPointF CenterPoint;
   // PostType ScaleTop;
   // PostType ScaleBottom;
    qreal Scale;
    int TopNumb;
    int BottomNumb;
    int Angel;

};

#endif // FINDPOST_H
