#ifndef DCAUDIOPLAYER_H
#define DCAUDIOPLAYER_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QMap>
#include <QList>

class DcAudioPlayer: public QObject
{
    Q_OBJECT

public:
    DcAudioPlayer(int sample_rates, int channels, int sample_size);

    void userDefaultSetting();
    void setVolume(qreal volume);
    qreal getVolume();
    // 获得所有可用的设备
    QMap<int, QString> getAllDevice();
    void setDeviceByIndex(int);
    //是否可用
    bool open();
    void start();
    //播放PCM数据
    void play(const char* buf, size_t len);

private:
    int channel_num_; // 声道数量
    int sample_rate_; // 采样率
    int sample_bits_; // 存储大小

    QList<QAudioDeviceInfo> device_list_; //音频录入设备列表
    QAudioDeviceInfo curr_device_; //当前输入设备
    QAudioFormat fmt_;
    QMap<int, QString> device_map_;
    QAudioOutput* player_;
    QIODevice* io_device_;
};

#endif // DCAUDIOPLAYER_H
