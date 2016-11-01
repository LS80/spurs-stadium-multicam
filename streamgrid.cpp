#include "streamgrid.h"
#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QMediaPlayer>
// #include <QMediaContent>
#include <QVideoWidget>
#include <QUrl>
#include <QString>
#include <QKeyEvent>

StreamGrid::StreamGrid()
  : mainWidget(new QWidget)
{
  setCentralWidget(mainWidget);

  QGridLayout *layout = new QGridLayout;
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  mainWidget->setLayout(layout);

  QString streams[] = { "04asu087t_1@409528",
                        "074tzyqju_1@394719",
                        "0i1p9ue46_1@409669",
                        "04et6qkjl_1@409668" };

  const QString urlFmt = "http://streamamghdl-lh.akamaihd.net/i/%1/index_1_av-p.m3u8";

  int i = 0;
  for (int row=0; row<2; row++) {
    for (int column=0; column<2; column++) {
      QMediaPlayer *player = new QMediaPlayer;
      players[i] = player;
      QVideoWidget *videoWidget = new QVideoWidget;
      videoWidgets[i] = videoWidget;
      QString url = urlFmt.arg(streams[i]);
      player->setMedia(QUrl(url));
      player->setVideoOutput(videoWidget);
      layout->addWidget(videoWidgets[i], row, column);
      i++;
    }
  }
}

void StreamGrid::start()
{
  for (int i=0; i<4; i++) {
    players[i]->play();
  }
}

void StreamGrid::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Escape) close();
}
