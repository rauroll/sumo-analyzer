#include "pedestrianwindow.h"
#include "ui_pedestrianwindow.h"

PedestrianWindow::PedestrianWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PedestrianWindow)
{
    ui->setupUi(this);
    ui->distanceSlider->setRange(1, 1000);
    ui->distanceBox->setRange(1, 1000);
    QObject::connect(ui->distanceSlider, SIGNAL(valueChanged(int)), ui->distanceBox, SLOT(setValue(int)));
    QObject::connect(ui->distanceBox, SIGNAL(valueChanged(int)), ui->distanceSlider, SLOT(setValue(int)));
    ui->distanceBox->setValue(100);
}

PedestrianWindow::~PedestrianWindow()
{
    delete ui;
}

void PedestrianWindow::on_buttonBox_accepted()
{
    distance = ui->distanceBox->value();
}
