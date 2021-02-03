//
// Created by justin on 2020/11/19.
//
#include "MultimediaPlayer.h"
#include "PlayControl.h"
#include "AdvancedControl.h"
#include "PlaylistModel.h"
#include "HistogramWidget.h"
#include "VideoWidget.h"
#include "PlayListItemDelegate.h"

#include <QVideoWidget>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStyle>
#include <QMediaPlaylist>
#include <QListView>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMessageBox>
#include <QMediaMetaData>
#include <QStatusBar>
#include <QTime>
#include <QFormLayout>
#include <QFile>
#include <QScreen>
#include <QImageWriter>

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    m_player = new QMediaPlayer(this);
    m_player->setAudioRole(QAudio::VideoRole);

    m_playlist = new QMediaPlaylist();
    m_player->setPlaylist(m_playlist);

    connect(m_player, &QMediaPlayer::durationChanged, this, &MultimediaPlayer::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MultimediaPlayer::positionChanged);
    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &MultimediaPlayer::metaDataChanged);
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &MultimediaPlayer::playlistPositionChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MultimediaPlayer::statusChanged);
    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &MultimediaPlayer::bufferingProgress);
    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &MultimediaPlayer::videoAvailableChanged);
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this,
            &MultimediaPlayer::displayErrorMessage);
    connect(m_player, &QMediaPlayer::stateChanged, this, &MultimediaPlayer::stateChanged);

    m_videoWidget = new VideoWidget(this);
    m_player->setVideoOutput(m_videoWidget);

    m_playlistModel = new PlaylistModel(this);
    m_playlistModel->setPlaylist(m_playlist);

    m_playlistView = new QListView(this);
    m_playlistView->setItemDelegate(new PlayListItemDelegate(this));
    m_playlistView->setModel(m_playlistModel);
    m_playlistView->setCurrentIndex(m_playlistModel->index(m_playlist->currentIndex(), 0));

    connect(m_playlistView, &QAbstractItemView::activated, this, &MultimediaPlayer::jump);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, m_player->duration() / 1000);

    m_labelDuration = new QLabel(this);
    connect(m_slider, &QSlider::sliderMoved, this, &MultimediaPlayer::seek);

    m_labelHistogram = new QLabel(this);
    m_labelHistogram->setText("Histogram: ");
    m_videoHistogram = new HistogramWidget(this);
    m_audioHistogram = new HistogramWidget(this);
    QHBoxLayout *histogramLayout = new QHBoxLayout;
    histogramLayout->addWidget(m_labelHistogram);
    histogramLayout->addWidget(m_videoHistogram, 1);
    histogramLayout->addWidget(m_audioHistogram, 2);

    m_videoProbe = new QVideoProbe(this);
    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_videoHistogram, &HistogramWidget::processFrame);
    m_videoProbe->setSource(m_player);

    m_audioProbe = new QAudioProbe(this);
    connect(m_audioProbe, &QAudioProbe::audioBufferProbed, m_audioHistogram, &HistogramWidget::processBuffer);
    m_audioProbe->setSource(m_player);

    // set PlayControl
    PlayControl *playControl = new PlayControl(this);
    playControl->setState(m_player->state());
    playControl->setVolume(m_player->volume());
    playControl->setMuted(playControl->isMuted());

    connect(playControl, &PlayControl::play, this, &MultimediaPlayer::play);
    connect(playControl, &PlayControl::pause, m_player, &QMediaPlayer::pause);
    connect(playControl, &PlayControl::stop, m_player, &QMediaPlayer::stop);
    connect(playControl, &PlayControl::next, m_playlist, &QMediaPlaylist::next);
    connect(playControl, &PlayControl::previous, this, &MultimediaPlayer::previousClicked);
    connect(playControl, &PlayControl::changeVolume, m_player, &QMediaPlayer::setVolume);
    connect(playControl, &PlayControl::changeMuting, m_player, &QMediaPlayer::setMuted);
    connect(playControl, &PlayControl::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
    connect(playControl, &PlayControl::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));

    connect(m_player, &QMediaPlayer::stateChanged, playControl, &PlayControl::setState);
    connect(m_player, &QMediaPlayer::volumeChanged, playControl, &PlayControl::setVolume);
    connect(m_player, &QMediaPlayer::mutedChanged, playControl, &PlayControl::setMuted);

    m_openBtn = new QPushButton("Open", this);
    m_openBtn->setObjectName("openBtn");
    connect(m_openBtn, &QAbstractButton::clicked, this, &MultimediaPlayer::open);

    m_clearBtn = new QPushButton("Clear", this);
    m_clearBtn->setObjectName("clearBtn");
    m_clearBtn->setEnabled(false);
    connect(m_clearBtn, &QAbstractButton::clicked, this, &MultimediaPlayer::clear);

    m_fullScreenBtn = new QPushButton("Full Screen", this);
    m_fullScreenBtn->setCheckable(true);

    m_colorBtn = new QPushButton("Color Options", this);
    m_colorBtn->setEnabled(false);
    connect(m_colorBtn, &QPushButton::clicked, this, &MultimediaPlayer::showColorDialog);

    QBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(m_videoWidget, 2);
    displayLayout->addWidget(m_playlistView);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(m_openBtn);
    controlLayout->addWidget(m_clearBtn);
    controlLayout->addWidget(m_fullScreenBtn);
    controlLayout->addWidget(m_colorBtn);
    controlLayout->addWidget(playControl);

    AdvancedControl *advancedControl= new AdvancedControl(this);
    connect(this, &MultimediaPlayer::activeAdvancedControl, advancedControl, &AdvancedControl::active);

    QHBoxLayout *sliderDurationLayout = new QHBoxLayout;
    sliderDurationLayout->addWidget(m_slider);
    sliderDurationLayout->addWidget(m_labelDuration);

    QBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(displayLayout);
    mainLayout->addLayout(sliderDurationLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(advancedControl);
    mainLayout->addLayout(histogramLayout);

    setLayout(mainLayout);

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));
        // set all Widgets disabled
        playControl->setEnabled(false);
        m_playlistView->setEnabled(false);
        m_openBtn->setEnabled(false);
        m_clearBtn->setEnabled(false);
        m_colorBtn->setEnabled(false);
        m_fullScreenBtn->setEnabled(false);
    }

    QFile styleFile(":/qss/multimediaPlayer.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    setStyleSheet(styleSheet);

    metaDataChanged();
}

