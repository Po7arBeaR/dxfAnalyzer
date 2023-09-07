
#ifndef MARKPOINTTYPE_H
#define MARKPOINTTYPE_H

#include "findpost.h"
#include <QObject>


class MarkPointType : public QObject
{
    Q_OBJECT
public:
    explicit MarkPointType(QObject *parent = nullptr);
    ~MarkPointType();
    MarkPointType(const MarkPointType & obj);
    MarkPointType operator=(const MarkPointType &obj);
signals:

public:
    PostType MarkPoint;
    qreal scale;
public:
    PostType getScaleMarkPoint();
};

inline MarkPointType::~MarkPointType()
{

}

#endif // MARKPOINTTYPE_H
