#ifndef DCRECORDER_H
#define DCRECORDER_H

#include <QObject>
#include <QAudioInput>
#include <QString>
#include <QMap>

class DCRecorder : public QObject
{
    Q_OBJECT
public:
    // 采样大小
    enum SAMPLESIZE
    {
        SAMPLE_SIZE_8 = 8,
        SAMPLE_SIZE_16 = 16
    };
public:
    explicit DCRecorder(QObject *parent = nullptr);

    // 使用默认设置（包括默认的录入设置，采样率等）
    void useDefaultSettings();
    bool open();
    // 开始录音
    void start();
    void stop();
    void setVolume(qreal volume);
    qreal getVolume();
    // 获得所有可用的设备
    QMap<int, QString> getAllDevice();
    // 设置用于录音的设置索引，索引由getAllDevice()方法返回，设置非法的索引将会使用默认录入设备
    void setDeviceByIndex(int);
    QList<int> GetAllSampleRates();
    QList<int> GetAllSampleChannels();
    // 已经录入数据大小
    size_t bytesReady();
    size_t read(char* buf, size_t len);
    // 设置一帧的大小
    void setFrameSize(int size);
    void setChannelNum(int channels);
    void setSampleRate(int sample_rates);
    void setSampleSize(SAMPLESIZE sample_size);
    void setSampleType(QAudioFormat::SampleType sample_type);

    int getFrameSize() { return frame_size_; }
    int getChannelNum() { return fmt_.channelCount(); }
    int getSampleRate() { return fmt_.sampleRate(); }
    int getSampleSize() { return fmt_.sampleSize(); }
    int getSampleType() { return fmt_.sampleType(); }


private:
    int frame_size_; // 帧大小
    int channel_num_; // 声道数量
    int sample_rate_; // 采样率
    int sample_bits_; // 存储大小

    QList<QAudioDeviceInfo> device_list_; //音频录入设备列表
    QAudioDeviceInfo curr_device_; //当前输入设备
    QAudioFormat fmt_;
    QMap<int, QString> device_map_;
    QAudioInput* recorder_;
    QIODevice* io_device_;
    QList<int> sample_support_rate_;
    QList<int> sample_support_channels_;

signals:
    void frameFull();

public slots:
};

#endif // DCRECORDER_H
