#include "streamgrid.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  StreamGrid streams;
  QObject::connect(&streams, &StreamGrid::ready, &streams, &StreamGrid::showFullScreen);
  streams.start();

  return app.exec();
}
