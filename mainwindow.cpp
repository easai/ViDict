#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "aboutdialog.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>
#include <QSettings>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      net_manager(new QNetworkAccessManager(this)), net_reply(nullptr),
      m_data_buffer(new QByteArray) {
  ui->setupUi(this);
  ui->radioButton_en->setChecked(true);

  connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::lookup);
  connect(ui->action_Quit, &QAction::triggered, this, &QApplication::quit);
  connect(ui->action_About, &QAction::triggered, this, &MainWindow::about);
  connect(ui->actionAbout_Qt, &QAction::triggered, this,
          &QApplication::aboutQt);
  connect(ui->actionCu_t, &QAction::triggered, this, &MainWindow::cut);
  connect(ui->action_Paste, &QAction::triggered, this, &MainWindow::paste);
  connect(ui->action_Copy, &QAction::triggered, this, &MainWindow::copy);
  connect(ui->action_Undo, &QAction::triggered, this, &MainWindow::undo);
  connect(ui->action_Redo, &QAction::triggered, this, &MainWindow::redo);
  connect(ui->action_Preference, &QAction::triggered, this, &MainWindow::preference);

  loadSettings();
}

MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
  delete m_data_buffer;
}

void MainWindow::saveSettings() {
  QSettings settings(AUTHOR, APPNAME);
  settings.beginGroup(WINDOW);
  settings.setValue(GEOMETRY, saveGeometry());
  settings.endGroup();
}

void MainWindow::loadSettings() {
  QSettings settings(AUTHOR, APPNAME);
  settings.beginGroup(WINDOW);
  restoreGeometry(settings.value(GEOMETRY).toByteArray());
  settings.endGroup();
}

void MainWindow::lookup() {
  QString word;
  word = ui->lineEdit->text();
  if (word.isEmpty())
    return;
  QString lang = "en";
  if (!ui->radioButton_en->isChecked()) {
    lang = "vi";
  }
  QString url =
      "https://botudien.pythonanywhere.com/api/lookup/" + lang + "/" + word;
  _lookup(&url);
}

void MainWindow::preference()
{
}

void MainWindow::setBackground()
{
  QColor color=QColorDialog::getColor(background,this,"Choose Background Color");
  if(color.isValid()){
    background=color;
    QString css=QString("background : %1").arg(color.name());
    ui->textEdit->setStyleSheet(css);
  }
}

QColor MainWindow::getBackground() const
{
  return background;
}

void MainWindow::_lookup(QString *url) {
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
    if(array.size()<=0){
      ui->textEdit->append("Not found");
    }
    m_data_buffer->clear();
  }
}
void MainWindow::about() {
  AboutDialog *dlg=new AboutDialog(this);
  dlg->exec();
}

void MainWindow::redo() { ui->textEdit->redo(); }

void MainWindow::undo() { ui->textEdit->undo(); }

void MainWindow::paste() { ui->textEdit->paste(); }

void MainWindow::copy() { ui->textEdit->copy(); }

void MainWindow::cut() { ui->textEdit->cut(); }
