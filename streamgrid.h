#include <QWidget>
#include <QMediaPlayer>

class QKeyEvent;
class QVideoWidget;
class QGridLayout;

class StreamGrid : public QWidget
{
  Q_OBJECT
public:
  StreamGrid();
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
