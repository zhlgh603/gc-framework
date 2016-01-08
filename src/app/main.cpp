#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

using namespace ExtensionSystem;

const char corePluginNameC[] = "CoreModule";

typedef QList<PluginSpec *> PluginSpecSet;

static inline QStringList getPluginPaths()
{
    QStringList rc;
    // Figure out root:  Up one from 'bin'
    QDir rootDir = QApplication::applicationDirPath();
    rootDir.cdUp();
    const QString rootDirPath = rootDir.canonicalPath();
    QString pluginPath;
    pluginPath = rootDirPath;
    pluginPath += QLatin1String("/plugins");
    rc.push_back(pluginPath);

    return rc;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PluginManager pluginManager;
    PluginManager::setPluginIID(QLatin1String("www.contron.com.cn.GCFPlugin"));

    // Load
    const QStringList pluginPaths = getPluginPaths();
    pluginManager.setPluginPaths(pluginPaths);
    const PluginSpecSet plugins = pluginManager.plugins();
    PluginSpec *coreplugin = 0;
    foreach (PluginSpec *spec, plugins) {
        if (spec->name() == QLatin1String(corePluginNameC)) {
            coreplugin = spec;
            break;
        }
    }
    if (!coreplugin) {
//        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
//        const QString reason = QCoreApplication::translate("Application", "Could not find Core plugin in %1").arg(nativePaths);
//        displayError(msgCoreLoadFailure(reason));
        return 1;
    }

    pluginManager.loadPlugins();
//    if (coreplugin->hasError()) {
//        displayError(msgCoreLoadFailure(coreplugin->errorString()));
//        return 1;
//    }

//    MainWindow w;
//    w.show();

    return a.exec();
}
