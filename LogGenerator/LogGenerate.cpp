#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>
#include "LogGenerate.h"

LogGenerate::LogGenerate(LogRemoteHelper *aHelper)
    : m_Timer(new QTimer(this))
    , m_RemoteObject(aHelper)
{
    connect(m_Timer, &QTimer::timeout, this, &LogGenerate::onTimeOut);
    m_Timer->setSingleShot(true);
    m_Timer->start(QRandomGenerator::global()->bounded(1, 50));
}

LogGenerate::~LogGenerate()
{
    if (m_Timer->isActive())
    {
        m_Timer->stop();
    }
}

void LogGenerate::onTimeOut()
{
    static long long count = 1;
    if (m_RemoteObject)
    {
        count++;
        m_RemoteObject->setLog(QString("this is a log test fot count: %1").arg(count));
        m_Timer->start(QRandomGenerator::global()->bounded(1, 50));
    }
}