MultimediaPlayer::~MultimediaPlayer() {
}

bool MultimediaPlayer::isPlayerAvailable() const {
    return m_player->isAvailable();
}

void MultimediaPlayer::play() {
    m_player->play();

    emit activeAdvancedControl(m_player);
}

void MultimediaPlayer::open() {
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = m_player->supportedMimeTypes();

    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
//        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(
            QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0, QDir::homePath())
//            QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath())
    );

    if (fileDialog.exec() == QDialog::Accepted) {
        addToPlaylist(fileDialog.selectedUrls());
    }
}

void MultimediaPlayer::clear() {
    m_player->stop();
    m_playlist->clear();
    m_clearBtn->setEnabled(false);
}

void MultimediaPlayer::screenShot() {
    // todo render current frame of playing video

    QRect contentRect = m_videoWidget->contentsRect();
    QPixmap screenContent(contentRect.size());
    m_videoWidget->render(&screenContent, QPoint(), QRegion(contentRect));

    const QString format = "png";

    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QFileDialog fileDialog(this, "Save Screen Shot as", saveDir);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(saveDir);

    QStringList mimeTypes;
    const QList<QByteArray> baMimeTypes = QImageWriter::supportedMimeTypes();
    for (const QByteArray &bf : baMimeTypes) {
        mimeTypes.append(bf);
    }
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);

    if (fileDialog.exec() != QDialog::Accepted) return;

    const QString fileName = fileDialog.selectedFiles().first();

    if (!screenContent.save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                .arg(QDir::toNativeSeparators(fileName)));
    }
}

