#include "QtDesignderShared.h"
#include "QtDesignderSharedPlugin.h"

#include <QtCore/QtPlugin>

QtDesignderSharedPlugin::QtDesignderSharedPlugin(QObject *parent)
	: QObject(parent)
{
	initialized = false;
}

void QtDesignderSharedPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
	if (initialized)
		return;

	initialized = true;
}

bool QtDesignderSharedPlugin::isInitialized() const
{
	return initialized;
}

QWidget *QtDesignderSharedPlugin::createWidget(QWidget *parent)
{
	return new QtDesignderShared(parent);
}

QString QtDesignderSharedPlugin::name() const
{
	return "QtDesignderShared";
}

QString QtDesignderSharedPlugin::group() const
{
	return "My Plugins";
}

QIcon QtDesignderSharedPlugin::icon() const
{
	return QIcon();
}

QString QtDesignderSharedPlugin::toolTip() const
{
	return QString();
}

QString QtDesignderSharedPlugin::whatsThis() const
{
	return QString();
}

bool QtDesignderSharedPlugin::isContainer() const
{
	return false;
}

QString QtDesignderSharedPlugin::domXml() const
{
	return "<widget class=\"QtDesignderShared\" name=\"qtDesignderShared\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString QtDesignderSharedPlugin::includeFile() const
{
	return "QtDesignderShared.h";
}
