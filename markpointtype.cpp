
#include "markpointtype.h"

MarkPointType::MarkPointType(QObject *parent)
    : QObject{parent}
{
    scale=1;
}

MarkPointType::MarkPointType(const MarkPointType &obj)
{
    this->scale=obj.scale;
    this->MarkPoint=obj.MarkPoint;
}

MarkPointType MarkPointType::operator=(const MarkPointType &obj)
{
    if(this!=&obj)
    {
        this->scale=obj.scale;
        this->MarkPoint=obj.MarkPoint;
    }
    return *this;
}

PostType MarkPointType::getScaleMarkPoint()
{
    PostType pt;
    pt.cx=MarkPoint.cx*scale;
    pt.cy=MarkPoint.cy*scale;
    pt.radius=MarkPoint.radius*scale;
    return pt;
}

