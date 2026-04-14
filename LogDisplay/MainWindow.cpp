#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LogDef.h"
#include "qurl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client(std::make_unique<LogClient>(this))
{
    ui->setupUi(this);
    ui->plainTextEdit->setReadOnly(true);
    setWindowTitle("Log Displayer");
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->btnReConnect, &QPushButton::clicked, this, &MainWindow::onReconnectClicked);
    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::onCloseServerClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMaxLogCount(int aCount)
{
    QMetaObject::invokeMethod(this, "onSetMaxLogCount", Qt::QueuedConnection, Q_ARG(int, aCount));
}

void MainWindow::setLog(const QString &aLog)
{
    QMetaObject::invokeMethod(this, "onSetLog", Qt::QueuedConnection, Q_ARG(QString, aLog));
}

void MainWindow::setConnectStatus(const QString &aStatus)
{
    QMetaObject::invokeMethod(this, "onSetConnectStatus", Qt::QueuedConnection, Q_ARG(QString, aStatus));
}

void MainWindow::onClearClicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::onReconnectClicked()
{
    m_client->reConnect();
}

void MainWindow::onCloseServerClicked()
{
    m_client->closeServer();
}

void MainWindow::onSetMaxLogCount(int aCount)
{
    ui->plainTextEdit->setMaximumBlockCount(aCount);
}

void MainWindow::onSetLog(const QString &aLog)
{
    ui->plainTextEdit->appendPlainText(aLog);
}

void MainWindow::onSetConnectStatus(const QString &aStatus)
{
    ui->labelConnect->setText(aStatus);
}




