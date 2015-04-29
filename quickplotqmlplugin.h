#ifndef QUICKPLOTQMLPLUGIN_H
#define QUICKPLOTQMLPLUGIN_H

#include <QQmlExtensionPlugin>

class QuickPlotQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QUICKPLOTQMLPLUGIN_H
