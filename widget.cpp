#include "widget.h"
#include "ui_widget.h"
#include "dxfreader.h"

#include <QRectF>
#include <QTableWidgetItem>
#include <QString>
#include <QFile>
#include <QMap>
#include <QtMath>
#include <QTextDocument>
#include <QFuture>
#include <QtConcurrent>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::Widget)
    ,fileDialog(new QFileDialog )
    ,outPutDialog(new QFileDialog)
    ,scene(new QGraphicsScene)
    ,View(new InteractiveView)
    ,TW_LeftParam(new QTabWidget)
   // ,tw_MarkParam(new QTableWidget(this))
    ,hLayoutXCenter(new QHBoxLayout)
    ,hLayoutYCenter(new QHBoxLayout)
    ,hLayoutXOffset(new QHBoxLayout)
    ,hLayoutYOffset(new QHBoxLayout)
    ,hLayoutZoom(new QHBoxLayout)
    ,vLayoutParam(new QVBoxLayout )
    ,hLayoutMain(new QHBoxLayout)
    ,gLayoutZoffset(new QGridLayout)
    ,ConfigQFrame(new Frame_Config)
    ,LoadDialog(new LoadingDialog)
    //, MarkItem(new QGraphicsTextItem)
    //,m_ProgressD(new QProgressDialog)
    ,thread(new asyncThread)

