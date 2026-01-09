#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QLCDNumber>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort *arduino;
    QLabel *labelCheck;
    QLCDNumber *lcdDistance;

private:
    Ui::MainWindow *ui;
    void readSerial();
    QSerialPort *device;
    double currentAngle;
    double currentDistance;
    float kat = 0.0;
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_searchButton_clicked();
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void paintLine(QPainter *line);
};
#endif // MAINWINDOW_H
