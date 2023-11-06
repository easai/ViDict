#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "aboutdialog.h"
#include "configdialog.h"
#include <QColorDialog>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      net_manager(new QNetworkAccessManager(this)), net_reply(nullptr),
      m_data_buffer(new QByteArray) {
  ui->setupUi(this);
  ui->radioButton_en->setChecked(true);
  setWindowIcon(QIcon("://images/vidict-favicon.ico"));
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
  connect(ui->action_Preference, &QAction::triggered, this,
          &MainWindow::preference);
  connect(ui->radioButton_en, &QPushButton::clicked, this,
          &MainWindow::setLang);
  connect(ui->radioButton_vi, &QPushButton::clicked, this,
          &MainWindow::setLang);
  connect(ui->action_Preference, &QAction::triggered, this,
          &MainWindow::setConfig);
  connect(ui->action_Open_Botudien, &QAction::triggered, this,
          &MainWindow::openBotudien);

  loadSettings();
  ui->radioButton_en->setChecked(m_lang == "en");
  ui->radioButton_vi->setChecked(m_lang == "vi");
  ui->textEdit->setFont(m_config.font());
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
  settings.setValue(LANG, m_lang);
  settings.setValue(APIFORMAT, m_config.format());
  settings.setValue(FONT, m_config.font().toString());
  settings.endGroup();
}

void MainWindow::loadSettings() {
  QSettings settings(AUTHOR, APPNAME);
  settings.beginGroup(WINDOW);
  restoreGeometry(settings.value(GEOMETRY).toByteArray());
  m_lang = settings.value(LANG, DEFAULT_LANG).toString();
  if (m_lang.isEmpty()) {
    m_lang = DEFAULT_LANG;
  }
  m_config.setFormat(settings.value(APIFORMAT, DEFAULT_APIFORMAT).toString());
  if (m_config.format().isEmpty()) {
    m_config.setFormat(DEFAULT_APIFORMAT);
  }
  QFont font = ui->textEdit->font();
  font.fromString(settings.value(FONT, font.toString()).toString());
  m_config.setFont(font);

  settings.endGroup();
}

void MainWindow::lookup() {
  QString word;
  word = ui->lineEdit->text();
  if (word.isEmpty())
    return;

  m_url = QString(m_config.format()).arg(m_lang, word);
  _lookup();
}

void MainWindow::preference() {}

void MainWindow::setBackground() {
  QColor color =
      QColorDialog::getColor(background, this, "Choose Background Color");
  if (color.isValid()) {
    background = color;
    QString css = QString("background : %1").arg(color.name());
    ui->textEdit->setStyleSheet(css);
  }
}

void MainWindow::openBotudien() {
  const QUrl botudien("https://botudien.pythonanywhere.com/");
  QDesktopServices::openUrl(botudien);
}

QString MainWindow::lang() const { return m_lang; }

QColor MainWindow::getBackground() const { return background; }

void MainWindow::_lookup() {
  ui->textEdit->clear();
  const QUrl API_ENDPOINT(m_url);
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
    if (array.size() <= 0) {
      ui->textEdit->append("Not found");
    }
    m_data_buffer->clear();
  }
}

void MainWindow::setLang() {
  m_lang = "en";
  if (!ui->radioButton_en->isChecked()) {
    m_lang = "vi";
  }
}

void MainWindow::about() {
  AboutDialog *dlg = new AboutDialog(this);
  dlg->exec();
}

void MainWindow::setConfig() {
  ConfigDialog *dlg = new ConfigDialog(this, m_config);
  auto res = dlg->exec();
  if (res == QDialog::Accepted) {
    m_config = dlg->config();
    ui->textEdit->setFont(m_config.font());
  }
}

void MainWindow::redo() { ui->textEdit->redo(); }

void MainWindow::undo() { ui->textEdit->undo(); }

void MainWindow::paste() { ui->textEdit->paste(); }

void MainWindow::copy() { ui->textEdit->copy(); }

void MainWindow::cut() { ui->textEdit->cut(); }
