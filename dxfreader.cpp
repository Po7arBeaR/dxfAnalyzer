
#include "dxfreader.h"
#include <QDebug>



dxfReader::dxfReader(const QString &fileName, QObject *parent)
{
    DL_Dxf *dxf = new DL_Dxf;

    if (!dxf->in(fileName.toLocal8Bit().data(), this)) { // if file open failed
        std::cerr << fileName.toStdString() << " could not be opened.\n";
        return;
    }
    delete dxf;
    dxf = nullptr;


}

dxfReader::dxfReader( std::stringstream &fileName, QObject *parent)
{
    DL_Dxf *dxf = new DL_Dxf;
   // dxf->in(fileName,this);
    if (!dxf->in(fileName, this)) { // if file open failed
        std::cerr << fileName.str() << " could not be opened.\n";
        return;
    }
    delete dxf;
    dxf = nullptr;
}
void dxfReader::addLine(const DL_LineData &data)
{
    dxfLines << data;
}

//void dxfReader::addText(const DL_TextData &data)
//{
//    dxfText << data;
//    //    qDebug() << QString("%1").arg(data.text.data());
//}

void dxfReader::addCircle(const DL_CircleData &data)
{
    dxfCircle<<data;
}

//void dxfReader::addArc(const DL_ArcData &data)
//{
//    dxfArc<<data;
//}
