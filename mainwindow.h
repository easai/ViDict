#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define AUTHOR "easai"
#define APPNAME "ViDict"
#define WINDOW "Window"
#define WIDTH "Width"
#define HEIGHT "Height"

#include <QMainWindow>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void dataReadyToRead();
  void dataReadFinished();

private slots:
  void about();
  void redo();
  void undo();
  void paste();
  void copy();
  void cut();
  void lookup();

private:
  Ui::MainWindow *ui;
  QNetworkAccessManager *net_manager;
  QNetworkReply *net_reply;
  QByteArray *m_data_buffer;
  void _lookup(QString *);
  void saveSettings();
  void loadSettings();
};
#endif // MAINWINDOW_H
