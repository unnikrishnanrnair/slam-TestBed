#include "tetrahedron.h"
#include "winslam.h"
#include "mainwindow.h"
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <iostream>

Winslam::Winslam(MainWindow *mw)
    : mainWindow(mw)
{
    tetrahedron = new Tetrahedron;

/*
    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();

    connect(xSlider, &QSlider::valueChanged, glWidget, &GLWidget::setXRotation);
    connect(glWidget, &GLWidget::xRotationChanged, xSlider, &QSlider::setValue);
    connect(ySlider, &QSlider::valueChanged, glWidget, &GLWidget::setYRotation);
    connect(glWidget, &GLWidget::yRotationChanged, ySlider, &QSlider::setValue);
    connect(zSlider, &QSlider::valueChanged, glWidget, &GLWidget::setZRotation);
    connect(glWidget, &GLWidget::zRotationChanged, zSlider, &QSlider::setValue);
*/
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;

    container->addWidget(tetrahedron);
/*  container->addWidget(xSlider);
    container->addWidget(ySlider);
    container->addWidget(zSlider);
 */

    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);
    //dockBtn = new QPushButton(tr("Undock"), this);
    //connect(dockBtn, &QPushButton::clicked, this, &Window::dockUndock);
    //mainLayout->addWidget(dockBtn);

    setLayout(mainLayout);

    //xSlider->setValue(15 * 16);
    //ySlider->setValue(345 * 16);
    //zSlider->setValue(0 * 16);

    setWindowTitle(tr("Hello GL"));
}

QSlider *Winslam::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void Winslam::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Winslam::setDataView(Eigen::MatrixXd dataModel){
    tetrahedron->setDataView(dataModel);
}

void Winslam::setContaminatedDataView(Eigen::MatrixXd dataModelContaminated){
    tetrahedron->setContaminatedDataView(dataModelContaminated);
}

void Winslam::setEstimatedDataView(Eigen::MatrixXd dataModelEstimated){
    tetrahedron->setEstimatedDataView(dataModelEstimated);
}

void Winslam::setScala(double X, double Y, double Z){
    std::cout<< "WinSlam.setScala" <<std::endl;
    tetrahedron->setScala(X,Y,Z);
}

void Winslam::setTrasla(double X, double Y, double Z){
    tetrahedron->setTrasla(X,Y,Z);
}
void Winslam::setDots(){
    tetrahedron->setDots();
}

void Winslam::setLines(){
    tetrahedron->setLines();
}
void Winslam::setViewJustEstimated(){
    tetrahedron->setViewJustEstimated();
}
/*void Winslam::dockUndock()
{
    if (parent()) {
        setParent(0);
        setAttribute(Qt::WA_DeleteOnClose);
        move(QApplication::desktop()->width() / 2 - width() / 2,
             QApplication::desktop()->height() / 2 - height() / 2);
        //dockBtn->setText(tr("Dock"));
        show();
    } else {
        if (!mainWindow->centralWidget()) {
            if (mainWindow->isVisible()) {
                setAttribute(Qt::WA_DeleteOnClose, false);
                //dockBtn->setText(tr("Undock"));
                mainWindow->setCentralWidget(this);
            } else {
                QMessageBox::information(0, tr("Cannot dock"), tr("Main window already closed"));
            }
        } else {
            QMessageBox::information(0, tr("Cannot dock"), tr("Main window already occupied"));
        }
    }
}
*/
