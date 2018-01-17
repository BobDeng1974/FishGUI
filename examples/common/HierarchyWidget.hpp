#pragma once

#include <FishGUI/ModelView/TreeWidget.hpp>

struct GameObject
{
	std::string name;
	std::vector<GameObject*> children;
	
	GameObject(const std::string& name) : name(name) {}
};


class HierarchyModel : public FishGUI::TTreeModel<GameObject*>
{
	typedef GameObject* T;
public:
	virtual int childCount(T item) const override
	{
		return (int)item->children.size();
	}

	virtual T childAt(int row, int column, T parent) const
	{
		return parent->children[row];
	}

	virtual std::string text(T item) const
	{
		return item->name;
	}
};

class HierarchyWidget : public FishGUI::TreeWidget<GameObject*>
{
public:
	typedef FishGUI::TreeWidget<GameObject*> Super;

	HierarchyWidget(const char* name);
};
