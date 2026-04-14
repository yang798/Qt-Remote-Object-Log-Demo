#include <QDateTime>
#include "LogRemoteHelper.h"
#include "QCoreApplication"

LogRemoteHelper::LogRemoteHelper(QObject *parent)
    : myLogSimpleSource(parent)
    , m_Timer(new QTimer(this))
    , m_Process(new QProcess(this))
{
    initParam();
    m_Timer->setInterval(50);
    connect(m_Timer, &QTimer::timeout, this, &LogRemoteHelper::onTimerOut);
    //connect(m_Process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &LogRemoteHelper::onProcessFinished);
    m_Timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_Timer->start();
    m_Process->startDetached("LogDisplay");
}

LogRemoteHelper::~LogRemoteHelper()
{
    if (m_Timer->isActive())
    {
        m_Timer->stop();
    }
}

void LogRemoteHelper::setLog(const QString &aLog)
{
    emit logChanged(aLog);
}

void LogRemoteHelper::exitServer()
{
    QCoreApplication::quit();
}

void LogRemoteHelper::initParam()
{
    setMaxLogCount(2000);
    setMaxFileSize(30);
    setIsWriteToFile(false);
    setLogDir("E:/Log");
    setHeartBeatInterval(50);
}

void LogRemoteHelper::onTimerOut()
{
    qint64 t = QDateTime::currentMSecsSinceEpoch();
    emit heartToClient(t);
}

void LogRemoteHelper::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)

    m_Process->start("LogDisplay");
}
