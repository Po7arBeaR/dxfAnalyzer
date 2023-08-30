
#ifndef ASYNCTHREAD_H
#define ASYNCTHREAD_H
#include <QObject>
#include <QThread>
#include "dxfreader.h"
#include <QDebug>
class asyncThread : public QThread
{
    Q_OBJECT
public:
    explicit asyncThread(QObject *parent = nullptr);
    void run()override
    {
            emit SendData(m_clist,m_Llist);
         //   emit SendLine(m_Llist);

    }
signals:
    void SendData(QList<DL_CircleData> clist,QList<DL_LineData>llist);
   // void SendLine(QList<DL_LineData>list);
public:
    QList<DL_CircleData> m_clist;
    QList<DL_LineData> m_Llist;
};


#endif // ASYNCTHREAD_H
