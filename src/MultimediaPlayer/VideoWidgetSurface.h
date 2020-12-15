//
// Created by justin on 2020/12/15.
//

#pragma once

#include <QAbstractVideoSurface>


class VideoWidgetSurface : public QAbstractVideoSurface {
Q_OBJECT

public:

    VideoWidgetSurface(QWidget *widget, QObject *parent = 0);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    bool isFormatSupported(const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const;

    bool start(const QVideoSurfaceFormat &format);

    void stop();

    bool present(const QVideoFrame &frame);

    QRect videoRect() const { return targetRect; }

    void updateVideoRect();

    void paint(QPainter *painter);

private:
    QWidget *widget;
    QImage::Format imageFormat;
    QRect targetRect;
    QSize imageSize;
    QRect sourceRect;
    QVideoFrame currentFrame;

};
