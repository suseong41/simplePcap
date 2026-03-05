#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./mac.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isRunning(false)
{
    ui->setupUi(this);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onStartButton);

    lenModel = new QStringListModel(this);
    ui->lenView->setModel(lenModel);

    typeModel = new QStringListModel(this);
    ui->typeView->setModel(typeModel);

    saModel = new QStringListModel(this);
    ui->saView->setModel(saModel);

    daModel = new QStringListModel(this);
    ui->daView->setModel(daModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartButton()
{
    if(isRunning == false)
    {
        isRunning = true;
        ui->runBtn->setText("Stop");

        devType = ui->devIn->text().toStdString();
    }
    else
    {
        isRunning = false;
        ui->runBtn->setText("Start");
    }
}