{
    ui->setupUi(this);
  //  this->setObjectName("数模解析软件");
    this->setWindowTitle("数模解析软件");
    fileDialog->setWindowTitle("请选择文件");
    fileDialog->setDirectory("./");
    fileDialog->setNameFilter(tr("File(*.dxf* *.DXF*)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);

//    outPutDialog->setWindowTitle("请选择文件夹");
//    outPutDialog->setDirectory("./");
    outPutDialog->setNameFilter(tr("File(*.csv* *.CSV*)"));
    outPutDialog->setFileMode(QFileDialog::ExistingFiles);
    outPutDialog->setViewMode(QFileDialog::Detail);
  //  ConfigQFrame->setObjectName("配置");
    ConfigQFrame->setWindowTitle("配置");
    ConfigQFrame->setGeometry(this->geometry().center().x()/2,this->geometry().center().y()/2,300,300);
    ui->tw_AnalyzedData->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_AnalyzedData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tw_AnalyzedData->horizontalHeader()->resizeSections(QHeaderView::Stretch);

    ui->tw_MarkPointData->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_MarkPointData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tw_MarkPointData->horizontalHeader()->resizeSections(QHeaderView::Stretch);

    TW_LeftParam->insertTab(0,ui->tw_AnalyzedData,tr("极柱"));
    TW_LeftParam->insertTab(1,ui->tw_MarkPointData,tr("MARK点"));
  //  ui->lab_x1  ->setReadOnly(1);
  //  ui->lineEdit_2->setReadOnly(1);
  //  ui->lineEdit_3->setReadOnly(1);
   // ui->lineEdit_4->setReadOnly(1);
    scene->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    View->setInteractive(true);
  //  View->setDragMode(QGraphicsView::ScrollHandDrag);
    View->setScene(scene);

    View->setRenderHints(QPainter::SmoothPixmapTransform);
    hLayoutYOffset->addWidget(ui->PB_Load);
    hLayoutYOffset->addWidget(ui->pb_output);
    hLayoutYOffset->addWidget(ui->pb_Config);
    vLayoutParam->addLayout(hLayoutYOffset);
    vLayoutParam->addWidget(TW_LeftParam);
    gLayoutZoffset->setColumnMinimumWidth(1,1);
    gLayoutZoffset->setRowMinimumHeight(2,2);

    gLayoutZoffset->addWidget(ui->label);
    gLayoutZoffset->addWidget(ui->lab_x1);
    gLayoutZoffset->addWidget(ui->label_2);
    gLayoutZoffset->addWidget(ui->lab_y1);
    gLayoutZoffset->addWidget(ui->label_3);
    gLayoutZoffset->addWidget(ui->lab_x2);
    gLayoutZoffset->addWidget(ui->label_4);
    gLayoutZoffset->addWidget(ui->lab_y2);
    vLayoutParam->addLayout(gLayoutZoffset);
    hLayoutZoom->addWidget(View);
    hLayoutMain->addLayout(vLayoutParam);
    hLayoutMain->addLayout(hLayoutZoom);
    this->setLayout(hLayoutMain);
    tbRowCount=0;
   // DistanceOfPost=14.4;
    ui->tw_MarkPointData->clear();

    View->setTransform(QTransform().scale(1, -1));
   // m_ProgressD->reset();
   // m_ProgressD->setModal(1);
    connect(thread, SIGNAL(SendData(QList<DL_CircleData>, QList<DL_LineData> )),this,SLOT(DrawItem(QList<DL_CircleData> , QList<DL_LineData> )));
    connect(this ,SIGNAL( SendTable()),this, SLOT(ShowTable()));
    connect(this,SIGNAL(SendMark()),this,SLOT(ShowMark()));
    connect(this,SIGNAL(CloseLoadDialog()),this,SLOT(HideLoadDialog()));
    connect(ui->pb_Config,SIGNAL(clicked()),ConfigQFrame,SLOT(show()));

}

Widget::~Widget()
{
    delete ui;
    delete fileDialog;
    delete outPutDialog;
    delete scene;
    delete View;
    //delete MarkPosText;
    //delete CrosslineItem1;
    //delete CrosslineItem2;
}



void Widget::on_PB_Load_clicked()
{
  //  ProgDialog->setVisible(0);
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
    LoadDialogCount=0;
//    scene->clear();
    CircleData.clear();
    BatteryPostData.clear();
    BatteryList.clear();
    SortedList.clear();
    MarkList.clear();
     ui->tw_MarkPointData->setRowCount(0);
    CrosslineItem1=new QGraphicsLineItem();
    CrosslineItem2=new QGraphicsLineItem();
    MarkCrossLineItem1=new QGraphicsLineItem();
    MarkCrossLineItem2=new QGraphicsLineItem();
    MarkPointItem=new QGraphicsTextItem();
    MarkItem=new QGraphicsTextItem();
    MarkPosText=new QTextDocument();
    MarkPointPosText=new  QTextDocument();
    sortDistance=ConfigQFrame->m_findAngle;
    DistanceOfPost=ConfigQFrame->m_PostDistance.toDouble();
    QFuture<bool> analyFuture=QtConcurrent::run(&Widget::AnalyzeFile,this,FileNames.join("/"));
    LoadDialog->show();

/*
  //  analyFuture.waitForFinished();
    //return ;
//    dxfReader dxfreader(FileNames.join("/"));

////    dx_data fData;
////    dx_iface *input = new dx_iface();
////   bool  badState = input->fileImport( FileNames.join("/").toStdString(), &fData );
//  //  QPen pen;
//    CrosslineFlag=true;
//   // pen.setColor(Qt::black);
//  //  pen.setWidth(0);
//    QFile f(FileNames.join(""));
//    f.open(QIODevice::ReadOnly |QIODevice::Text);

//    scale=0;
//    while(!f.atEnd())
//    {
//        if(f.readLine()=="$DIMLFAC\n")
//        {
//            if(f.readLine()==" 40\n")
//            {
//                scale= f.readLine().toDouble();
//                break;
//            }
//        }
//    }
//    if(f.isOpen())f.close();
//    qDebug()<<"scale is:"<<scale;
//    thread->m_Llist.clear();
//    thread->m_clist.clear();
//    for(auto d: dxfreader.dxfLines) {
//       // QLineF line(d.x1 , d.y1, d.x2,  d.y2);
//        //LinesData << d;
//      //  QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
//        //lineItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
//        thread->m_Llist<<d;
////        pen.setCosmetic(true);
////        lineItem->setPen(pen);
//      //     qDebug()<<QTime::currentTime();
//        //scene->addItem(lineItem);

//    }

//    // pen.setColor(Qt::blue);
//    for(auto d:dxfreader.dxfCircle)
//    {
//      //  QRectF Rect(d.cx-d.radius,d.cy-d.radius,d.radius*2,d.radius*2);
//        thread->m_clist<<d;
//      //  QGraphicsEllipseItem *CircleItem=new QGraphicsEllipseItem(Rect);
//        //CircleItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
//       // pen.setCosmetic(true);
//      //  CircleItem->setPen(pen);
//     //   qDebug()<<QTime::currentTime();
//        //scene->addItem(CircleItem);

//    }
//    thread->start();
//    tbRowCount=0;

////    BasePointx=0;
////    BasePointy=0;
//    bool repeated;
//  //  analyFuture.waitForFinished();
//    for(auto db : thread->m_clist)
//    {
//        repeated=false;
//        //  QList<DL_CircleData> filter;
//        for (auto circle :  CircleData)
//        {
//            if((QString::number(circle.cx,'f',2)==QString::number(db.cx,'f',2))&&(QString::number(circle.cy,'f',2)==QString::number(db.cy,'f',2)))
//            {
//                repeated=true;
//                break;
//            }
//        }
//        if(repeated)
//        {
//            continue;
//        }
////        if(BasePointx==0||BasePointx>db.cx)
////        {
////            BasePointx=db.cx;
////        }

////        if(BasePointy==0||BasePointy>db.cy)
////        {
////            BasePointy=db.cy;
////        }

//        CircleData<<db;

//        //        tbRowCount= ui->tw_AnalyzedData->rowCount();
//        //        ui->tw_AnalyzedData->insertRow(tbRowCount);
//        //        ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(db.cx)));
//        //        ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(db.cy)));
//        //        ui->tw_AnalyzedData->setItem(tbRowCount,2,new QTableWidgetItem(QString::number(db.radius)));

//    }
//   // qDebug()<<dxfreader.dxfCircle.count();
//   // qDebug()<<CircleData.count();
//    bool jump=0;
//    QList<int> Connected;
//        for (int i =0 ; i <CircleData.count() ;i++)
//        {
//         jump=0;
//         for (auto k :Connected)
//         {
//            if(k==i)jump=true;
//         }


//             if(jump)continue;
//             for(int j=0;j<CircleData.count();j++)
//             {
//             if(i==j)continue;
//             if(QString::number(CircleData[i].cx,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
//                 QString::number(CircleData[i].cy+ DistanceOfPost,'f',2)==QString::number(CircleData[j].cy,'f',2))
//             {
//                BatteryMark bm;
//                bm.Top.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
//                bm.Top.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
//                bm.Top.radius=QString::number(CircleData[i].radius,'f',3).toDouble();
//                bm.Bottom.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
//                bm.Bottom.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
//                bm.Bottom.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
//                bm.Angel=0;
//                bm.TopNumb=i;
//                bm.BottomNumb=j;
//                BatteryList.push_back(bm);
//                Connected<<j;
//                continue;
//             }
//             if(QString::number(CircleData[i].cx,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
//                 QString::number(CircleData[i].cy- DistanceOfPost,'f',2)==QString::number(CircleData[j].cy,'f',2))
//             {
//                BatteryMark bm;
//                bm.Top.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
//                bm.Top.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
//                bm.Top.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
//                bm.Bottom.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
//                bm.Bottom.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
//                bm.Bottom.radius=QString::number(CircleData[i].radius,'f',3).toDouble();
//                bm.Angel=0;
//                bm.TopNumb=j;
//                bm.BottomNumb=i;
//                BatteryList.push_back(bm);
//                Connected<<j;
//                continue;
//             }
//            if(QString::number(CircleData[i].cx+ DistanceOfPost,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
//                QString::number(CircleData[i].cy,'f',2)==QString::number(CircleData[j].cy,'f',2))
//             {
//                BatteryMark bm;
//                bm.Top.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
//                bm.Top.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
//                bm.Top.radius=QString::number(CircleData[i].radius,'f',3).toDouble();
//                bm.Bottom.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
//                bm.Bottom.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
//                bm.Bottom.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
//                bm.Angel=90;
//                bm.TopNumb=i;
//                bm.BottomNumb=j;
//                BatteryList.push_back(bm);
//                Connected<<j;
//                continue;
//             }
//            if(QString::number(CircleData[i].cx- DistanceOfPost,'f',2)==QString::number(CircleData[j].cx,'f',2)&&
//                QString::number(CircleData[i].cy,'f',2)==QString::number(CircleData[j].cy,'f',2))
//             {
//                BatteryMark bm;
//                bm.Top.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
//                bm.Top.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
//                bm.Top.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
//                bm.Bottom.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
//                bm.Bottom.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
//                bm.Bottom.radius=QString::number(CircleData[j].radius).toDouble();
//                bm.Angel=90;
//                bm.TopNumb=j;
//                bm.BottomNumb=i;
//                BatteryList.push_back(bm);
//                Connected<<j;
//                 continue;
//             }


//             }
//        }
//        QMap<double,QList<BatteryMark>> row;
//           QMap<int,QList<BatteryMark>> srow;
//      //  bool filter=false;
//      //  QList<double> index;
//        if(sortDistance)
//        {
//             for (int i =0 ;i<BatteryList.count();i++)
//             {
//             //             filter=false;
//             //                 for(auto r:row.keys())//todelete
//             //                 {

//             //                 if(r==BatteryList[i].Top.cx)//todelete
//             //                 {
//             //                     filter=true;
//             //                     break;
//             //                 }

//             //

//             row[BatteryList[i].Top.cx].append(BatteryList[i]);


//             //                 if(!filter)
//             //                 {

//             //                 index<<BatteryList[i].Top.cx;//todelete
//             //                 }
//             }

//             int index=0;
//             for(auto d:row)
//             {
//             //             QList<BatteryMark> slist;
//             //             for(auto dd: d)
//             //             {
//             QList<BatteryMark> sortedRow;
//             for(auto dd:d)
//             {
//                 sortedRow.append(dd);

//             }
//             if(index%2==0)
//             {
//             for (int i = 0; i < sortedRow.count()-1; i++)
//             {
//                 for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
//                 {
//                     if(sortedRow[j].Top.cy>sortedRow[j+1].Top.cy)
//                     {
//                         std:: swap(sortedRow[j],sortedRow[j+1]);
//                     }
//                 }
//             }
//             }
//             else
//             {
//             for (int i = 0; i < sortedRow.count()-1; i++)
//             {
//                 for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
//                 {
//                     if(sortedRow[j].Top.cy<sortedRow[j+1].Top.cy)
//                     {
//                         std:: swap(sortedRow[j],sortedRow[j+1]);
//                     }
//                 }
//             }
//             }
//             //             }
//             srow[index++].append(sortedRow);

//        }

//      //  qDebug()<<"before"<<index;//todelete
//       // std::sort(index.begin(),index.end());//todelete


//    //    qDebug()<<"after"<<index;



//       }
//        else
//        {
//        for (int i =0 ;i<BatteryList.count();i++)
//        {
//        row[BatteryList[i].Top.cy].append(BatteryList[i]);
//        }
//        int index=0;
//        for(auto d:row)
//        {
//        //             QList<BatteryMark> slist;
//        //             for(auto dd: d)
//        //             {
//        QList<BatteryMark> sortedRow;
//        for(auto dd:d)
//        {
//                 sortedRow.append(dd);

//        }
//        if(index%2==0)
//        {
//        for (int i = 0; i < sortedRow.count()-1; i++)
//        {
//                 for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
//                 {

//                     if(sortedRow[j].Top.cx>sortedRow[j+1].Top.cx)
//                     {
//                         std:: swap(sortedRow[j],sortedRow[j+1]);
//                     }
//                 }
//        }
//        }
//        else
//        {
//        for (int i = 0; i < sortedRow.count()-1; i++)
//        {
//                 for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
//                 {

//                     if(sortedRow[j].Top.cx<sortedRow[j+1].Top.cx)
//                     {
//                         std:: swap(sortedRow[j],sortedRow[j+1]);
//                     }
//                 }
//        }
//        }
//        //             }
//        srow[index++].append(sortedRow);
//       }

//       }

//        for(auto d:srow)
//        {
//          for (int i = 0; i < d.count(); i++)
//          {
//             DL_CircleData top(d[i].Top.cx,d[i].Top.cy,0,d[i].Top.radius);

//             BatteryPostData<<top;
//             DL_CircleData Bottom(d[i].Bottom.cx,d[i].Bottom.cy,0,d[i].Bottom.radius);
//             BatteryPostData<<Bottom;
//          }
//        }


//        for (auto circle: BatteryPostData)
//    {
//        tbRowCount= ui->tw_AnalyzedData->rowCount();
//        ui->tw_AnalyzedData->insertRow(tbRowCount);
//        ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(circle.cx)));
//        ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(circle.cy)));
//        ui->tw_AnalyzedData->setItem(tbRowCount,2,new QTableWidgetItem(QString::number(circle.radius)));
//    }

//    auto d=thread->m_clist.at(0);

//    View->centerOn(d.cx,d.cy);

   // View->viewport()->update();
*/

}

void Widget::on_tw_AnalyzedData_itemDoubleClicked(QTableWidgetItem *item)
{

        qreal x/*= ui->tw_AnalyzedData->item(item->row(),0)->text().toDouble()*/;
        qreal y/*= ui->tw_AnalyzedData->item(item->row(),1)->text().toDouble()*/;
        qreal r/*== ui->tw_AnalyzedData->item(item->row(),2)->text().toDouble()*/;
        qreal s;
        x= SortedList[ ui->tw_AnalyzedData->currentIndex().row()].GetCenterPoint().cx;
        y=SortedList[ui->tw_AnalyzedData->currentIndex().row()].GetCenterPoint().cy;
        r=SortedList[ui->tw_AnalyzedData->currentIndex().row()].GetCenterPoint().radius;
        s=SortedList[ui->tw_AnalyzedData->currentIndex().row()].Scale;
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


        MarkPosText->setPlainText(QString::asprintf("x:%9.3f\r\ny:%9.3f",x*s,y*s));
        MarkItem->setDocument(MarkPosText);
        MarkItem->setFont(QFont("Microsoft YaHei",20,30,1));
        ui->lab_x1->setText(QString::number(SortedList[ui->tw_AnalyzedData->currentIndex().row()].getTopScale().cx,'f',3));
        ui->lab_y1->setText(QString::number(SortedList[ui->tw_AnalyzedData->currentIndex().row()].getTopScale().cy,'f',3));
        ui->lab_x2->setText(QString::number(SortedList[ui->tw_AnalyzedData->currentIndex().row()].getBottomScale().cx,'f',3));
        ui->lab_y2->setText(QString::number(SortedList[ui->tw_AnalyzedData->currentIndex().row()].getBottomScale().cy,'f',3));
        MarkItem->setPos(x,y);
        pen.setCosmetic(true);
        if(CrosslineFlag)
        {
        CrosslineFlag=false;
        CrosslineItem1->setPen(pen);
        MarkItem->setDefaultTextColor(Qt::red);
       // MarkItem->setTextWidth(20);
         MarkItem->setTransform(QTransform().scale(1,-1));
        CrosslineItem2->setPen(pen);
        scene->addItem(CrosslineItem1);
        scene->addItem(CrosslineItem2);
        scene->addItem(MarkItem);
        }


}

void Widget::DrawItem(QList<DL_CircleData> clist, QList<DL_LineData> llist)
{
        scene->clear();
//        CircleData.clear();
//        BatteryPostData.clear();
//        BatteryList.clear();
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(0);
          for(auto d: llist) {
         QLineF line(d.x1 , d.y1, d.x2,  d.y2);
         QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
       //  lineItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
         pen.setCosmetic(true);
         lineItem->setPen(pen);
         scene->addItem(lineItem);
        // View->update();
        }
          pen.setColor(Qt::blue);
        for(auto d:clist)
          {
         QRectF Rect(d.cx-d.radius,d.cy-d.radius,d.radius*2,d.radius*2);

          QGraphicsEllipseItem *CircleItem=new QGraphicsEllipseItem(Rect);
     //    CircleItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
          pen.setCosmetic(true);
          CircleItem->setPen(pen);
         //   qDebug()<<QTime::currentTime();
         scene->addItem(CircleItem);
       //  View->update();
          }
        QLineF line1(0,+100,0,-100);
        QLineF line2(100,0,-100,0);
        QString mark;
        mark="x=0\ny=0";
        QGraphicsTextItem *tt=new QGraphicsTextItem(mark);
        tt->setTransform(QTransform().scale(1,-1));
        QGraphicsLineItem *lineItem1 = new QGraphicsLineItem(line1);
        QGraphicsLineItem *lineItem2 = new QGraphicsLineItem(line2);
      //  tt->setTextWidth(30.0);
        tt->setPos(0,0);
        pen.setColor(Qt::green);
        lineItem1->setPen(pen);
        lineItem2->setPen(pen);
        tt->setDefaultTextColor(Qt::black);
        tt->setFont(QFont("Microsoft YaHei",20,30,1));
       //  qDebug()<<"isFree?2";
        scene->addItem(tt);
        scene->addItem(lineItem1);
        scene->addItem(lineItem2);
     //   qDebug()<<"isFree?1";
        View->update();
        emit CloseLoadDialog();
}

void Widget::ShowTable()
{
        ui->tw_AnalyzedData->clear();
        ui->tw_AnalyzedData->setColumnCount(3);
        ui->tw_AnalyzedData->setRowCount(0);


         tbRowCount=0;
        for (auto circle: SortedList)
        {
         tbRowCount= ui->tw_AnalyzedData->rowCount();
         qreal s=circle.Scale;
         ui->tw_AnalyzedData->insertRow(tbRowCount);
         ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(circle.GetCenterPoint().cx*s)));
         ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(circle.GetCenterPoint().cy*s)));
         QTableWidgetItem *checkBox = new QTableWidgetItem();
         checkBox->setCheckState(Qt::Checked);
         ui->tw_AnalyzedData->setItem(tbRowCount,2,checkBox);

         //ui->tw_AnalyzedData->setItem(tbRowCount,3 new QTableWidgetItem)
        }
        emit CloseLoadDialog();
}

