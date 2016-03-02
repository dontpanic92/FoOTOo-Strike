#ifndef __AGE_PROPERTYSCENENODE_HEADER__
#define __AGE_PROPERTYSCENENODE_HEADER__

#include <SceneNode.h>
#include <vector>
#include <qobject.h>
#include <qtpropertybrowser\qtvariantproperty.h>
#include "Property.h"

class RenderWidget;
class PropertySceneNode : public QObject
{
	Q_OBJECT
public:
	PropertySceneNode(RenderWidget* renderWidget, AGE::SceneNode* node);
	QString GetShowName() { return mShowName; }

	void AddProperties(QtAbstractPropertyBrowser* widget, QtVariantPropertyManager* manager);

protected:
	const QString mShowName = "SceneNode";
	AGE::SceneNode* mSceneNode;
	std::vector<Property> mProperties;
	RenderWidget* mWidget;
};

#endif