// Check for ".m3u" playlists.
static bool isPlaylist(const QUrl &url) {
    if (!url.isLocalFile()) {
        return false;
    }
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void MultimediaPlayer::addToPlaylist(const QList<QUrl> &urls) {
    for (auto &url: urls) {
        if (isPlaylist(url)) {
            m_playlist->load(url);
        } else {
            m_playlist->addMedia(url);
        }
    }
    m_clearBtn->setEnabled(true);
}

void MultimediaPlayer::setCustomAudioRole(const QString &role) {
    m_player->setCustomAudioRole(role);
}

void MultimediaPlayer::durationChanged(qint64 duration) {
    m_duration = duration / 1000;
    m_slider->setMaximum(m_duration);
}

void MultimediaPlayer::positionChanged(qint64 progress) {
    if (!m_slider->isSliderDown()) {
        m_slider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void MultimediaPlayer::metaDataChanged() {
    if (m_player->isMetaDataAvailable()) {
        setTrackInfo(QString("%1 - %2")
                             .arg(m_player->metaData(QMediaMetaData::AlbumArtist).toString())
                             .arg(m_player->metaData(QMediaMetaData::Title).toString()));

        if (m_coverLabel) {
            QUrl url = m_player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            m_coverLabel->setPixmap(!url.isEmpty()
                                    ? QPixmap(url.toString())
                                    : QPixmap());
        }
    }
}

void MultimediaPlayer::previousClicked() {
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (m_player->position() <= 5000) {
        m_playlist->previous();
    } else {
        m_player->setPosition(0);
    }
}

void MultimediaPlayer::jump(const QModelIndex &index) {
    if (index.isValid()) {
        m_playlist->setCurrentIndex(index.row());
        play();
    }
}

void MultimediaPlayer::playlistPositionChanged(int currentItem) {
    clearHistogram();
    m_playlistView->setCurrentIndex(m_playlistModel->index(currentItem, 0));
}

void MultimediaPlayer::seek(int seconds) {
    m_player->setPosition(seconds * 1000);
}

void MultimediaPlayer::statusChanged(QMediaPlayer::MediaStatus status) {
    handleCursor(status);

    // handle status message
    switch (status) {
        case QMediaPlayer::UnknownMediaStatus:
        case QMediaPlayer::NoMedia:
        case QMediaPlayer::LoadedMedia:
            setStatusInfo(QString());
            break;
        case QMediaPlayer::LoadingMedia:
            setStatusInfo(tr("Loading..."));
            break;
        case QMediaPlayer::BufferingMedia:
        case QMediaPlayer::BufferedMedia:
            setStatusInfo(tr("Buffering %1%").arg(m_player->bufferStatus()));
            break;
        case QMediaPlayer::StalledMedia:
            setStatusInfo(tr("Stalled %1%").arg(m_player->bufferStatus()));
            break;
        case QMediaPlayer::EndOfMedia:
//            QApplication::alert(this);
            break;
        case QMediaPlayer::InvalidMedia:
            displayErrorMessage();
            break;
    }
}

void MultimediaPlayer::stateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::StoppedState)
        clearHistogram();
}

void MultimediaPlayer::handleCursor(QMediaPlayer::MediaStatus status) {
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void MultimediaPlayer::bufferingProgress(int progress) {
    if (m_player->mediaStatus() == QMediaPlayer::StalledMedia)
        setStatusInfo(tr("Stalled %1%").arg(progress));
    else
        setStatusInfo(tr("Buffering %1%").arg(progress));
}

void MultimediaPlayer::videoAvailableChanged(bool available) {
    if (!available) {
        disconnect(m_fullScreenBtn, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
        disconnect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenBtn, &QPushButton::setChecked);
        m_videoWidget->setFullScreen(false);
    } else {
        connect(m_fullScreenBtn, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
        connect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenBtn, &QPushButton::setChecked);

        if (m_fullScreenBtn->isChecked())
            m_videoWidget->setFullScreen(true);
    }
    m_colorBtn->setEnabled(available);
}

void MultimediaPlayer::setTrackInfo(const QString &info) {
    m_trackInfo = info;

    if (m_statusBar) {
        m_statusBar->showMessage(m_trackInfo);
        m_statusLabel->setText(m_statusInfo);
    } else {
        if (!m_statusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
        else
            setWindowTitle(m_trackInfo);
    }
}

void MultimediaPlayer::setStatusInfo(const QString &info) {
    m_statusInfo = info;

    if (m_statusBar) {
        m_statusBar->showMessage(m_trackInfo);
        m_statusLabel->setText(m_statusInfo);
    } else {
        if (!m_statusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
        else
            setWindowTitle(m_trackInfo);
    }
}

void MultimediaPlayer::displayErrorMessage() {
    setStatusInfo(m_player->errorString());
}

void MultimediaPlayer::updateDurationInfo(qint64 currentInfo) {
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                          currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
                        m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    m_labelDuration->setText(tStr);
}

void MultimediaPlayer::showColorDialog() {
    if (!m_colorDialog) {
        QSlider *brightnessSlider = new QSlider(Qt::Horizontal);
        brightnessSlider->setRange(-100, 100);
        brightnessSlider->setValue(m_videoWidget->brightness());
        connect(brightnessSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setBrightness);
        connect(m_videoWidget, &QVideoWidget::brightnessChanged, brightnessSlider, &QSlider::setValue);

        QSlider *contrastSlider = new QSlider(Qt::Horizontal);
        contrastSlider->setRange(-100, 100);
        contrastSlider->setValue(m_videoWidget->contrast());
        connect(contrastSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setContrast);
        connect(m_videoWidget, &QVideoWidget::contrastChanged, contrastSlider, &QSlider::setValue);

        QSlider *hueSlider = new QSlider(Qt::Horizontal);
        hueSlider->setRange(-100, 100);
        hueSlider->setValue(m_videoWidget->hue());
        connect(hueSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setHue);
        connect(m_videoWidget, &QVideoWidget::hueChanged, hueSlider, &QSlider::setValue);

        QSlider *saturationSlider = new QSlider(Qt::Horizontal);
        saturationSlider->setRange(-100, 100);
        saturationSlider->setValue(m_videoWidget->saturation());
        connect(saturationSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setSaturation);
        connect(m_videoWidget, &QVideoWidget::saturationChanged, saturationSlider, &QSlider::setValue);

        QFormLayout *layout = new QFormLayout;
        layout->addRow(tr("Brightness"), brightnessSlider);
        layout->addRow(tr("Contrast"), contrastSlider);
        layout->addRow(tr("Hue"), hueSlider);
        layout->addRow(tr("Saturation"), saturationSlider);

        QPushButton *button = new QPushButton(tr("Close"));
        layout->addRow(button);

        m_colorDialog = new QDialog(this);
        m_colorDialog->setWindowTitle(tr("Color Options"));
        m_colorDialog->setLayout(layout);

        connect(button, &QPushButton::clicked, m_colorDialog, &QDialog::close);
    }
    m_colorDialog->show();
}

void MultimediaPlayer::clearHistogram() {
    QMetaObject::invokeMethod(m_videoHistogram, "processFrame", Qt::QueuedConnection,
                              Q_ARG(QVideoFrame, QVideoFrame()));
    QMetaObject::invokeMethod(m_audioHistogram, "processBuffer", Qt::QueuedConnection,
                              Q_ARG(QAudioBuffer, QAudioBuffer()));
}