void Widget::ShowMark()
{
        ui->tw_MarkPointData->clear();
        ui->tw_MarkPointData->setColumnCount(3);
        ui->tw_MarkPointData->setRowCount(0);
        RowCount=0;
        for(auto circle:MarkList)
        {
           RowCount=ui->tw_MarkPointData->rowCount() ;

         //  qDebug()<<RowCount;
           ui->tw_MarkPointData->insertRow(RowCount);
          // ui->tw_MarkPointData->removeRow(RowCount - 1);
           ui->tw_MarkPointData->setItem(RowCount,0,new QTableWidgetItem(QString::number(circle.getScaleMarkPoint().cx)));
           ui->tw_MarkPointData->setItem(RowCount,1 ,new QTableWidgetItem(QString::number(circle.getScaleMarkPoint().cy)));
           QTableWidgetItem *checkBox = new QTableWidgetItem();
           checkBox->setCheckState(Qt::Unchecked);
           ui->tw_MarkPointData->setItem(RowCount,2,checkBox);
        //    thread->msleep(100);
        //   ui->tw_MarkPointData->setItem(RowCount,2,new QTableWidgetItem(QString::number(circle.getScaleMarkPoint().radius)));
        }
       // ui->tw_MarkPointData->setRowCount(ui->tw_MarkPointData->rowCount());
    //    qDebug()<<"columnCount:"<<ui->tw_MarkPointData->columnCount()<<"rowCount:"<<ui->tw_MarkPointData->rowCount();
   //     qDebug()<<RowCount;
        emit CloseLoadDialog();
}

