
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
#include "interactiveview.h"
#include "dxfreader.h"
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

    void on_tw_AnalyzedData_cellClicked(int row, int column);

private:
    Ui::Widget *ui;
    QFileDialog *fileDialog ;
    QGraphicsScene *scene;
    InteractiveView* View;
    QHBoxLayout* hLayoutXCenter;
    QHBoxLayout* hLayoutYCenter;
    QHBoxLayout *hLayoutXOffset;
    QHBoxLayout *hLayoutYOffset;
    QHBoxLayout *hLayoutZoom;
    QVBoxLayout *vLayoutParam;
    QHBoxLayout *hLayoutMain;
private:
    QList<DL_LineData> LinesData;
    QList<DL_CircleData> CircleData;
    QList<DL_ArcData> ArcData ;
    QList<DL_InsertData> InsertData;
    int tbRowCount;
};

#endif // WIDGET_H
