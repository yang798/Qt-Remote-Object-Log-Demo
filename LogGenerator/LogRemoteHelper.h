#ifndef LOGREMOTEHELPER_H
#define LOGREMOTEHELPER_H

#include <QTimer>
#include <QProcess>
#include "rep_myLog_source.h"

class LogRemoteHelper: public myLogSimpleSource
{
    Q_OBJECT

public:
    LogRemoteHelper(QObject *parent = nullptr);
    virtual ~LogRemoteHelper();

public:
    void setLog(const QString &aLog);
    void exitServer() override;

private:
    void initParam();

private slots:
    void onTimerOut();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QTimer          *m_Timer;
    QProcess        *m_Process;
};

#endif // LOGREMOTEHELPER_H