void Widget::HideLoadDialog()
{
        LoadDialogCount++;
        if(LoadDialogCount>=3)
        {
         LoadDialogCount=0;
         LoadDialog->hide();
        }

}

bool Widget::AnalyzeFile(QString FileNames)
{


      //  QFuture<bool> ClearFuture=QtConcurrent::run(&Widget::InitBuffer,this);
        dxfReader dxfreader(FileNames);

        //    dx_data fData;
        //    dx_iface *input = new dx_iface();
        //   bool  badState = input->fileImport( FileNames.join("/").toStdString(), &fData );
        //  QPen pen;
        QFuture<bool> Filterfuture=   QtConcurrent::run(&Widget::CircleFilter,this,dxfreader.dxfCircle);

        CrosslineFlag=true;
        MarkCrosslineFlag=true;
        // pen.setColor(Qt::black);
        //  pen.setWidth(0);
        QFile f(FileNames);
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
        if(f.isOpen())f.close();
         QFuture<bool> FilterMarkPoint=QtConcurrent::run(&Widget::MarkFilter,this,dxfreader.dxfCircle,scale);
      //  qDebug()<<"scale is:"<<scale;
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
        //ClearFuture.waitForFinished();
       // qDebug()<<"OutOfFture";
        thread->start();

        Filterfuture.waitForFinished();

//        bool repeated;
//        //  analyFuture.waitForFinished();
//        for(auto db : dxfreader.dxfCircle)
//        {
//         repeated=false;
//         //  QList<DL_CircleData> filter;
//         for (auto circle :  CircleData)
//         {
//             if((QString::number(circle.cx,'f',2)==QString::number(db.cx,'f',2))&&(QString::number(circle.cy,'f',2)==QString::number(db.cy,'f',2)))
//             {
//        repeated=true;
//        break;
//             }
//         }
//         if(repeated)
//         {
//             continue;
//         }


//         CircleData<<db;

//         //        tbRowCount= ui->tw_AnalyzedData->rowCount();
//         //        ui->tw_AnalyzedData->insertRow(tbRowCount);
//         //        ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(db.cx)));
//         //        ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(db.cy)));
//         //        ui->tw_AnalyzedData->setItem(tbRowCount,2,new QTableWidgetItem(QString::number(db.radius)));

//        }
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
        bm.Top.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
        bm.Top.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
        bm.Top.radius=QString::number(CircleData[i].radius,'f',3).toDouble();
        bm.Bottom.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
        bm.Bottom.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
        bm.Bottom.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
        bm.Scale=scale;
        bm.Angle=0;
        bm.TopNumb=i;
        bm.BottomNumb=j;

        BatteryList.push_back(bm);
        Connected<<j;
        continue;
             }
             if(QString::number(CircleData[i].cx,'f',3)==QString::number(CircleData[j].cx,'f',3)&&
                 QString::number(CircleData[i].cy- DistanceOfPost,'f',3)==QString::number(CircleData[j].cy,'f',3))
             {
        BatteryMark bm;
        bm.Top.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
        bm.Top.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
        bm.Top.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
        bm.Bottom.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
        bm.Bottom.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
        bm.Bottom.radius=QString::number(CircleData[i].radius,'f',3).toDouble();
         bm.Scale=scale;
        bm.Angle=0;
        bm.TopNumb=j;
        bm.BottomNumb=i;
        BatteryList.push_back(bm);
        Connected<<j;
        continue;
             }
             if(QString::number(CircleData[i].cx+ DistanceOfPost,'f',3)==QString::number(CircleData[j].cx,'f',3)&&
                 QString::number(CircleData[i].cy,'f',3)==QString::number(CircleData[j].cy,'f',3))
             {
        BatteryMark bm;
        bm.Top.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
        bm.Top.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
        bm.Top.radius=QString::number(CircleData[i].radius,'f',3).toDouble();
        bm.Bottom.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
        bm.Bottom.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
        bm.Bottom.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
        bm.Scale=scale;
        bm.Angle=90;
        bm.TopNumb=i;
        bm.BottomNumb=j;
        BatteryList.push_back(bm);
        Connected<<j;
        continue;
             }
             if(QString::number(CircleData[i].cx- DistanceOfPost,'f',3)==QString::number(CircleData[j].cx,'f',3)&&
                 QString::number(CircleData[i].cy,'f',3)==QString::number(CircleData[j].cy,'f',3))
             {
        BatteryMark bm;
        bm.Top.cx=QString::number(CircleData[j].cx,'f',3).toDouble();
        bm.Top.cy=QString::number(CircleData[j].cy,'f',3).toDouble();
        bm.Top.radius=QString::number(CircleData[j].radius,'f',3).toDouble();
        bm.Bottom.cx=QString::number(CircleData[i].cx,'f',3).toDouble();
        bm.Bottom.cy=QString::number(CircleData[i].cy,'f',3).toDouble();
        bm.Bottom.radius=QString::number(CircleData[j].radius).toDouble();
        bm.Scale=scale;
        bm.Angle=90;
        bm.TopNumb=j;
        bm.BottomNumb=i;
        BatteryList.push_back(bm);
        Connected<<j;
        continue;
             }


         }
        }
        QMap<double,QList<BatteryMark>> row;
        QMap<int,QList<BatteryMark>> srow;
        //  bool filter=false;
        //  QList<double> index;
        if(sortDistance)
        {
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
             if(index%2==0)
             {
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
             }
             else
             {
        for (int i = 0; i < sortedRow.count()-1; i++)
        {
                 for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
                 {
                     if(sortedRow[j].Top.cy<sortedRow[j+1].Top.cy)
                     {
                         std:: swap(sortedRow[j],sortedRow[j+1]);
                     }
                 }
        }
             }
             //             }
             srow[index++].append(sortedRow);

         }

         //  qDebug()<<"before"<<index;//todelete
         // std::sort(index.begin(),index.end());//todelete


         //    qDebug()<<"after"<<index;



        }
        else
        {
         for (int i =0 ;i<BatteryList.count();i++)
         {
             row[BatteryList[i].Top.cy].append(BatteryList[i]);
         }
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
             if(index%2==0)
             {
        for (int i = 0; i < sortedRow.count()-1; i++)
        {
                 for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
                 {

                     if(sortedRow[j].Top.cx>sortedRow[j+1].Top.cx)
                     {
                         std:: swap(sortedRow[j],sortedRow[j+1]);
                     }
                 }
        }
             }
             else
             {
        for (int i = 0; i < sortedRow.count()-1; i++)
        {
                 for (int j =0 ;j<sortedRow.count()-i-1 ;j++)
                 {

                     if(sortedRow[j].Top.cx<sortedRow[j+1].Top.cx)
                     {
                         std:: swap(sortedRow[j],sortedRow[j+1]);
                     }
                 }
        }
             }
             //             }
             srow[index++].append(sortedRow);
         }

        }

        for(auto d:srow)
        {
         for (int i = 0; i < d.count(); i++)
         {

             //BatteryMark temp;

             SortedList<<d[i];
//             DL_CircleData Bottom(d[i].Bottom.cx,d[i].Bottom.cy,0,d[i].Bottom.radius);
//             BatteryPostData<<Bottom;
         }
        }


        emit SendTable();
        FilterMarkPoint.waitForFinished();
        emit SendMark();
      //  QtConcurrent::run(&Widget::ShowMark,this);
        auto d=thread->m_clist.at(0);

        View->centerOn(d.cx,d.cy);
        return 1;
}

