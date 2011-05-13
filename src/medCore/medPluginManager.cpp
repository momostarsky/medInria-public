/* medPluginManager.cpp ---
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Oct 28 18:09:54 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Mon Feb 22 21:28:17 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 52
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "medPluginManager.h"

#include <QtCore>

#include <dtkCore/dtkPluginManager.h>
#include <dtkCore/dtkPlugin.h>


class medPluginManagerPrivate
{
public:
    QHash<QString, QStringList> handlers;
};

medPluginManager *medPluginManager::instance(void)
{
    if(!s_instance)
        s_instance = new medPluginManager;
    return s_instance;
}

void medPluginManager::uninitialize()
{
    //do nothing, setting the path only brings about problems when the dtkSettingsEditor is used.
}

void medPluginManager::readSettings(void)
{
    QSettings settings;
    // qSettings should use what is defined in the application (organization and appName)

    settings.beginGroup("plugins");
    QDir plugins_dir = qApp->applicationDirPath() + "/../plugins";
    qDebug()<<"plugins default path:"<<plugins_dir.absolutePath();
    
    setPath (settings.value("path", plugins_dir.absolutePath()).toString());

    
    if (!settings.contains("path"))
    {
        qDebug()<<"fill in empty path in settings";
        settings.setValue("path", plugins_dir.absolutePath());
    }

    settings.endGroup();

    const char PLUGIN_PATH_VAR_NAME[] = "MEDINRIA_PLUGIN_PATH";
    QByteArray pluginVarArray = qgetenv ( PLUGIN_PATH_VAR_NAME );
    if ( !pluginVarArray.isEmpty() ) {
        setPath( QString(pluginVarArray.constData()));
    }

    if(path().isEmpty()) {
        qWarning() << "Your dtk config does not seem to be set correctly.";
        qWarning() << "Please set plugins.path.";
    }
}

void medPluginManager::writeSettings(void)
{

    QSettings settings;
    settings.beginGroup("plugins");
    settings.setValue("path", path());
    settings.endGroup();
}


QStringList medPluginManager::handlers(const QString& category)
{
    if (d->handlers.contains(category))
        return d->handlers.value(category);

    return QStringList();
}

void medPluginManager::onPluginLoaded(const QString& name)
{
    dtkPlugin *plug = plugin(name);

    QStringList categories;

    if (plug->hasMetaData("category"))
        categories = plug->metaDataValues("category");

    foreach(QString category, categories)
        d->handlers[category] << plug->types();
}

medPluginManager::medPluginManager(void) : dtkPluginManager(), d(new medPluginManagerPrivate)
{
    connect(this, SIGNAL(loaded(const QString&)), this, SLOT(onPluginLoaded(const QString&)));
}

medPluginManager::~medPluginManager(void)
{
    delete d;

    d = NULL;
}

medPluginManager *medPluginManager::s_instance = NULL;
