#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LogDef.h"
#include "qurl.h"
#include "LogClient.h"

#define TIMEOUT 200

LogClient::LogClient(MainWindow *aMainWindow)
    : QObject(nullptr)
    , m_HeartInterval(50)
    , m_IsWriteToFile(false)
    , m_MaxLogCount(2000)
    , m_MaxFileSize(30)
    , m_LogDir("E:/Log")
    , m_HeartCounter(0)
    , m_Timer(new QTimer())
    , m_thread(new QThread())
    , m_MainWindow(aMainWindow)
{
    moveToThread(m_thread);
    m_Node.moveToThread(m_thread);
    m_Timer->moveToThread(m_thread);
    connect(m_Timer, &QTimer::timeout, this, &LogClient::onTimeOut);
    connect(m_thread, &QThread::started, this, &LogClient::connectToHost);
    m_Timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_thread->start();
    if (m_MainWindow)
    {
        m_MainWindow->setMaxLogCount(m_MaxLogCount);
    }
}

LogClient::~LogClient()
{
    if (m_Timer->isActive())
    {
        if (QThread::currentThread() == m_thread)
        {
            m_Timer->stop();
        }
        else
        {
            QMetaObject::invokeMethod(m_Timer, "stop", Qt::BlockingQueuedConnection);
        }

        delete m_Timer;
        m_Timer = nullptr;
    }

    m_thread->quit();
    m_thread->wait();

    delete m_thread;
    m_thread = nullptr;
}

void LogClient::connectToHost()
{
    m_Node.connectToNode(QUrl(REMOTE_URL));
    m_Replica.reset(m_Node.acquire<myLogReplica>(SERVICE_NAME));

    if (!m_Replica)
    {
        return;
    }

    connect(m_Replica.get(), &myLogReplica::initialized, this, &LogClient::onInitialized);
    connect(m_Replica.get(), &myLogReplica::logChanged, this, &LogClient::onLogReceived);
    connect(m_Replica.get(), &myLogReplica::heartToClient, this, &LogClient::onHeartBeatReceived);
    connect(m_Replica.get(), &myLogReplica::heartBeatIntervalChanged, this, &LogClient::onHeartbeatIntervalChanged);
    connect(m_Replica.get(), &myLogReplica::logDirChanged, this, &LogClient::onLogDirChaned);
    connect(m_Replica.get(), &myLogReplica::maxFileSizeChanged, this, &LogClient::onMaxFileSizeChanged);
    connect(m_Replica.get(), &myLogReplica::maxLogCountChanged, this, &LogClient::onMaxLogCountChanged);
    connect(m_Replica.get(), &myLogReplica::isWriteToFileChanged, this, &LogClient::onIsWriteToFileChanged);
}

void LogClient::disConnnectFromHost()
{
    m_Timer->stop();
    disconnect(m_Replica.get(), &myLogReplica::initialized, this, &LogClient::onInitialized);
    disconnect(m_Replica.get(), &myLogReplica::logChanged, this, &LogClient::onLogReceived);
    disconnect(m_Replica.get(), &myLogReplica::heartToClient, this, &LogClient::onHeartBeatReceived);
    disconnect(m_Replica.get(), &myLogReplica::heartBeatIntervalChanged, this, &LogClient::onHeartbeatIntervalChanged);
    disconnect(m_Replica.get(), &myLogReplica::logDirChanged, this, &LogClient::onLogDirChaned);
    disconnect(m_Replica.get(), &myLogReplica::maxFileSizeChanged, this, &LogClient::onMaxFileSizeChanged);
    disconnect(m_Replica.get(), &myLogReplica::maxLogCountChanged, this, &LogClient::onMaxLogCountChanged);
    disconnect(m_Replica.get(), &myLogReplica::isWriteToFileChanged, this, &LogClient::onIsWriteToFileChanged);

    m_Replica.reset();
}

void LogClient::writeToFile()
{
    static QString logFileName;

    if (m_LogDir.isEmpty())
    {
        return;
    }

    QDir dir(m_LogDir);
    if (!dir.exists())
    {
        if (!dir.mkdir(m_LogDir))
        {
            return;
        }
    }

    if (logFileName.isEmpty())
    {
        logFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz_");
        logFileName += "log.txt";
    }

    QFile file(logFileName);
    if (file.exists() && file.size() > m_MaxFileSize * 1024 * 1024)
    {
        logFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz_");
        logFileName += "log.txt";
    }

    file.setFileName(logFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream(&file);
        for (const auto & str: m_LogContent)
        {
            stream << str << "\r\n";
        }

        file.close();
    }

    m_LogContent.clear();
}

void LogClient::reConnect()
{
    QMetaObject::invokeMethod(this, "onReconnect", Qt::QueuedConnection);
}

void LogClient::closeServer()
{
    QMetaObject::invokeMethod(this, "onCloseServer", Qt::QueuedConnection);
}

void LogClient::onLogReceived(const QString &aLog)
{
    if (m_IsWriteToFile)
    {
        m_LogContent << aLog;

        if (m_LogContent.size() > m_MaxLogCount)
        {
            writeToFile();
        }
    }

    if (m_MainWindow)
    {
        m_MainWindow->setLog(aLog);
    }
}

void LogClient::onInitialized()
{
    if (!m_Replica)
    {
        return;
    }

    m_HeartInterval = m_Replica->heartBeatInterval();
    m_IsWriteToFile = m_Replica->isWriteToFile();
    m_MaxLogCount = m_Replica->maxLogCount();
    m_MaxFileSize = m_Replica->maxFileSize();
    m_LogDir = m_Replica->logDir();

    if (m_HeartInterval != 0)
    {
        m_Timer->start(m_HeartInterval);
    }

    m_MainWindow->setConnectStatus("已连接");
}

void LogClient::onTimeOut()
{
    m_HeartCounter++;

    if (m_HeartCounter > TIMEOUT / m_HeartInterval)
    {
        disConnnectFromHost();
        m_MainWindow->setConnectStatus("未连接");
        m_Timer->stop();

        if (m_IsWriteToFile && !m_LogContent.isEmpty())
        {
            writeToFile();
        }
    }
    else
    {
        if (m_IsWriteToFile && !m_LogContent.isEmpty())
        {
            writeToFile();
        }
    }
}

void LogClient::onHeartBeatReceived(qint64 aTime)
{
    Q_UNUSED(aTime)
    m_HeartCounter = 0;
}

void LogClient::onHeartbeatIntervalChanged(int aInverval)
{
    m_HeartInterval = aInverval;
    m_Timer->start(m_HeartInterval);
}

void LogClient::onIsWriteToFileChanged(bool aIsWriteToFile)
{
    m_IsWriteToFile = aIsWriteToFile;
}

void LogClient::onMaxLogCountChanged(int aMaxLogCount)
{
    m_MaxLogCount = aMaxLogCount;

    if (m_MainWindow)
    {
        m_MainWindow->setMaxLogCount(aMaxLogCount);
    }
}

void LogClient::onMaxFileSizeChanged(int aMaxFileSize)
{
    m_MaxFileSize = aMaxFileSize;
}

void LogClient::onLogDirChaned(const QString &aLogDir)
{
    m_LogDir = aLogDir;
}

void LogClient::onCloseServer()
{
    if (m_Replica)
    {
        m_Replica->exitServer();
    }
}

void LogClient::onReconnect()
{
    disConnnectFromHost();
    connectToHost();
}
