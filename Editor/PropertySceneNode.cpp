#include <SceneNode.h>
#include "RenderWidget.h"
#include "PropertySceneNode.h"

PropertySceneNode::PropertySceneNode(RenderWidget* renderWidget, AGE::SceneNode* node)
	:mSceneNode(node), mWidget(renderWidget)
{
	std::function<QVariant(int)> positionGet = [this](int index){
		return mSceneNode->GetTransform()->GetPosition()[index];
	};

	std::function<bool(int, const QVariant&)> positionSet = [this](int index, const QVariant& s){
		bool ok = false;
		float v = s.toFloat(&ok);
		if (!ok)
			return false;

		AGE::Vector3 position = mSceneNode->GetTransform()->GetPosition();
		position[index] = v;
		mSceneNode->GetTransform()->SetPosition(position);
		return true;
	};

	using namespace std::placeholders;

	mProperties.push_back(Property("Relative X", QVariant::Type::Double,
		std::bind(positionGet, 0), std::bind(positionSet, 0, _1)));
	mProperties.push_back(Property("Relative Y", QVariant::Type::Double,
		std::bind(positionGet, 1), std::bind(positionSet, 1, _1)));
	mProperties.push_back(Property("Relative Z", QVariant::Type::Double,
		std::bind(positionGet, 2), std::bind(positionSet, 2, _1)));
}

void PropertySceneNode::AddProperties(QtAbstractPropertyBrowser* widget, QtVariantPropertyManager* manager)
{
	QtProperty *topItem = manager->addProperty(QtVariantPropertyManager::groupTypeId(), mShowName);
	std::map<QtProperty*, Property*> pmap;

	for (auto& v : mProperties) {
		QtVariantProperty* item = manager->addProperty(v.GetType(), v.GetName());
		item->setValue(v.Get());
		topItem->addSubProperty(item);

		pmap.insert(std::make_pair(item, &v));
	}

	widget->addProperty(topItem);

	connect(manager, &QtVariantPropertyManager::valueChanged,
		[pmap, this](QtProperty* property, const QVariant& val){
		auto f = pmap.find(property);
		if (f == pmap.end())
			return;
		f->second->Set(val);
		mWidget->update();
	});
}
