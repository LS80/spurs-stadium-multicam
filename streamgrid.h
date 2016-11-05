#include <QMainWindow>
#include <QMediaPlayer>

class QKeyEvent;
class QWidget;
class QVideoWidget;

class StreamGrid : public QMainWindow
{
  Q_OBJECT
public:
  StreamGrid();
  void playStreamGrid();
private:
  QWidget *mainWidget;
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
