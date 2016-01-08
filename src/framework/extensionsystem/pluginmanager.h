#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "extensionsystem_global.h"

#include <QObject>

namespace ExtensionSystem {

class PluginCollection;
class IPlugin;
class PluginSpec;

namespace Internal { class PluginManagerPrivate; }

class EXTENSIONSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager *instance();

    explicit PluginManager();
    ~PluginManager();

    // Object pool operations
    static void addObject(QObject *obj);
    static void removeObject(QObject *obj);

public:
    static void setPluginPaths(const QStringList &paths);
    static QList<PluginSpec *> plugins();
    static QString platformName();
    static QString pluginIID();
    static void setPluginIID(const QString &iid);

    void loadPlugins();

signals:

public slots:
};


} // namespace ExtensionSystem

#endif // PLUGINMANAGER_H
