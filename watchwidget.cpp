#include "watchwidget.h"
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>

#define MAX_SECOND 60
#define MAX_MINUTE 60
#define MAX_HOUR 24

WatchWidget::WatchWidget(QWidget* parent)
    :QWidget(parent),
      hour_(0),
      minute_(0),
      second_(0),
	timer_(NULL),
      format_("%1: %2: %3")
{
    time_lbl_ = new QLabel(this);
    //time_lbl_->setText("fdfdfdsfdfdfd");
    time_lbl_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //time_lbl_->setStyleSheet(QString("QLabel{font-size:48px;}"));

    setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hMainLayout = new QHBoxLayout;
    hMainLayout->addStretch();
    hMainLayout->addWidget(time_lbl_);
    hMainLayout->addStretch();
    setLayout(hMainLayout);
}

QString WatchWidget::toShowText()
{
    format_ = "%1: %2: %3";
    sh_ = QString::number(hour_);
    sm_ = QString::number(minute_);
    ss_ = QString::number(second_);
    if (sh_.size() < 2)
    {
        sh_.push_front('0');
    }
    if (sm_.size() < 2)
    {
        sm_.push_front('0');
    }
    if (ss_.size() < 2)
    {
        ss_.push_front('0');
    }
    show_text_ = format_.arg(sh_, sm_, ss_);//.arg(sm_).arg(ss_);
    return show_text_;
}

void WatchWidget::increment()
{
    if (second_ >= MAX_SECOND)
    {
        second_ = 0;
        minute_ += 1;
    }
    if (minute_ >= MAX_MINUTE)
    {
        minute_ = 0;
        hour_ += 1;
    }
    if (hour_ >= MAX_HOUR)
    {
        hour_ = 0;
    }
}

void WatchWidget::start()
{
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(showTime()));
    timer_->start(1000);

    showTime();
}

void WatchWidget::showTime()
{
    time_lbl_->setText(toShowText());
    second_ += 1;
    increment();
    update();
}

void WatchWidget::setInitTime(int hour, int minute, int second)
{
    hour_ = hour;
    minute_ = minute;
    second_ = second;
}

void WatchWidget::reset()
{
	if (timer_)
	{
		timer_->stop();
	}
    hour_ = 0;
    minute_ = 0;
    second_ = 0;
}
