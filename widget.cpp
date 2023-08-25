
#include "widget.h"
#include "ui_widget.h"

#include <QRectF>
#include <QTableWidgetItem>
#include <QString>
#include <QFile>
#include <QMap>
#include <QtMath>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , fileDialog(new QFileDialog )
    ,scene(new QGraphicsScene)
    ,View(new InteractiveView)
    , hLayoutXCenter(new QHBoxLayout)
    , hLayoutYCenter(new QHBoxLayout)
    ,hLayoutXOffset(new QHBoxLayout)
    ,hLayoutYOffset(new QHBoxLayout)
    ,hLayoutZoom(new QHBoxLayout)
    ,vLayoutParam(new QVBoxLayout )
    ,hLayoutMain(new QHBoxLayout)
    ,thread(new asyncThread)
{
    ui->setupUi(this);
    fileDialog->setWindowTitle("请选择文件");
    fileDialog->setDirectory("./");
    fileDialog->setNameFilter(tr("File(*.dxf* *.DXF*)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    ui->tw_AnalyzedData->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_AnalyzedData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scene->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    View->setInteractive(true);
    View->setDragMode(QGraphicsView::ScrollHandDrag);
    View->setScene(scene);
    View->setRenderHints(QPainter::SmoothPixmapTransform);
    hLayoutYOffset->addWidget(ui->PB_Load);
    hLayoutYOffset->addWidget(ui->pb_output);
    hLayoutYOffset->addWidget(ui->pb_Config);
    vLayoutParam->addLayout(hLayoutYOffset);
    vLayoutParam->addWidget(ui->tw_AnalyzedData);
    hLayoutZoom->addWidget(View);
    hLayoutMain->addLayout(vLayoutParam);
    hLayoutMain->addLayout(hLayoutZoom);
    this->setLayout(hLayoutMain);
    tbRowCount=0;
    DistanceOfPost=57.6;
    View->setTransform(QTransform().scale(1, -1));
    connect(thread, SIGNAL(SendData(QList<DL_CircleData>, QList<DL_LineData> )),this,SLOT(DrawItem(QList<DL_CircleData> , QList<DL_LineData> )));
}

Widget::~Widget()
{
    delete ui;
    delete fileDialog;
    delete scene;
    delete View;
    //delete CrosslineItem1;
    //delete CrosslineItem2;
}



void Widget::on_PB_Load_clicked()
{

    QStringList FileNames;
    FileNames.clear();
    if(fileDialog->exec())
    {
        FileNames=fileDialog->selectedFiles();
       // if(FileNames.count()<=0)return;
    }
    else
    {
        return;
    }
    ui->tw_AnalyzedData->clear();
    ui->tw_AnalyzedData->setColumnCount(3);
    ui->tw_AnalyzedData->setRowCount(0);

    scene->clear();
    CircleData.clear();
    BatteryPostData.clear();
    BatteryList.clear();
    CrosslineItem1=new QGraphicsLineItem();
    CrosslineItem2=new QGraphicsLineItem();
    dxfReader dxfreader(FileNames.join("/"));
  //  QPen pen;
    CrosslineFlag=true;
   // pen.setColor(Qt::black);
  //  pen.setWidth(0);
    QFile f(FileNames.join(""));
    f.open(QIODevice::ReadOnly |QIODevice::Text);

    scale=0;
    while(!f.atEnd())
    {
        if(f.readLine()=="$DIMLFAC\n")
        {
            if(f.readLine()==" 40\n")
            {
                scale= f.readLine().toDouble();
                break;
            }
        }
    }
    qDebug()<<"scale is:"<<scale;
    thread->m_Llist.clear();
    thread->m_clist.clear();
    for(auto d: dxfreader.dxfLines) {
       // QLineF line(d.x1 , d.y1, d.x2,  d.y2);
        //LinesData << d;
      //  QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
        //lineItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        thread->m_Llist<<d;
//        pen.setCosmetic(true);
//        lineItem->setPen(pen);
      //     qDebug()<<QTime::currentTime();
        //scene->addItem(lineItem);

    }

    // pen.setColor(Qt::blue);
    for(auto d:dxfreader.dxfCircle)
    {
      //  QRectF Rect(d.cx-d.radius,d.cy-d.radius,d.radius*2,d.radius*2);
        thread->m_clist<<d;
      //  QGraphicsEllipseItem *CircleItem=new QGraphicsEllipseItem(Rect);
        //CircleItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
       // pen.setCosmetic(true);
      //  CircleItem->setPen(pen);
     //   qDebug()<<QTime::currentTime();
        //scene->addItem(CircleItem);

    }
    thread->start();
    tbRowCount=0;


    bool repeated;
    for(auto db : dxfreader.dxfCircle)
    {
        repeated=false;
        //  QList<DL_CircleData> filter;
        for (auto circle :  CircleData)
        {
            if((QString::number(circle.cx,'f',2)==QString::number(db.cx,'f',2))&&(QString::number(circle.cy,'f',2)==QString::number(db.cy,'f',2)))
            {
                repeated=true;
                break;
            }
        }
        if(repeated)
        {
            continue;
        }
        CircleData<<db;

        //        tbRowCount= ui->tw_AnalyzedData->rowCount();
        //        ui->tw_AnalyzedData->insertRow(tbRowCount);
        //        ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(db.cx)));
        //        ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(db.cy)));
        //        ui->tw_AnalyzedData->setItem(tbRowCount,2,new QTableWidgetItem(QString::number(db.radius)));

    }
   // qDebug()<<dxfreader.dxfCircle.count();
   // qDebug()<<CircleData.count();
    bool jump=0;
    QList<int> Connected;
        for (int i =0 ; i <CircleData.count() ;i++)
        {
         jump=0;
         for (auto k :Connected)
         {
            if(k==i)jump=true;
         }


             if(jump)continue;
             for(int j=0;j<CircleData.count();j++)
             {
             if(i==j)continue;
             if(QString::number(CircleData[i].cx,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
                 QString::number(CircleData[i].cy+ DistanceOfPost,'f',2)==QString::number(CircleData[j].cy,'f',2))
             {
                BatteryMark bm;
                bm.Top.cx=CircleData[i].cx;
                bm.Top.cy=CircleData[i].cy;
                bm.Top.radius=CircleData[i].radius;
                bm.Bottom.cx=CircleData[j].cx;
                bm.Bottom.cy=CircleData[j].cy;
                bm.Bottom.radius=CircleData[j].radius;
                bm.Angel=0;
                bm.TopNumb=i;
                bm.BottomNumb=j;
                BatteryList.push_back(bm);
                Connected<<j;
                continue;
             }
             if(QString::number(CircleData[i].cx,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
                 QString::number(CircleData[i].cy- DistanceOfPost,'f',2)==QString::number(CircleData[j].cy,'f',2))
             {
                BatteryMark bm;
                bm.Top.cx=CircleData[j].cx;
                bm.Top.cy=CircleData[j].cy;
                bm.Top.radius=CircleData[j].radius;
                bm.Bottom.cx=CircleData[i].cx;
                bm.Bottom.cy=CircleData[i].cy;
                bm.Bottom.radius=CircleData[i].radius;
                bm.Angel=0;
                bm.TopNumb=j;
                bm.BottomNumb=i;
                BatteryList.push_back(bm);
                Connected<<j;
                continue;
             }
            if(QString::number(CircleData[i].cx+ DistanceOfPost,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
                QString::number(CircleData[i].cy,'f',2)==QString::number(CircleData[j].cy,'f',2))
             {
                BatteryMark bm;
                bm.Top.cx=CircleData[i].cx;
                bm.Top.cy=CircleData[i].cy;
                bm.Top.radius=CircleData[i].radius;
                bm.Bottom.cx=CircleData[j].cx;
                bm.Bottom.cy=CircleData[j].cy;
                bm.Bottom.radius=CircleData[j].radius;
                bm.Angel=90;
                bm.TopNumb=i;
                bm.BottomNumb=j;
                BatteryList.push_back(bm);
                Connected<<j;
                continue;
             }
            if(QString::number(CircleData[i].cx- DistanceOfPost,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
                QString::number(CircleData[i].cy,'f',2)==QString::number(CircleData[j].cy,'f',2))
             {
                BatteryMark bm;
                bm.Top.cx=CircleData[j].cx;
                bm.Top.cy=CircleData[j].cy;
                bm.Top.radius=CircleData[j].radius;
                bm.Bottom.cx=CircleData[i].cx;
                bm.Bottom.cy=CircleData[i].cy;
                bm.Bottom.radius=CircleData[j].radius;
                bm.Angel=90;
                bm.TopNumb=j;
                bm.BottomNumb=i;
                BatteryList.push_back(bm);
                Connected<<j;
                 continue;
             }


             }
        }
        QMap<double,QList<BatteryMark>> row;
      //  bool filter=false;
      //  QList<double> index;

        for (int i =0 ;i<BatteryList.count();i++)
        {
//             filter=false;
//                 for(auto r:row.keys())//todelete
//                 {

//                 if(r==BatteryList[i].Top.cx)//todelete
//                 {
//                     filter=true;
//                     break;
//                 }

//

             row[BatteryList[i].Top.cx].append(BatteryList[i]);


//                 if(!filter)
//                 {

//                 index<<BatteryList[i].Top.cx;//todelete
//                 }
        }
      //  qDebug()<<"before"<<index;//todelete
       // std::sort(index.begin(),index.end());//todelete


    //    qDebug()<<"after"<<index;

      QMap<int,QList<BatteryMark>> srow;
        int index=0;
       for(auto d:row)
        {
//             QList<BatteryMark> slist;
//             for(auto dd: d)
//             {
             QList<BatteryMark> sortedRow;
             for(auto dd:d)
             {
             sortedRow.append(dd);

             }

             for (int i = 0; i < sortedRow.count()-1; i++)
             {
             for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
             {
                 if(sortedRow[j].Top.cy>sortedRow[j+1].Top.cy)
                 {
                     std:: swap(sortedRow[j],sortedRow[j+1]);
                 }
             }
             }
//             }
             srow[index++].append(sortedRow);

       }
        for(auto d:srow)
        {
          for (int i = 0; i < d.count(); i++)
          {
             DL_CircleData top(d[i].Top.cx,d[i].Top.cy,0,d[i].Top.radius);

             BatteryPostData<<top;
             DL_CircleData Bottom(d[i].Bottom.cx,d[i].Bottom.cy,0,d[i].Bottom.radius);
             BatteryPostData<<Bottom;
          }
        }


        for (auto circle: BatteryPostData)
    {
        tbRowCount= ui->tw_AnalyzedData->rowCount();
        ui->tw_AnalyzedData->insertRow(tbRowCount);
        ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(circle.cx)));
        ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(circle.cy)));
        ui->tw_AnalyzedData->setItem(tbRowCount,2,new QTableWidgetItem(QString::number(circle.radius)));
    }

    auto d=dxfreader.dxfCircle.at(0);

    View->centerOn(d.cx,d.cy);
   // View->viewport()->update();
}


//void Widget::on_tw_AnalyzedData_cellClicked(int row, int column)
//{
//    qreal x= ui->tw_AnalyzedData->item(row,0)->text().toDouble();
//    qreal y= ui->tw_AnalyzedData->item(row,1)->text().toDouble();
//    QPen pen;
//    pen.setColor(Qt::red);
//    pen.setWidth(0);

//    QLineF line1(x,y+10,x,y-10);
//    QLineF line2(x+10,y,x-10,y);
//   // QGraphicsLineItem *lineItem1 = new QGraphicsLineItem(line1);
//  //  QGraphicsLineItem *lineItem2 = new QGraphicsLineItem(line2);
//    CrosslineItem1->setLine(line1);
//    CrosslineItem2->setLine(line2);
//    pen.setCosmetic(true);
//    CrosslineItem1->setPen(pen);
//    scene->addItem(CrosslineItem1);
//    CrosslineItem2->setPen(pen);
//    scene->addItem(CrosslineItem2);

//}




void Widget::on_tw_AnalyzedData_itemDoubleClicked(QTableWidgetItem *item)
{

        qreal x= ui->tw_AnalyzedData->item(item->row(),0)->text().toDouble();
        qreal y= ui->tw_AnalyzedData->item(item->row(),1)->text().toDouble();
        qreal r= ui->tw_AnalyzedData->item(item->row(),2)->text().toDouble();
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        r=r*10;
        QLineF line1(x,y+r,x,y-r);
        QLineF line2(x+r,y,x-r,y);

       // QGraphicsLineItem *lineItem1 = new QGraphicsLineItem(line1);
      //  QGraphicsLineItem *lineItem2 = new QGraphicsLineItem(line2);
        CrosslineItem1->setLine(line1);
        CrosslineItem2->setLine(line2);

        pen.setCosmetic(true);
        if(CrosslineFlag)
        {
        CrosslineFlag=false;
        CrosslineItem1->setPen(pen);
        CrosslineItem2->setPen(pen);
        scene->addItem(CrosslineItem1);
        scene->addItem(CrosslineItem2);

        }


}

void Widget::DrawItem(QList<DL_CircleData> clist, QList<DL_LineData> llist)
{
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(0);
          for(auto d: llist) {
         QLineF line(d.x1 , d.y1, d.x2,  d.y2);
         QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
         pen.setCosmetic(true);
         lineItem->setPen(pen);
         scene->addItem(lineItem);
        }
          pen.setColor(Qt::blue);
        for(auto d:clist)
          {
         QRectF Rect(d.cx-d.radius,d.cy-d.radius,d.radius*2,d.radius*2);

          QGraphicsEllipseItem *CircleItem=new QGraphicsEllipseItem(Rect);
         //CircleItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
          pen.setCosmetic(true);
          CircleItem->setPen(pen);
         //   qDebug()<<QTime::currentTime();
         scene->addItem(CircleItem);
          }

}

