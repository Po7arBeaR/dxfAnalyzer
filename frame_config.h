
#ifndef FRAME_CONFIG_H
#define FRAME_CONFIG_H

#include <QFrame>

#include <QObject>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QSettings>
class Frame_Config : public QFrame
{
    Q_OBJECT
public:
    Frame_Config();
    ~Frame_Config();
private:
    QVBoxLayout * v_MainLayout;
    QHBoxLayout * h_PostDistanceLayout;
    QHBoxLayout * h_AngleLayout;
    QHBoxLayout * h_SaveLayout;
private:
    QPushButton *pb_save;
    QLabel* lab_PostDistance;
    QLineEdit* LE_PostDistance;
    QLabel* lab_Angle;
    QCheckBox* cb_XWay;
    QCheckBox* cb_YWay;
private:
    QSettings* Config;
public:
    QString m_PostDistance;
    bool m_findAngle;
public slots:
    void SaveChange();
};

#endif // FRAME_CONFIG_H
