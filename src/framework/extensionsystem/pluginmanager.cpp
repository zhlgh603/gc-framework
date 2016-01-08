#include "pluginmanager.h"
#include "pluginmanager_p.h"
#include "iplugin.h"
#include "pluginspec.h"
#include "pluginspec_p.h"
#include "plugincollection.h"

#include <QDir>
#include <QFile>
#include <QLibrary>

using namespace ExtensionSystem;
using namespace ExtensionSystem::Internal;

static Internal::PluginManagerPrivate *d = 0;
static PluginManager *m_instance=0;

static bool lessThanByPluginName(const PluginSpec *one, const PluginSpec *two)
{
    return one->name() < two->name();
}

/*!
    Gets the unique plugin manager instance.
 */
PluginManager *PluginManager::instance()
{
    return m_instance;
}

/*!
    Creates a plugin manager. Should be done only once per application.
 */
PluginManager::PluginManager()
{
    m_instance = this;
    d = new PluginManagerPrivate(this);
}

PluginManager::~PluginManager()
{
    delete d;
    d = 0;
}

/*!
    Adds the object \a obj to the object pool, so it can be retrieved
    again from the pool by type.

    The plugin manager does not do any memory management - added objects
    must be removed from the pool and deleted manually by whoever is responsible for the object.

    Emits the objectAdded() signal.

    \sa PluginManager::removeObject()
    \sa PluginManager::getObject()
    \sa PluginManager::getObjects()
*/
void PluginManager::addObject(QObject *obj)
{
    d->addObject(obj);
}

/*!
    Emits aboutToRemoveObject() and removes the object \a obj from the object pool.
    \sa PluginManager::addObject()
*/
void PluginManager::removeObject(QObject *obj)
{
    d->removeObject(obj);
}

QList<PluginSpec *> PluginManager::plugins()
{
    return d->pluginSpecs;
}

static inline QString getPlatformName()
{
#if defined(Q_OS_MAC)
    if (QSysInfo::MacintoshVersion >= QSysInfo::MV_10_0) {
        QString result = QLatin1String("OS X");
        result += QLatin1String(" 10.") + QString::number(QSysInfo::MacintoshVersion - QSysInfo::MV_10_0);
        return result;
    } else {
        return QLatin1String("Mac OS");
    }
#elif defined(Q_OS_UNIX)
    QString base;
#  ifdef Q_OS_LINUX
    base = QLatin1String("Linux");
#  else
    base = QLatin1String("Unix");
#  endif // Q_OS_LINUX
    QFile osReleaseFile(QLatin1String("/etc/os-release")); // Newer Linuxes
    if (osReleaseFile.open(QIODevice::ReadOnly)) {
        QString name;
        QString version;
        forever {
            const QByteArray line = osReleaseFile.readLine();
            if (line.isEmpty())
                break;
            if (line.startsWith("NAME=\""))
                name = QString::fromLatin1(line.mid(6, line.size() - 8)).trimmed();
            if (line.startsWith("VERSION_ID=\""))
                version = QString::fromLatin1(line.mid(12, line.size() - 14)).trimmed();
        }
        if (!name.isEmpty()) {
            if (!version.isEmpty())
                name += QLatin1Char(' ') + version;
            return base + QLatin1String(" (") + name + QLatin1Char(')');
        }
    }
    return base;
#elif defined(Q_OS_WIN)
    QString result = QLatin1String("Windows");
    switch (QSysInfo::WindowsVersion) {
    case QSysInfo::WV_XP:
        result += QLatin1String(" XP");
        break;
    case QSysInfo::WV_2003:
        result += QLatin1String(" 2003");
        break;
    case QSysInfo::WV_VISTA:
        result += QLatin1String(" Vista");
        break;
    case QSysInfo::WV_WINDOWS7:
        result += QLatin1String(" 7");
        break;
    default:
        break;
    }
    if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8)
        result += QLatin1String(" 8");
    return result;
#endif // Q_OS_WIN
    return QLatin1String("Unknown");
}

QString PluginManager::platformName()
{
    static const QString result = getPlatformName();
    return result;
}

/*!
    Sets the plugin search paths, i.e. the file system paths where the plugin manager
    looks for plugin descriptions. All given \a paths and their sub directory trees
    are searched for plugin xml description files.

    \sa pluginPaths()
    \sa loadPlugins()
*/
void PluginManager::setPluginPaths(const QStringList &paths)
{
    d->setPluginPaths(paths);
}

/*!
    The IID that valid plugins must have.

    \sa setPluginIID()
*/
QString PluginManager::pluginIID()
{
    return d->pluginIID;
}

/*!
    Sets the IID that valid plugins must have. Only plugins with this IID are loaded, others are
    silently ignored.

    At the moment this must be called before setPluginPaths() is called.
    // ### TODO let this + setPluginPaths read the plugin meta data lazyly whenever loadPlugins() or plugins() is called.
*/
void PluginManager::setPluginIID(const QString &iid)
{
    d->pluginIID = iid;
}

void PluginManager::loadPlugins()
{

}

PluginManagerPrivate::PluginManagerPrivate(ExtensionSystem::PluginManager *pluginManager)
{

}

PluginManagerPrivate::~PluginManagerPrivate()
{

}

void PluginManagerPrivate::addObject(QObject *obj)
{

}

void PluginManagerPrivate::removeObject(QObject *obj)
{

}

