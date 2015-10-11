#ifndef PEDESTRIANWINDOW_H
#define PEDESTRIANWINDOW_H

#include <QDialog>

namespace Ui {
class PedestrianWindow;
}

class PedestrianWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PedestrianWindow(QWidget *parent = 0);
    ~PedestrianWindow();
    int distance;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PedestrianWindow *ui;
};

#endif // PEDESTRIANWINDOW_H
