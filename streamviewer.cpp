#include "streamviewer.h"

StreamViewer::StreamViewer(const std::vector<QUrl>& streams)
  : layout(new QStackedLayout), streams(streams), numStreams(streams.size())
{
  setAttribute(Qt::WA_QuitOnClose);
  setWindowFlags(Qt::FramelessWindowHint);
  setLayout(layout);

  QPalette *palette = new QPalette;
  palette->setColor(QPalette::Background, Qt::white);
  setPalette(*palette);

  QLabel *label = new QLabel;
  label->setAlignment(Qt::AlignCenter);
  loading = new QMovie(":loading.gif");
  label->setMovie(loading);
  layout->insertWidget(0, label);

  for (const QUrl &stream : streams) {
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(stream);
    player->play();
    players.push_back(player);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &StreamViewer::checkIfStreamsReady);
  }

  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->setSpacing(0);
  gridLayout->setContentsMargins(0, 0, 0, 0);
  QWidget *gridWidget = new QWidget();
  gridWidget->setLayout(gridLayout);
  layout->insertWidget(1, gridWidget);

  int i = 0;
  for (int row=0; row<2; row++) {
    for (int column=0; column<2; column++) {
      QVideoWidget *gridVideoWidget = new QVideoWidget;
      gridVideoWidgets.push_back(gridVideoWidget);
      players[i]->setVideoOutput(gridVideoWidget);
      gridLayout->addWidget(gridVideoWidget, row, column);
      i++;
    }
  }

  for (int i=4; i<numStreams; i++) {
    QVideoWidget *videoWidget = new QVideoWidget;
    players[i]->setVideoOutput(videoWidget);
    layout->insertWidget(i - 2, videoWidget);
  }

  connect(this, SIGNAL(streamsReady()), this, SLOT(showStartingScreen()));
  loading->start();
}

void StreamViewer::showStartingScreen()
{
  showStreamGrid();
  loading->stop();
}

void StreamViewer::checkIfStreamsReady(QMediaPlayer::MediaStatus state)
{
  Q_UNUSED(state);
  for (const auto &player : players) {
    if (player->mediaStatus() == QMediaPlayer::BufferedMedia ||
        player->mediaStatus() == QMediaPlayer::StalledMedia ||
        player->state() == QMediaPlayer::StoppedState) {
      disconnect(player, &QMediaPlayer::mediaStatusChanged, this, &StreamViewer::checkIfStreamsReady);
    } else return;
  }
  emit streamsReady();
}

void StreamViewer::showStreamGrid()
{
  for (int i=0; i<4; i++) {
    gridVideoWidgets[i]->show();
  }
  layout->setCurrentIndex(1);
}

void StreamViewer::showStreamFullScreen(int index)
{
  if (index > 3) layout->setCurrentIndex(index - 2);
  else {
    gridVideoWidgets[index]->show();
    for (int i=0; i<4; i++) {
      if (i != index) {
        gridVideoWidgets[i]->hide();
      }
    }
    layout->setCurrentIndex(1);
  }
}

void StreamViewer::keyPressEvent(QKeyEvent *event)
{
  int key = event->key();
  int keyIndex = key - Qt::Key_1;
  if (keyIndex >= 0 && keyIndex < numStreams) showStreamFullScreen(keyIndex);
  else {
    switch(key) {
      case Qt::Key_0:
      case Qt::Key_F11:
        showStreamGrid();
        break;
      case Qt::Key_Escape:
        for (QMediaPlayer* player : players) player->stop();
        close();
        break;
    }
  }
}
