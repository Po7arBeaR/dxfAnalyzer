
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
{
    ui->setupUi(this);
    fileDialog->setWindowTitle("请选择文件");
    fileDialog->setDirectory("./");
    fileDialog->setNameFilter(tr("File(*.dxf* *.DXF*)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    scene->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
    View->setInteractive(true);
    View->setDragMode(QGraphicsView::ScrollHandDrag);
    View->setScene(scene);
    View->setRenderHints(QPainter::SmoothPixmapTransform);
    vLayoutParam->addWidget(ui->PB_Load);
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
    ui->tw_AnalyzedData->setColumnCount(2);
    ui->tw_AnalyzedData->setRowCount(0);
    scene->clear();
    dxfReader dxfreader(FileNames.join("/"));
    QPen pen;
    pen.setColor(Qt::yellow);
    pen.setWidth(0);
    for(auto d: dxfreader.dxfLines) {
        QLineF line(d.x1 , d.y1, d.x2,  d.y2);
        LinesData << d;
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
        pen.setCosmetic(true);
        lineItem->setPen(pen);
        scene->addItem(lineItem);
    }
    for(auto d:dxfreader.dxfCircle)
    {
        QRectF Rect(d.cx-d.radius,d.cy-d.radius,d.radius*2,d.radius*2);
        CircleData<<d;
        QGraphicsEllipseItem *CircleItem=new QGraphicsEllipseItem(Rect);
        pen.setCosmetic(true);
        CircleItem->setPen(pen);
        scene->addItem(CircleItem);
    }
    tbRowCount=0;


    for(auto db : dxfreader.dxfCircle)
    {  tbRowCount= ui->tw_AnalyzedData->rowCount();
        ui->tw_AnalyzedData->insertRow(tbRowCount);
        ui->tw_AnalyzedData->setItem(tbRowCount,0,new QTableWidgetItem(QString::number(db.cx)));
        ui->tw_AnalyzedData->setItem(tbRowCount,1,new QTableWidgetItem(QString::number(db.cy)));
    }
    auto d=dxfreader.dxfCircle.at(0);

    View->centerOn(d.cx,d.cy);
}


void Widget::on_tw_AnalyzedData_cellClicked(int row, int column)
{
    qreal x= ui->tw_AnalyzedData->item(row,0)->text().toDouble();
    qreal y= ui->tw_AnalyzedData->item(row,1)->text().toDouble();
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(0);

    QLineF line1(x,y+10,x,y-10);
    QLineF line2(x+10,y,x-10,y);
    QGraphicsLineItem *lineItem1 = new QGraphicsLineItem(line1);
    QGraphicsLineItem *lineItem2 = new QGraphicsLineItem(line2);
    pen.setCosmetic(true);
    lineItem1->setPen(pen);
    scene->addItem(lineItem1);
    lineItem2->setPen(pen);
    scene->addItem(lineItem2);

}

