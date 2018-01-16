#pragma once

#include <set>

struct FontIcon
{
//	int codePoint;	// code point in .ttf
	std::string fontText;	// utf8
	int 		fontSize;
	const char* fontFace;
};

// notice that ItemType must be a pointer type or std::shared_ptr
template<class ItemType, class DataType>
class TTreeModel
{
public:
	ItemType		childAt(ItemType parent, int row) const;
	int 			rowCount(ItemType parent) const;
	bool 			hasChildren(ItemType parent) const { return rowCount(parent) > 0; }
	DataType		data(ItemType item) const;
	const FontIcon*	icon(ItemType item) const { return nullptr; }
//	bool		setData(ItemType* item, const DataType& data);
	
	void SetRootItem(ItemType root)
	{
		m_root = root;
	}
	
protected:
	ItemType m_root;
};

template<class ItemType, class DataType>
class TListModel
{
public:
	ItemType	childAt(ItemType parent, int row) const;
	int		 	rowCount(ItemType parent) const;
	DataType	data(ItemType item) const;
};

