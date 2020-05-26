#include "dcrecorder.h"

DCRecorder::DCRecorder(QObject *parent) : QObject(parent),
    frame_size_(1024),
    channel_num_(2),
    sample_rate_(44100),
    sample_bits_(16),
    recorder_(NULL),
    io_device_(NULL),
    curr_device_(QAudioDeviceInfo::defaultInputDevice())
{
    fmt_.setCodec("audio/pcm");
    fmt_.setSampleRate(sample_rate_);
    fmt_.setChannelCount(channel_num_);
    fmt_.setSampleSize(sample_bits_);
    fmt_.setByteOrder(QAudioFormat::LittleEndian);
    fmt_.setSampleType(QAudioFormat::SignedInt);

    sample_support_rate_ = curr_device_.supportedSampleRates();
    sample_support_channels_ = curr_device_.supportedChannelCounts();
    //
    device_list_ = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);//输入设备列表
    for(int i = 0; i < device_list_.count(); i++)
    {
       QAudioDeviceInfo device = device_list_.at(i);
       device_map_[i] = device.deviceName();
    }
}

 void DCRecorder::useDefaultSettings()
 {
     fmt_.setCodec("audio/pcm");
     fmt_.setSampleRate(44100);
     fmt_.setChannelCount(2);
     fmt_.setSampleSize(16);
     fmt_.setByteOrder(QAudioFormat::LittleEndian);
     fmt_.setSampleType(QAudioFormat::SignedInt);

     if (!curr_device_.isFormatSupported(fmt_))
     {
         fmt_ = curr_device_.nearestFormat(fmt_);
     }
 }

bool DCRecorder::open()
{
    if (curr_device_.isNull())
    {
        return false;
    }
    if (!curr_device_.isFormatSupported(fmt_))
    {
        //fmt = curr_device_.nearestFormat(fmt);
        return false;
    }
    return true;
}

void DCRecorder::start()
{
    if (curr_device_.isFormatSupported(fmt_))
    {
        recorder_ = new QAudioInput(fmt_);
        //开始录制音频
        io_device_ = recorder_->start();
    }
}

void DCRecorder::stop()
{
    if (recorder_ && io_device_)
    {
        recorder_->stop();
    }
}

void DCRecorder::setVolume(qreal volume)
{
    if (recorder_)
    {
        recorder_->setVolume(volume);
    }
}

qreal DCRecorder::getVolume()
{
    if (recorder_)
    {
        return recorder_->volume();
    }
    else
    {
        return 1.0;
    }

}

void DCRecorder::setDeviceByIndex(int idx)
{
    if (idx < 0 || idx >= device_map_.size())
    {
        curr_device_ = QAudioDeviceInfo::defaultInputDevice();
    }
    else
    {
        curr_device_ = device_list_.at(idx);
    }
    sample_support_rate_ = curr_device_.supportedSampleRates();
    sample_support_channels_ = curr_device_.supportedChannelCounts();
}

QList<int> DCRecorder::GetAllSampleRates()
{
    return sample_support_rate_;
}

QList<int> DCRecorder::GetAllSampleChannels()
{
    return sample_support_channels_;
}

QMap<int, QString> DCRecorder::getAllDevice()
{
    return device_map_;
}

size_t DCRecorder::bytesReady()
{
    if (recorder_)
    {
        return recorder_->bytesReady();
    }
    return 0;
}

size_t DCRecorder::read(char* buf, size_t len)
{
    if (recorder_ && io_device_)
    {
        return io_device_->read(buf, len);
    }
    return 0;
}

void DCRecorder::setFrameSize(int size)
{
    frame_size_ = size;
}

void DCRecorder::setChannelNum(int channels)
{
    fmt_.setChannelCount(channels);
}

void DCRecorder::setSampleRate(int sample_rates)
{
    fmt_.setSampleRate(sample_rates);
}

void DCRecorder::setSampleSize(SAMPLESIZE sample_size)
{
    fmt_.setSampleSize(sample_size);
}

void DCRecorder::setSampleType(QAudioFormat::SampleType sample_type)
{
    fmt_.setSampleType(sample_type);
}
