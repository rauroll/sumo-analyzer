#ifndef AREAWINDOW_H
#define AREAWINDOW_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class AreaWindow;
}

class AreaWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AreaWindow(QWidget *parent = 0);
    ~AreaWindow();

    QString lon2;
    QString lat2;
    QString lat;
    QString lon;

private slots:

    void on_buttonBox_accepted();

private:
    Ui::AreaWindow *ui;
};

#endif // AREAWINDOW_H
