
#include "widget.h"
#include "ui_widget.h"

#include <QRectF>
#include <QTableWidgetItem>
#include <QString>

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
    ,CrosslineItem1(new QGraphicsLineItem)
    ,CrosslineItem2(new QGraphicsLineItem)
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
    vLayoutParam->addLayout(hLayoutYOffset);
    vLayoutParam->addWidget(ui->tw_AnalyzedData);
    hLayoutZoom->addWidget(View);
    hLayoutMain->addLayout(vLayoutParam);
    hLayoutMain->addLayout(hLayoutZoom);
    this->setLayout(hLayoutMain);
    tbRowCount=0;
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
    dxfReader dxfreader(FileNames.join("/"));
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(0);
    for(auto d: dxfreader.dxfLines) {
        QLineF line(d.x1 , d.y1, d.x2,  d.y2);
        LinesData << d;
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
        pen.setCosmetic(true);
        lineItem->setPen(pen);
        scene->addItem(lineItem);
    }
     pen.setColor(Qt::blue);
    for(auto d:dxfreader.dxfCircle)
    {
        QRectF Rect(d.cx-d.radius,d.cy-d.radius,d.radius*2,d.radius*2);

        QGraphicsEllipseItem *CircleItem=new QGraphicsEllipseItem(Rect);
        pen.setCosmetic(true);
        CircleItem->setPen(pen);
        scene->addItem(CircleItem);
    }

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
    qDebug()<<dxfreader.dxfCircle.count();
    qDebug()<<CircleData.count();

//    for (int i =0 ; i <CircleData.count() ;i++)
//    {
//         for(int j=0;j<CircleData.count();j++)
//         {
//            if(i==j)continue;
//            if( CircleData[i].cx+DistanceOfPost==CircleData[j].cx &&
//                CircleData[i].cy+DistanceOfPost==CircleData[j].cy &&
//                CircleData[i].cx-DistanceOfPost==CircleData[j].cx &&
//                CircleData[i].cy-DistanceOfPost==CircleData[j].cy )
//            {
//                PostType val;
//                val.cd(CircleData[i].cx,CircleData[i].cy,CircleData[i].cz,CircleData[i].radius);
//                val.Angel=90;
//                val.ConnectNum=1;
//                PostList.append(val);
//            }
//         }
//    }
    for (auto circle :  CircleData)
    {
        tbRowCount= ui->tw_AnalyzedData->rowCount();
        ui->tw_AnalyzedData->insertRow(tbRowCount);
        ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(circle.cx)));
        ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(circle.cy)));
        ui->tw_AnalyzedData->setItem(tbRowCount,2,new QTableWidgetItem(QString::number(circle.radius)));
    }

    auto d=dxfreader.dxfCircle.at(0);

    View->centerOn(d.cx,d.cy);

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
        CrosslineItem1->setPen(pen);
        scene->addItem(CrosslineItem1);
        CrosslineItem2->setPen(pen);
        scene->addItem(CrosslineItem2);

}

