#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QtMath>
#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint center(width() / 2, height() / 2);
    int maxRadius = 250; // Maksymalny zasięg sonaru

    // Ustawienie koloru linii
    QPen gridPen(Qt::darkGreen, 1, Qt::DashLine); // Linia przerywana
    painter.setPen(gridPen);

    // Rysowanie linii pod kątami
    for (int angle = 0; angle < 360; angle += 30) {
        double angleRad = qDegreesToRadians((double)angle);


        int x2 = center.x() + maxRadius * qCos(angleRad);
        int y2 = center.y() - maxRadius * qSin(angleRad);

        painter.drawLine(center, QPoint(x2, y2));
    }

    // Rysowanie okręgów
    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    for (int r = 50; r <= maxRadius; r += 50) {
        painter.drawEllipse(center, r, r);
    }

    double angleRad = qDegreesToRadians(this->currentAngle);

    double scale = 1.0;
    int targetX = center.x() + (this->currentDistance * scale * qCos(angleRad));
    int targetY = center.y() - (this->currentDistance * scale * qSin(angleRad));


    QPen pointPen(Qt::red, 5);
    pointPen.setCapStyle(Qt::RoundCap);
    painter.setPen(pointPen);

    painter.drawPoint(targetX, targetY);

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

