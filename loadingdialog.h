#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H
#include <QMovie>
#include <QLabel>
#include <QDialog>
#include <QPainter>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#define USER_CANCEL -1
class LoadingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    ~LoadingDialog();

    //设置是否显示取消等待按钮
    void setCanCancel(bool bCanCancel);
    //移动到指定窗口中间显示
    void moveToCenter(QWidget* pParent);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void initUi();
Q_SIGNALS:
    void cancelWaiting();
private slots:
    void cancelBtnClicked();
public slots:
    //设置提示文本
    void setTipsText(QString strTipsText);
private:
    QFrame *m_pCenterFrame;
    QLabel *m_pMovieLabel;
    QMovie *m_pLoadingMovie;
    QLabel *m_pTipsLabel;
    QPushButton *m_pCancelBtn;
};
#endif // LOADINGDIALOG_H
