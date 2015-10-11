#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QThread thread;
    QThread thread2;
    QThread thread3;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    float range;



private slots:

    void on_vehButton_clicked();

    void on_parseOutputButton_clicked();

    void on_rangeSlider_valueChanged(int value);

    void on_clearButton_clicked();

    void finishWriting();
    void handleParsingResults();
    void finishGenerating();

    void on_outputButton_clicked();

    void on_areaSettingsButton_clicked();

    void on_pedGenButton_clicked();

    void on_resultsButton_clicked();

    void checkIfReadyToCalculate();


private:


    QString fileName;
    QString outputFileName;



    void updateSimInfo(int timesteps, int pedestrians);
    Ui::MainWindow *ui;
    void clearAll();

};

#endif // MAINWINDOW_H
