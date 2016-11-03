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
  void start();
private:
  QWidget *mainWidget;
  QMediaPlayer* players[4];
  QVideoWidget* videoWidgets[4];
public slots:
  void changedMediaStatus(QMediaPlayer::MediaStatus state);
signals:
  void ready();
protected:
  void keyPressEvent(QKeyEvent *event);
};
