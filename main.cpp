#include "streamgrid.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  StreamGrid streams;
  QObject::connect(&streams, &StreamGrid::streamGridPlaying,
                   &streams, &StreamGrid::showFullScreen);
  streams.playStreamGrid();

  return app.exec();
}
