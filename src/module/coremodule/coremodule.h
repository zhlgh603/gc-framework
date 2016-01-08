#ifndef COREMODULE_H
#define COREMODULE_H

#include <extensionsystem/iplugin.h>

class CoreModule: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "www.contron.com.cn.GCFPlugin" FILE "CoreModule.json")
public:
    CoreModule();
    ~CoreModule();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
};

#endif // COREMODULE_H
