
#ifndef DXFREADER_H
#define DXFREADER_H
#include <QObject>
#include "dxflib/dl_dxf.h"
#include "dxflib/dl_creationadapter.h"
class dxfReader: public QObject, public DL_CreationAdapter
{
    Q_OBJECT
public:
    dxfReader(const QString &fileName, QObject *parent = nullptr);
    dxfReader( std::stringstream &fileName,QObject *parent=nullptr );
    virtual void addLine(const DL_LineData &data) override;
    //virtual void addText(const DL_TextData &data) override;
    virtual void addCircle(const DL_CircleData &data)override;
   // virtual void addArc(const DL_ArcData &data)override;
    QList<DL_LineData> dxfLines;
    QList<DL_TextData> dxfText;
    QList<DL_CircleData> dxfCircle;
    QList<DL_ArcData> dxfArc;

private:
    QString fileName;
};

#endif // DXFREADER_H
