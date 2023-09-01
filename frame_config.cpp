
#include "frame_config.h"
#include <QRegularExpressionValidator>
#include <QFile>
#include <QtCore>

#define POSTS_DISTANCE "PostDistance"
#define FIND_DIRECTION "CheckDirection"
#define X_WAY "X"
#define Y_WAY "Y"
Frame_Config::Frame_Config():
    v_MainLayout(new QVBoxLayout)
    ,h_PostDistanceLayout(new QHBoxLayout)
    ,h_SaveLayout(new QHBoxLayout)
    ,h_AngleLayout(new QHBoxLayout)
    ,pb_save(new QPushButton)
    ,lab_PostDistance(new QLabel)
    ,LE_PostDistance(new QLineEdit)
    ,lab_Angle(new QLabel)
    ,cb_XWay(new QCheckBox)
    ,cb_YWay(new QCheckBox)

{
   // m_findAngle=-1;
    lab_PostDistance->setText("极柱间距");
    lab_Angle->setText("查找方向");
    cb_XWay->setText("X方向");
    cb_YWay->setText("Y方向");
    pb_save->setText("保存设置");
    LE_PostDistance->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9.-]+$")));
    h_PostDistanceLayout->addWidget(lab_PostDistance);
    h_PostDistanceLayout->addWidget(LE_PostDistance);
    h_AngleLayout->addWidget(lab_Angle);
    h_AngleLayout->addWidget(cb_XWay);
    h_AngleLayout->addWidget(cb_YWay);
    cb_XWay->setAutoExclusive(1);
    cb_YWay->setAutoExclusive(1);
    h_SaveLayout->addWidget(pb_save);
    v_MainLayout->addLayout(h_PostDistanceLayout);
    v_MainLayout->addLayout(h_AngleLayout);
    v_MainLayout->addLayout(h_SaveLayout);
    this->setLayout(v_MainLayout);
    QString FileName=QCoreApplication::applicationFilePath()+"/Config.ini";

    Config = new QSettings("./setting.ini",QSettings::IniFormat);

    if(QFile::exists(FileName))
    {
        m_PostDistance=Config->value(POSTS_DISTANCE,"0").toString();
        m_findAngle=Config->value(FIND_DIRECTION,X_WAY)==X_WAY? 1:0;
    }
    else
    {
        Config->setValue(POSTS_DISTANCE,"0");
        Config->setValue(FIND_DIRECTION,X_WAY);
        Config->sync();
       m_PostDistance=Config->value(POSTS_DISTANCE,"0").toString();
        m_findAngle=Config->value(FIND_DIRECTION,X_WAY)==X_WAY? 1:0;

    }
    if(m_findAngle)
    {
        cb_XWay->setChecked(1);
    }
    else
    {
        cb_YWay->setChecked(1);
    }
    LE_PostDistance->setText(m_PostDistance);
    connect(pb_save,SIGNAL(clicked()),this,SLOT(SaveChange()));
}

Frame_Config::~Frame_Config()
{
    delete Config;
}

void Frame_Config::SaveChange()
{
    Config->setValue(POSTS_DISTANCE,this->LE_PostDistance->text());
    if(this->cb_XWay->isChecked()&&(!this->cb_YWay->isChecked()))Config->setValue(FIND_DIRECTION,X_WAY);
    else if(this->cb_YWay->isChecked()&&(!this->cb_XWay->isChecked()))Config->setValue(FIND_DIRECTION,Y_WAY);
    m_PostDistance=Config->value(POSTS_DISTANCE,"0").toString();
    m_findAngle=Config->value(FIND_DIRECTION,X_WAY)==X_WAY? 1:0;
    if(m_findAngle)
    {
        cb_XWay->setChecked(1);
    }
    else
    {
        cb_YWay->setChecked(1);
    }
    LE_PostDistance->setText(m_PostDistance);
}

