#ifndef LOGGENERATE_H
#define LOGGENERATE_H

#include "LogRemoteHelper.h"

class LogGenerate: public QObject
{
    Q_OBJECT
public:
    LogGenerate(LogRemoteHelper *aHelper);
    virtual ~LogGenerate();

private slots:
    void onTimeOut();

private:
    QTimer                  *m_Timer;
    LogRemoteHelper         *m_RemoteObject;
};

#endif // LOGGENERATE_H
