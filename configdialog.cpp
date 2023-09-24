#include "configdialog.h"
#include "ui_configdialog.h"
#include <QColorDialog>
#include <QFontDialog>

ConfigDialog::ConfigDialog(QWidget *parent, const Config &m_config)
    : QDialog(parent), ui(new Ui::ConfigDialog) {
  ui->setupUi(this);
  ui->lineEdit_API->setText(m_config.format());
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &ConfigDialog::saveFormat);
  connect(ui->button_font, &QPushButton::clicked, this, &ConfigDialog::selectFont);
}

ConfigDialog::~ConfigDialog() { delete ui; }

Config ConfigDialog::config() const { return m_config; }

void ConfigDialog::saveFormat() {
  QString format = ui->lineEdit_API->text();
  m_config.setFormat(format);
}

void ConfigDialog::selectFont() {
  bool ok;
  QFont font = QFontDialog::getFont(&ok, m_config.font(), this);
  if (ok) {
      m_config.setFont(font);
  } else {
  }
}
