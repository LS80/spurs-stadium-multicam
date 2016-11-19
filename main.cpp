#include "streamviewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  StreamViewer viewer;
  QObject::connect(&viewer, &StreamViewer::streamGridPlaying,
                   &viewer, &StreamViewer::showFullScreen);
  viewer.playStreamGrid();

  return app.exec();
}