void PluginManagerPrivate::setPluginPaths(const QStringList &paths)
{
    pluginPaths = paths;
//    readSettings();
    readPluginPaths();
}

void PluginManagerPrivate::readPluginPaths()
{
    qDeleteAll(pluginCategories);
    qDeleteAll(pluginSpecs);
    pluginSpecs.clear();
    pluginCategories.clear();

    QStringList pluginFiles;
    QStringList searchPaths = pluginPaths;
    while (!searchPaths.isEmpty()) {
        const QDir dir(searchPaths.takeFirst());
        const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        foreach (const QFileInfo &file, files) {
            const QString filePath = file.absoluteFilePath();
            if (QLibrary::isLibrary(filePath))
                pluginFiles.append(filePath);
        }
        const QFileInfoList dirs = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
        foreach (const QFileInfo &subdir, dirs)
            searchPaths << subdir.absoluteFilePath();
    }
    defaultCollection = new PluginCollection(QString());
    pluginCategories.insert(QString(), defaultCollection);

    foreach (const QString &pluginFile, pluginFiles) {
        PluginSpec *spec = new PluginSpec;
        if (!spec->d->read(pluginFile)) { // not a Qt Creator plugin
            delete spec;
            continue;
        }

        PluginCollection *collection = 0;
        // find correct plugin collection or create a new one
        if (pluginCategories.contains(spec->category())) {
            collection = pluginCategories.value(spec->category());
        } else {
            collection = new PluginCollection(spec->category());
            pluginCategories.insert(spec->category(), collection);
        }
        // defaultDisabledPlugins and defaultEnabledPlugins from install settings
        // is used to override the defaults read from the plugin spec
        if (spec->isEnabledByDefault() && defaultDisabledPlugins.contains(spec->name())) {
            spec->d->setEnabledByDefault(false);
            spec->d->setEnabledBySettings(false);
        } else if (!spec->isEnabledByDefault() && defaultEnabledPlugins.contains(spec->name())) {
            spec->d->setEnabledByDefault(true);
            spec->d->setEnabledBySettings(true);
        }
        if (!spec->isEnabledByDefault() && forceEnabledPlugins.contains(spec->name()))
            spec->d->setEnabledBySettings(true);
        if (spec->isEnabledByDefault() && disabledPlugins.contains(spec->name()))
            spec->d->setEnabledBySettings(false);

        collection->addPlugin(spec);
        pluginSpecs.append(spec);
    }
    resolveDependencies();
    // ensure deterministic plugin load order by sorting
    qSort(pluginSpecs.begin(), pluginSpecs.end(), lessThanByPluginName);
//    emit q->pluginsChanged();
}

void PluginManagerPrivate::resolveDependencies()
{
    foreach (PluginSpec *spec, pluginSpecs) {
        spec->d->enabledIndirectly = false; // reset, is recalculated below
        spec->d->resolveDependencies(pluginSpecs);
    }

    QListIterator<PluginSpec *> it(loadQueue());
    it.toBack();
    while (it.hasPrevious()) {
        PluginSpec *spec = it.previous();
        spec->d->enableDependenciesIndirectly();
    }
}

QList<PluginSpec *> PluginManagerPrivate::loadQueue()
{
    QList<PluginSpec *> queue;
    foreach (PluginSpec *spec, pluginSpecs) {
        QList<PluginSpec *> circularityCheckQueue;
        loadQueue(spec, queue, circularityCheckQueue);
    }
    return queue;
}

bool PluginManagerPrivate::loadQueue(PluginSpec *spec, QList<PluginSpec *> &queue,
        QList<PluginSpec *> &circularityCheckQueue)
{
    if (queue.contains(spec))
        return true;
    // check for circular dependencies
    if (circularityCheckQueue.contains(spec)) {
        spec->d->hasError = true;
        spec->d->errorString = PluginManager::tr("Circular dependency detected:");
        spec->d->errorString += QLatin1Char('\n');
        int index = circularityCheckQueue.indexOf(spec);
        for (int i = index; i < circularityCheckQueue.size(); ++i) {
            spec->d->errorString.append(PluginManager::tr("%1(%2) depends on")
                .arg(circularityCheckQueue.at(i)->name()).arg(circularityCheckQueue.at(i)->version()));
            spec->d->errorString += QLatin1Char('\n');
        }
        spec->d->errorString.append(PluginManager::tr("%1(%2)").arg(spec->name()).arg(spec->version()));
        return false;
    }
    circularityCheckQueue.append(spec);
    // check if we have the dependencies
    if (spec->state() == PluginSpec::Invalid || spec->state() == PluginSpec::Read) {
        queue.append(spec);
        return false;
    }

    // add dependencies
    QHashIterator<PluginDependency, PluginSpec *> it(spec->dependencySpecs());
    while (it.hasNext()) {
        it.next();
        // Skip test dependencies since they are not real dependencies but just force-loaded
        // plugins when running tests
        if (it.key().type == PluginDependency::Test)
            continue;
        PluginSpec *depSpec = it.value();
        if (!loadQueue(depSpec, queue, circularityCheckQueue)) {
            spec->d->hasError = true;
            spec->d->errorString =
                PluginManager::tr("Cannot load plugin because dependency failed to load: %1(%2)\nReason: %3")
                    .arg(depSpec->name()).arg(depSpec->version()).arg(depSpec->errorString());
            return false;
        }
    }
    // add self
    queue.append(spec);
    return true;
}
