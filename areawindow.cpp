#include "areawindow.h"
#include "ui_areawindow.h"

AreaWindow::AreaWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AreaWindow)
{
    ui->setupUi(this);
}

AreaWindow::~AreaWindow()
{
    delete ui;
}


void AreaWindow::on_buttonBox_accepted()
{
    lon2 = ui->lon2LineEdit->text();
    lat2 = ui->lat2LineEdit->text();
    lat = ui->latLineEdit->text();
    lon = ui->lonLineEdit->text();
}
