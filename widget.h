
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QProgressDialog>
#include <QTabWidget>
#include "readerprogressdialog.h"
#include "interactiveview.h"
#include "loadingdialog.h"
#include "findpost.h"
#include "asyncthread.h"
#include "frame_config.h"
#include "markpointtype.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_PB_Load_clicked();

  //  void on_tw_AnalyzedData_cellClicked(int row, int column);



    void on_tw_AnalyzedData_itemDoubleClicked(QTableWidgetItem *item);

    void on_pb_output_clicked();

    void on_tw_MarkPointData_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::Widget *ui;
    QFileDialog *fileDialog ;
    QFileDialog *outPutDialog;
    QGraphicsScene *scene;
    InteractiveView* View;
    QHBoxLayout* hLayoutXCenter;
    QHBoxLayout* hLayoutYCenter;
    QHBoxLayout *hLayoutXOffset;
    QHBoxLayout *hLayoutYOffset;
    QHBoxLayout *hLayoutZoom;
    QVBoxLayout *vLayoutParam;
    QHBoxLayout *hLayoutMain;
    QGridLayout *gLayoutZoffset;
    Frame_Config* ConfigQFrame;
    QTabWidget* TW_LeftParam;
   // QTableWidget* tw_MarkParam;
private:
    QList<DL_LineData> LinesData;
    QList<DL_CircleData> CircleData;
    QList<DL_ArcData> ArcData ;
    QList<DL_InsertData> InsertData;
    QList<DL_CircleData> BatteryPostData;
    QList<DL_CircleData> AllCircleData;
    int tbRowCount;
    int RowCount;
    bool CrosslineFlag;
    bool MarkCrosslineFlag;

    qreal scale;//缩放
    qreal DistanceOfPost;//极柱
    bool sortDistance;
    QString FileName;
    QGraphicsLineItem *CrosslineItem1;
    QGraphicsLineItem *CrosslineItem2;
    QGraphicsLineItem *MarkCrossLineItem1;//
    QGraphicsLineItem *MarkCrossLineItem2;//
    QGraphicsTextItem *MarkItem;
    QGraphicsTextItem *MarkPointItem;//
    QList<BatteryMark> BatteryList;
    QTextDocument *MarkPosText;
    QTextDocument *MarkPointPosText;//
    LoadingDialog * LoadDialog;
    QList<BatteryMark> SortedList;
    QList<MarkPointType> MarkList;
//    qreal BasePointx;
//    qreal BasePointy;
   // QProgressDialog* m_ProgressD;
     int LoadDialogCount;
public slots:
    void DrawItem(QList<DL_CircleData> clist,QList<DL_LineData> llist);
    void ShowTable();
    void ShowMark();
    void HideLoadDialog();
 //   void on_tw_MarkParam_DoubleClick();
signals:
    void SendTable();
    void SendMark();
    void CloseLoadDialog();
private:
    asyncThread * thread;

public:
    bool AnalyzeFile(QString FileNames);
    bool CircleFilter( QList<DL_CircleData>);
    bool MarkFilter(QList<DL_CircleData> ,qreal);
    bool InitBuffer();
};

#endif // WIDGET_H
