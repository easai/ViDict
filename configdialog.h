#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include "config.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr, const Config &m_config = Config());
    ~ConfigDialog();

    Config config() const;

private slots:
    void saveFormat();
    void selectFont();

private:
    Ui::ConfigDialog *ui;
    Config m_config;

};

#endif // CONFIGDIALOG_H
