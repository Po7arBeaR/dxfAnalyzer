
#ifndef FINDPOST_H
#define FINDPOST_H


#include <QObject>
#include <QList>
#include "dxflib/dl_creationadapter.h"
struct PostType
{
    PostType()
    {

        ConnectNum=-1;
        Angel=-1;
    }
    DL_CircleData cd(double acx, double acy, double acz,
                     double aRadius);
    int ConnectNum;
    int Angel;
};
class FindPost:QObject
{
    Q_OBJECT
public:
    FindPost();
public:

};

#endif // FINDPOST_H
