#ifndef PLUGINMANAGER_P_H
#define PLUGINMANAGER_P_H

#include "pluginspec.h"

#include <QString>
#include <QHash>
#include <QStringList>

namespace ExtensionSystem {

class PluginManager;
class PluginCollection;

namespace Internal {

class EXTENSIONSYSTEM_EXPORT PluginManagerPrivate : QObject
{
    Q_OBJECT
public:
    PluginManagerPrivate(PluginManager *pluginManager);
    virtual ~PluginManagerPrivate();

    QHash<QString, PluginCollection *> pluginCategories;
    QList<PluginSpec *> pluginSpecs;
    QStringList pluginPaths;
    QStringList defaultDisabledPlugins; // Plugins/Ignored from install settings
    QStringList defaultEnabledPlugins; // Plugins/ForceEnabled from install settings
    QStringList disabledPlugins;
    QStringList forceEnabledPlugins;
    QString pluginIID;

    // Object pool operations
    void addObject(QObject *obj);
    void removeObject(QObject *obj);

    void setPluginPaths(const QStringList &paths);
    void readPluginPaths();
    void resolveDependencies();
    QList<PluginSpec *> loadQueue();
    bool loadQueue(PluginSpec *spec,
            QList<PluginSpec *> &queue,
            QList<PluginSpec *> &circularityCheckQueue);

private:
    PluginCollection *defaultCollection;
};

} // namespace Internal
} // namespace ExtensionSystem

#endif // PLUGINMANAGER_P_H
