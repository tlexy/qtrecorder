#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <thread>
#include <memory>

class QLineEdit;
class QPushButton;
class WatchWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initLayout();
    void do_record(const QString&);

private:
    QLineEdit* _line;
    QPushButton* _s_btn;
    QPushButton* _e_btn;
    WatchWidget* _watch;
    bool recorder_close_;
    std::shared_ptr<std::thread> _recorder_thread;

public slots:
    void startRecord();
    void stopRecord();
};

#endif // WIDGET_H
