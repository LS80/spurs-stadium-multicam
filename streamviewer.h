#include <QWidget>
#include <QMediaPlayer>

class QKeyEvent;
class QVideoWidget;
class QGridLayout;

class StreamViewer : public QWidget
{
  Q_OBJECT
public:
  StreamViewer();
  void playStreamGrid();
private:
  QGridLayout *layout;
  QMediaPlayer* players[5];
  QVideoWidget* videoWidgets[5];
  void showStreamGrid();
  void showStreamFullScreen(int index);
  void keyPressEvent(QKeyEvent *event);
public slots:
  void checkIfStreamGridPlaying(QMediaPlayer::MediaStatus state);
signals:
  void streamGridPlaying();
};
