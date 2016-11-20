#include <vector>

#include <QWidget>
#include <QMediaPlayer>
#include <QStackedLayout>
#include <QVideoWidget>
#include <QUrl>
#include <QString>
#include <QKeyEvent>
#include <QLabel>
#include <QMovie>
#include <QPalette>

class StreamViewer : public QWidget
{
  Q_OBJECT
public:
  StreamViewer(const std::vector<QUrl>& streams);
private:
  QStackedLayout *layout;
  std::vector<QUrl> streams;
  std::vector<QMediaPlayer*> players;
  std::vector<QVideoWidget*> gridVideoWidgets;
  QMovie *loading;
  int numStreams;
  void showStreamGrid();
  void showStreamFullScreen(int index);
  void keyPressEvent(QKeyEvent *event);
public slots:
  void checkIfStreamsReady(QMediaPlayer::MediaStatus state);
  void showStartingScreen();
signals:
  void streamsReady();
};
