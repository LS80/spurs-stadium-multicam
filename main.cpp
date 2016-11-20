#include "streamviewer.h"

#include <vector>
#include <algorithm>

#include <QApplication>


QUrl streamUrl (QString id) {
  const QString urlFmt = "http://streamamghdl-lh.akamaihd.net/i/%1/index_1_av-p.m3u8";
  return QUrl(urlFmt.arg(id));
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  std::vector<QString> ids = { "04asu087t_1@409528",
                               "074tzyqju_1@394719",
                               "0i1p9ue46_1@409669",
                               "04et6qkjl_1@409668",
                               "0xorfu8d8_1@308855" };

  std::vector<QUrl> streams;
  streams.resize(ids.size());
  std::transform (ids.begin(), ids.end(), streams.begin(), streamUrl);

  StreamViewer viewer(streams);
  viewer.showFullScreen();

  return app.exec();
}