bool Widget::CircleFilter( QList<DL_CircleData> dxfCircle)
{
        bool repeated;
        //  analyFuture.waitForFinished();
        for(auto db : dxfCircle)
        {
         repeated=false;
         //  QList<DL_CircleData> filter;
         for (auto circle :  CircleData)
         {
             if((QString::number(circle.cx,'f',3)==QString::number(db.cx,'f',3))&&(QString::number(circle.cy,'f',3)==QString::number(db.cy,'f',3)))
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
        return 1;
}

bool Widget::MarkFilter(QList<DL_CircleData>dxfCircle, qreal scale)
{
        for(auto circle:dxfCircle)
        {
         if(QString::number(circle.radius,'f',2).toDouble()*scale==ConfigQFrame->m_markRadius.toDouble())
         {
             MarkPointType markpoint;
             markpoint.MarkPoint.cx=circle.cx;
             markpoint.MarkPoint.cy=circle.cy;
             markpoint.MarkPoint.radius=circle.radius;
             markpoint.scale=scale;
             MarkList<<markpoint;
         }
         // MarkList
        }
       // qDebug()<<MarkList.count();
        return 1;
}

bool Widget::InitBuffer()
{
        qDebug()<<"into"<<__FUNCTION__<<"\r\n";


          scene->clear();

      //  scene->
      //  qDebug()<<'1';
        CircleData.clear();
      //  qDebug()<<'2';
        BatteryPostData.clear();
        BatteryList.clear();
        return 1;
}

//void Widget::ShowProgressDialog()
//{
//        emit ShowSig();
//}

void Widget::on_pb_output_clicked()
{
        QString Directory= outPutDialog->getExistingDirectory(this,"选择目录","./",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
        if(Directory=="")
        {
         return ;
        }
       // qDebug()<<Directory;
        QFile outFile(Directory+"/1.csv");

        if(outFile.open(QIODevice::WriteOnly))
        {
             outFile.write(QString("ID,X,Y,Angle,x1,y1,x2,y2\r\n").toStdString().c_str());
         for(int i =0 ; i<MarkList.count();i++)
         {
             if(ui->tw_MarkPointData->item(i,2)->checkState()==Qt::Checked)
             {
             QString str;
        str="MarkPoint,"+
             QString::number(MarkList[i].getScaleMarkPoint().cx)+","+
             QString::number(MarkList[i].getScaleMarkPoint().cy)+"\r\n";
             outFile.write(str.toStdString().c_str());
             }

         }
        // outFile.write("\n");
         for(int i =0 ; i<SortedList.count();i++)
         {
             if(ui->tw_AnalyzedData->item(i,2)->checkState()==Qt::Checked)
             {
             QString str;
             str=QString::number(i+1)+","+
                   QString::number(SortedList[i].GetCenterPoint().cx)+","+
                   QString::number(SortedList[i].GetCenterPoint().cy)+","+
                   QString::number(SortedList[i].Angle)+","+
                   QString::number(SortedList[i].getTopScale().cx)+","+
                   QString::number(SortedList[i].getTopScale().cy)+","+
                   QString::number(SortedList[i].getBottomScale().cx)+","+
                   QString::number(SortedList[i].getBottomScale().cy)+"\r\n";
             outFile.write(str.toStdString().c_str());
             }


         }

        }
        outFile.close();
        qDebug()<<"outputDone";
}


void Widget::on_tw_MarkPointData_itemDoubleClicked(QTableWidgetItem *item)
{
        qreal x/*= ui->tw_AnalyzedData->item(item->row(),0)->text().toDouble()*/;
        qreal y/*= ui->tw_AnalyzedData->item(item->row(),1)->text().toDouble()*/;
        qreal r/*== ui->tw_AnalyzedData->item(item->row(),2)->text().toDouble()*/;
        qreal s;
        x= MarkList[ ui->tw_MarkPointData->currentIndex().row()].MarkPoint.cx;
        y=MarkList[ui->tw_MarkPointData->currentIndex().row()].MarkPoint.cy;
        r=MarkList[ui->tw_MarkPointData->currentIndex().row()].MarkPoint.radius;
        s=MarkList[ui->tw_MarkPointData->currentIndex().row()].scale;
        QPen pen;
        pen.setColor(QColorConstants::Svg::purple);
        pen.setWidth(3);
         qDebug()<<r;
        r=r*10;
        QLineF line1(x,y+r,x,y-r);
        QLineF line2(x+r,y,x-r,y);

        // QGraphicsLineItem *lineItem1 = new QGraphicsLineItem(line1);
        //  QGraphicsLineItem *lineItem2 = new QGraphicsLineItem(line2);
        MarkCrossLineItem1->setLine(line1);
        MarkCrossLineItem2->setLine(line2);


        MarkPointPosText->setPlainText(QString::asprintf("x:%9.3f\r\ny:%9.3f",x*s,y*s));
        MarkPointItem->setDocument(MarkPointPosText);
        MarkPointItem->setFont(QFont("Microsoft YaHei",20,30,1));
        MarkPointItem->setPos(x,y);
        pen.setCosmetic(true);
        if(MarkCrosslineFlag)
        {
         MarkCrosslineFlag=false;
         MarkCrossLineItem1->setPen(pen);
         MarkPointItem->setDefaultTextColor(QColorConstants::Svg::skyblue);
         // MarkItem->setTextWidth(20);
         MarkPointItem->setTransform(QTransform().scale(1,-1));
         MarkCrossLineItem2->setPen(pen);
         scene->addItem(MarkCrossLineItem1);
         scene->addItem(MarkCrossLineItem2);
         scene->addItem(MarkPointItem);
        }
}

