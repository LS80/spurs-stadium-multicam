#include "streamgrid.h"

#include <QGridLayout>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>
#include <QString>
#include <QKeyEvent>

StreamGrid::StreamGrid()
  : layout(new QGridLayout)
{
  setAttribute(Qt::WA_QuitOnClose);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  QString streams[] = { "04asu087t_1@409528",
                        "074tzyqju_1@394719",
                        "0i1p9ue46_1@409669",
                        "04et6qkjl_1@409668",
                        "0xorfu8d8_1@308855" };

  const QString urlFmt = "http://streamamghdl-lh.akamaihd.net/i/%1/index_1_av-p.m3u8";

  for (int i=0; i<5; i++) {
    QMediaPlayer *player = new QMediaPlayer(this);
    players[i] = player;
    QVideoWidget *videoWidget = new QVideoWidget(this);
    videoWidgets[i] = videoWidget;
    QString url = urlFmt.arg(streams[i]);
    player->setMedia(QUrl(url));
    player->setVideoOutput(videoWidget);
  }

  int i = 0;
  for (int row=0; row<2; row++) {
    for (int column=0; column<2; column++) {
      layout->addWidget(videoWidgets[i], row, column);
      QObject::connect(players[i], &QMediaPlayer::mediaStatusChanged,
                       this, &StreamGrid::checkIfStreamGridPlaying);
      i++;
    }
  }
  videoWidgets[4]->hide();
  layout->addWidget(videoWidgets[4], 0, 2);
}

void StreamGrid::checkIfStreamGridPlaying(QMediaPlayer::MediaStatus state)
{
  for (int i=0; i<4; i++) {
    if (players[i]->mediaStatus() != QMediaPlayer::BufferedMedia) return;
  }
  emit streamGridPlaying();
}

void StreamGrid::playStreamGrid()
{
  for (int i=0; i<4; i++) players[i]->play();
}

void StreamGrid::showStreamGrid()
{
  for (int i=0; i<4; i++) {
    videoWidgets[i]->show();
  }
  videoWidgets[4]->hide();
}

void StreamGrid::showStreamFullScreen(int index)
{
  players[index]->play();
  videoWidgets[index]->show();
  for (int i=0; i<5; i++)
  {
    if (i != index) {
      videoWidgets[i]->hide();
    }
  }
}

void StreamGrid::keyPressEvent(QKeyEvent *event)
{
  switch(event->key())
  {
    case Qt::Key_Escape:
      close();
      break;
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
      showStreamFullScreen(event->key() - Qt::Key_1);
      break;
    case Qt::Key_0:
    case Qt::Key_F11:
      showStreamGrid();
      break;
  }
}
