#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <LogClient.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void setMaxLogCount(int aCount);
    void setLog(const QString &aLog);
    void setConnectStatus(const QString &aStatus);

private slots:
    void onClearClicked();
    void onReconnectClicked();
    void onCloseServerClicked();
    void onSetMaxLogCount(int aCount);
    void onSetLog(const QString &aLog);
    void onSetConnectStatus(const QString &aStatus);

private:
    Ui::MainWindow              *ui;
    std::unique_ptr<LogClient>  m_client;
};
#endif // MAINWINDOW_H
