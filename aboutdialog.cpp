#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->textBrowser->setOpenExternalLinks(true);
    QPixmap pix(":/images/botudien-qr.png");
    ui->label_barcode->setPixmap(pix.scaled(150,150));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
