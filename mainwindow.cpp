#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      net_manager(new QNetworkAccessManager(this)), net_reply(nullptr),
      m_data_buffer(new QByteArray) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
  delete m_data_buffer;
}

void MainWindow::on_action_About_triggered() {
  QMessageBox::about(this, "About ViDict",
                     "ViDict v1.0\n\nCopyright 2023 easai");
}

void MainWindow::on_actionAbout_Qt_triggered() { QApplication::aboutQt(); }

void MainWindow::on_action_Quit_triggered() { QApplication::quit(); }

void MainWindow::on_action_Redo_triggered() { ui->textEdit->redo(); }

void MainWindow::on_action_Undo_triggered() { ui->textEdit->undo(); }

void MainWindow::on_action_Paste_triggered() { ui->textEdit->paste(); }

void MainWindow::on_action_Copy_triggered() { ui->textEdit->copy(); }

void MainWindow::on_actionCu_t_triggered() { ui->textEdit->cut(); }

void MainWindow::on_pushButton_clicked() {
  QString word;
  word = ui->lineEdit->text();
  if (word.isEmpty())
    return;
  QString url = "https://botudien.pythonanywhere.com/api/lookup/en/" + word;
  lookup(&url);
}

void MainWindow::on_pushButton_vi_clicked() {
  QString word;
  word = ui->lineEdit_vi->text();
  if (word.isEmpty())
    return;
  QString url = "https://botudien.pythonanywhere.com/api/lookup/vi/" + word;
  lookup(&url);
}

void MainWindow::lookup(QString *url) {
  ui->textEdit->clear();
  const QUrl API_ENDPOINT(*url);
  QNetworkRequest request;
  request.setUrl(API_ENDPOINT);

  net_reply = net_manager->get(request);
  connect(net_reply, &QIODevice::readyRead, this, &MainWindow::dataReadyToRead);
  connect(net_reply, &QNetworkReply::finished, this,
          &MainWindow::dataReadFinished);
}

void MainWindow::dataReadyToRead() {
  m_data_buffer->append(net_reply->readAll());
}

void MainWindow::dataReadFinished() {
  if (net_reply->error()) {
    qDebug() << "Error : " << QString(*m_data_buffer);
  } else {
    QJsonDocument doc = QJsonDocument::fromJson(*m_data_buffer);
    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); i++) {
      QJsonObject object = array.at(i).toObject();
      QVariantMap map = object.toVariantMap();
      QString vi = map["vi"].toString();
      QString en = map["en"].toString();
      ui->textEdit->append(vi + " : " + en + "\n");
    }
    m_data_buffer->clear();
  }
}
