#include "dcaudioplayer.h"

DcAudioPlayer::DcAudioPlayer(int sample_rates, int channels, int sample_size)
    :sample_rate_(sample_rates),
      sample_bits_(sample_size),
      channel_num_(channels),
      io_device_(NULL),
      player_(NULL),
      curr_device_(QAudioDeviceInfo::defaultOutputDevice())
{
    fmt_.setCodec("audio/pcm");
    fmt_.setSampleRate(sample_rate_);
    fmt_.setChannelCount(channel_num_);
    fmt_.setSampleSize(sample_bits_);
    fmt_.setByteOrder(QAudioFormat::LittleEndian);
    fmt_.setSampleType(QAudioFormat::SignedInt);

    device_list_ = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);//输入设备列表
    for(int i = 0; i < device_list_.count(); i++)
    {
       QAudioDeviceInfo device = device_list_.at(i);
       device_map_[i] = device.deviceName();
    }
}

void DcAudioPlayer::userDefaultSetting()
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

void DcAudioPlayer::setVolume(qreal volume)
{
    if (player_)
    {
        player_->setVolume(volume);
    }
}

qreal DcAudioPlayer::getVolume()
{
    if (player_)
    {
        return player_->volume();
    }
    else
    {
        return 1.0;
    }
}

// 获得所有可用的设备
QMap<int, QString> DcAudioPlayer::getAllDevice()
{
    return  device_map_;
}

void DcAudioPlayer::setDeviceByIndex(int idx)
{
    if (idx < 0 || idx >= device_map_.size())
    {
        curr_device_ = QAudioDeviceInfo::defaultInputDevice();
    }
    else
    {
        curr_device_ = device_list_.at(idx);
    }
}

bool DcAudioPlayer::open()
{
    if (!curr_device_.isFormatSupported(fmt_))
    {
        return false;
    }
    return true;
}

void DcAudioPlayer::start()
{
    if (curr_device_.isFormatSupported(fmt_))
    {
        player_ = new QAudioOutput(fmt_);
        //开始播放音频
        io_device_ = player_->start();
    }
}

//播放PCM数据
void DcAudioPlayer::play(const char* buf, size_t len)
{
    size_t psize = 0;
    if(player_ && io_device_
            && player_->state()!=QAudio::StoppedState
            && player_->state()!=QAudio::SuspendedState)
    {
        int chunks = player_->bytesFree()/player_->periodSize();
        size_t con = 0;
        while (chunks && con < len)
        {
            psize = len - con > player_->periodSize() ? player_->periodSize() : len - con;
            //写入到扬声器
            io_device_->write(buf + con, psize);
            con += psize;
            //--chunks;
        }
    }
}
