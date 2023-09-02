#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define AUTHOR "easai"
#define APPNAME "ViDict"
#define WINDOW "Window"
#define GEOMETRY "Geometry"
#define LANG "Lang"
#define DEFAULT_LANG "en"

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QColor>

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

  QColor getBackground() const;

  QString lang() const;

  public slots:
  void dataReadyToRead();
  void dataReadFinished();
  void setLang();

private slots:
  void about();
  void redo();
  void undo();
  void paste();
  void copy();
  void cut();
  void lookup();
  void preference();
  void setBackground();

private:
  Ui::MainWindow *ui;
  QNetworkAccessManager *net_manager;
  QNetworkReply *net_reply;
  QByteArray *m_data_buffer;
  QString m_lang;
  QColor background;
  void _lookup(QString *);
  void saveSettings();
  void loadSettings();
};
#endif // MAINWINDOW_H
