#include "widget.h"
#include <QSlider>
#include "dcrecorder.h"
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTimer>
#include <QLabel>
#include <thread>
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDateTime>
#include <QMessageBox>
#include "watchwidget.h"

static DCRecorder recorder;

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      recorder_close_(false)
{
    initLayout();

//    QTimer* timer = new QTimer(this);

//    connect(timer, SIGNAL(timeout()), this, SLOT(stopRecord()));
//    timer->start(10 * 1000);
    resize(400, 320);
}

 void Widget::initLayout()
 {
     QLabel* lbl_text = new QLabel(tr("请录音文件名："));
     _line = new QLineEdit;
     _s_btn = new QPushButton(tr("开始录音"));
     _e_btn = new QPushButton(tr("停止录音"));
     _watch = new WatchWidget();
     _watch->setStyleSheet(QString("QLabel{font-size:48px;}"));
     _s_btn->setStyleSheet(QString("QLabel{font-size:16px;}"));
     _e_btn->setStyleSheet(QString("QLabel{font-size:16px;}"));

     connect(_s_btn, SIGNAL(clicked()), this, SLOT(startRecord()));
     connect(_e_btn, SIGNAL(clicked()), this, SLOT(stopRecord()));

     QHBoxLayout* hLayout1 = new QHBoxLayout;
     hLayout1->addStretch();
     hLayout1->addWidget(lbl_text);
     hLayout1->addSpacing(5);
     hLayout1->addWidget(_line);
     hLayout1->addStretch();

     QHBoxLayout* hLayout2 = new QHBoxLayout;
     hLayout2->addWidget(_s_btn);
     hLayout2->addSpacing(5);
     hLayout2->addWidget(_e_btn);

     QVBoxLayout* vMainLayout = new QVBoxLayout;
     //vMainLayout->addLayout(hLayout1);
     vMainLayout->setMargin(20);
     vMainLayout->setSpacing(10);
     vMainLayout->addLayout(hLayout2);
     vMainLayout->addWidget(_watch);

     setLayout(vMainLayout);
 }

 void Widget::startRecord()
 {
     QString filename = _line->text();
     if (filename.trimmed() == "")
     {
         filename =  QDateTime::currentDateTime().toString("yyyy-MM-ddTHH_mm_ss.zzz") + "_out.pcm";
     }
     recorder_close_ = false;
     _watch->reset();
     _watch->start();
     _recorder_thread = std::make_shared<std::thread>(&Widget::do_record, this, filename);
     //do_record(filename);
 }

 void Widget::stopRecord()
 {
     recorder_close_ = true;
     _watch->reset();
     _recorder_thread->join();
 }

void Widget::do_record(const QString& filename)
{
    qDebug() << "start record....filename:" << filename;
    recorder.useDefaultSettings();
    if (recorder.open())
    {
        recorder.start();
    }
    else
    {
        QMessageBox::information(this, "warning", tr("录音设备打开失败"));
        _watch->reset();
        return;
    }
    char buf[4096] = {0};

    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return;

    while(true)
    {
        int n = 0;
        int len = 0;
        if ((n = recorder.bytesReady()) > 4096)
        {
            len = recorder.read(buf, 4096);
            file.write(buf, len);
            //qDebug() << "bytes:" << n << "\tread:" << len;
        }
        if (recorder_close_)
        {
            recorder.stop();
            file.close();
            break;
        }
        //QThread::msleep(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    qDebug() << "stop record...";
}

Widget::~Widget()
{

}
