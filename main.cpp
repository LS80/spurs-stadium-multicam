#include "streamgrid.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  StreamGrid grid;
  grid.showFullScreen();
  grid.start();

  return app.exec();
}
