#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QtMath>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLCDNumber>
#include <QTimer>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);
    ui->lcdDistance->setDigitCount(5);
    ui->lcdDistance->display(currentDistance);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this](){
        kat += 5.0; // Prędkość obrotu radaru
        if (kat >= 360) kat = 0;
        update();
    });
    timer->start(30);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);

    painter.translate(width() / 2, height() / 2);

    int maxRadius = 250;

     //Rysowanie siatki
    painter.setPen(QPen(Qt::darkGreen, 1, Qt::DashLine));
    for (int angle = 0; angle < 360; angle += 30) {
        double angleRad = qDegreesToRadians((double)angle);
        int x2 = maxRadius * qCos(angleRad);
        int y2 = -maxRadius * qSin(angleRad);
        painter.drawLine(0, 0, x2, y2);
    }

    // Rysowanie okręgów
    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    for (int r = 50; r <= maxRadius; r += 50) {
        painter.drawEllipse(QPoint(0,0), r, r);
    }

    //  WYWOŁANIE LINII
    paintLine(&painter);

    //Rysowanie punktu celu (obiektu na radarze)
    double angleRadTarget = qDegreesToRadians(this->currentAngle);
    int targetX = this->currentDistance * qCos(angleRadTarget);
    int targetY = -this->currentDistance * qSin(angleRadTarget);

    painter.setPen(QPen(Qt::red, 8, Qt::SolidLine, Qt::RoundCap));
    painter.drawPoint(targetX, targetY);
}

void MainWindow::paintLine(QPainter *painter)
{
    painter->save();

    painter->setPen(QPen(Qt::green, 2));

    painter->rotate(kat);

    painter->drawLine(0, 0, 250, 0);

    painter->restore();
}

void MainWindow::on_searchButton_clicked()
{
    ui->searchBox->clear();
    qDebug() << "Szukam urządzeń";

    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    if(devices.isEmpty())
    {
        qDebug() << "Nie znaleziono żadnych urządzeń.";
        ui->searchBox->addItem("Brak urządzeń");
    }
    else
    {
        for(int i = 0; i < devices.count(); i++) {
            QString portName = devices.at(i).portName();
            QString description = devices.at(i).description();

            qDebug() << portName << description;

            ui->searchBox->addItem(portName + " (" + description + ")");
        }
    }
}




void MainWindow::on_connectButton_clicked()
{
    if(this->device->isOpen()) {
        this->device->close();
        ui->connectButton->setText("Połącz");
        qDebug() << "Zamknięto port.";
        return;
    }

    if(ui->searchBox->count() == 0) {
        qDebug() << "Nie wykryto żadnych urządzeń!";
        return;
    }

    QString portName = ui->searchBox->currentText().split(" ").first();
    this->device->setPortName(portName);

    if(device->open(QSerialPort::ReadWrite)) {
        this->device->setBaudRate(QSerialPort::Baud9600);
        this->device->setDataBits(QSerialPort::Data8);
        this->device->setParity(QSerialPort::NoParity);
        this->device->setStopBits(QSerialPort::OneStop);
        this->device->setFlowControl(QSerialPort::NoFlowControl);

        connect(device, &QSerialPort::readyRead, this, &MainWindow::readSerial);

        qDebug()<<"Otwarto port szeregowy.";
        labelCheck->setText("ON");

    }
    else {
        qDebug()<<"Otwarcie porty szeregowego się nie powiodło!";
    }

}


void MainWindow::on_disconnectButton_clicked()
{
    if(this->device->isOpen()) {
        this->device->close();
        qDebug() << "Zamknięto port.";
        labelCheck->setText("OFF");
        return;
    }
}

void MainWindow::readSerial() {
    while(this->device->canReadLine()) {
        QString line = QString::fromLocal8Bit(this->device->readLine()).trimmed();
        QStringList data = line.split(",");
        if(data.size() == 2) {
            double angle = data[0].toDouble();
            double distance = data[1].toDouble();

            qDebug() << "Kąt:" << angle << "Dystans:" << distance;

            this->currentAngle = angle;
            this->currentDistance = distance;

            update();
        }
    }
}

