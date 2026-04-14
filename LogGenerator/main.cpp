#include <QCoreApplication>
#include <QRemoteObjectHost>
#include <QUrl>
#include "LogRemoteHelper.h"
#include "LogDef.h"
#include "LogGenerate.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LogRemoteHelper logHelper;
    QRemoteObjectHost host(QUrl(REMOTE_URL));
    host.enableRemoting(&logHelper, SERVICE_NAME);
    LogGenerate aGenerate(&logHelper);

    return a.exec();
}
