#include <QMainWindow>

class QKeyEvent;
class QWidget;
// #include <QMediaPlayer>
class QMediaPlayer;
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
protected:
  void keyPressEvent(QKeyEvent *event);
};
