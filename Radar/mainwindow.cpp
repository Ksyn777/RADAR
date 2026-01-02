#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        int y2 = center.y() - maxRadius * qSin(angleRad); // Minus, bo Y rośnie w dół

        painter.drawLine(center, QPoint(x2, y2));
    }

    // Rysowanie okręgów
    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    for (int r = 50; r <= maxRadius; r += 50) {
        painter.drawEllipse(center, r, r);
    }
}
