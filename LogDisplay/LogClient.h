#ifndef LOGCLIENT_H
#define LOGCLIENT_H

#include <QThread>
#include <QRemoteObjectNode>
#include <memory>
#include <QTimer>
#include "rep_myLog_replica.h"

class MainWindow;
class LogClient: public QObject
{
    Q_OBJECT
public:
    LogClient(MainWindow *aMainWindow);
    virtual ~LogClient();

private:
    void disConnnectFromHost();
    void writeToFile();

public:
    void reConnect();
    void closeServer();

private slots:
    void connectToHost();
    void onLogReceived(const QString &aLog);
    void onInitialized();
    void onTimeOut();
    void onHeartBeatReceived(qint64 aTime);
    void onHeartbeatIntervalChanged(int aInverval);
    void onIsWriteToFileChanged(bool aIsWriteToFile);
    void onMaxLogCountChanged(int aMaxLogCount);
    void onMaxFileSizeChanged(int aMaxFileSize);
    void onLogDirChaned(const QString &aLogDir);
    void onCloseServer();
    void onReconnect();

private:
    QRemoteObjectNode               m_Node;
    std::shared_ptr<myLogReplica>   m_Replica;
    int                             m_HeartInterval;
    bool                            m_IsWriteToFile;
    int                             m_MaxLogCount;
    int                             m_MaxFileSize;
    QString                         m_LogDir;
    int                             m_HeartCounter;
    QTimer                         *m_Timer;
    QStringList                     m_LogContent;
    QThread                        *m_thread;
    MainWindow                     *m_MainWindow;
};

#endif // LOGCLIENT_H
