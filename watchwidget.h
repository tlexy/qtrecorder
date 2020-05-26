#ifndef WATCHWIDGET_H
#define WATCHWIDGET_H

#include <QWidget>
#include <QString>

class QLabel;
class QTimer;

class WatchWidget : public QWidget
{
    Q_OBJECT
public:
    WatchWidget(QWidget* parent = 0);

    void setInitTime(int hour, int minute, int second);
    void reset();
    void start();

private:
    int hour_;
    int minute_;
    int second_;
    QLabel* time_lbl_;

    QString sh_;
    QString sm_;
    QString ss_;
    QString show_text_;
    QString format_;

	QTimer* timer_;

private slots:
    void showTime();

private:
    void increment();
    QString toShowText();
};

#endif // WATCHWIDGET_H
