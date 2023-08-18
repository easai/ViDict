#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void dataReadyToRead();
    void dataReadFinished();

private slots:
    void on_action_About_triggered();

    void on_actionAbout_Qt_triggered();

    void on_action_Quit_triggered();

    void on_action_Redo_triggered();

    void on_action_Undo_triggered();

    void on_action_Paste_triggered();

    void on_action_Copy_triggered();

    void on_actionCu_t_triggered();

    void on_pushButton_vi_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *net_manager;
    QNetworkReply *net_reply;
    QByteArray *m_data_buffer;
    void lookup(QString *);

};
#endif // MAINWINDOW_H
