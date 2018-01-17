#pragma once

#include <cassert>
#include <vector>
#include <set>
#include <algorithm>

#include <FishGUI/Widget.hpp>
#include <FishGUI/Input.hpp>

#include <GLFW/glfw3.h>

enum class SelectionType
{
	None,
	Single,
	Multi,
};

template<class T>
class TSelectionModel
{
public:
	TSelectionModel() = default;
	TSelectionModel(TSelectionModel&) = delete;
	TSelectionModel& operator=(TSelectionModel&) = delete;

	bool IsSelected(T item)
	{
		bool selected = (m_selected.find(item) != m_selected.end());
		return selected;
	}
	
	void Select(T item)
	{
		m_selected.insert(item);
		m_lastSelected = item;
		m_rangeSelectionEnd = nullptr;
		SelectionShanged();
	}

	void OnKeyEvent(FishGUI::KeyEvent* e)
	{
		if (e == nullptr || e->isAccepted())
			return;

		if (m_selected.empty())
			return;

		if (e->type() != FishGUI::KeyEvent::Type::KeyPress)
		{
			return;
		}

		// first selection
		T first = nullptr;
		int idx = 0;
		for (auto w : m_visibleItems)
		{
			if (IsSelected(w))
			{
				first = w;
				break;
			}
			idx++;
		}
		
		auto key = e->key();
		auto mod = e->modifiers();

		int offset = 0;
		if (m_itemsPerRow == 1)		// 1D
		{
			if (key == GLFW_KEY_UP)
				offset = -1;
			else if (key == GLFW_KEY_DOWN)
				offset = 1;
		}
		else	// 2D
		{
			if (key == GLFW_KEY_LEFT)
				offset = -1;
			else if (key == GLFW_KEY_RIGHT)
				offset = 1;
			else if (key == GLFW_KEY_UP)
				offset = -m_itemsPerRow;
			else if (key == GLFW_KEY_DOWN)
				offset = m_itemsPerRow;
		}

		idx += offset;
		if (idx >= 0 && idx < m_visibleItems.size())
		{
			//if (idx >= m_visibleItems.size())
			//	idx = m_visibleItems.size() - 1;
			auto rect = m_visibleItemRects[idx];
			ClearSelections();
			Select(m_visibleItems[idx]);
		}
	}
	
	void OnItemClicked(T item, FishGUI::MouseEvent* e)
	{
		assert(e != nullptr);
		auto it = m_selected.find(item);
		bool selected = (it != m_selected.end());
		bool isMulti = (m_type == SelectionType::Multi);
		int mods = e->modifiers();
#if __APPLE__
		constexpr int MODIFIER = int(FishGUI::Modifier::Super);
#else
		constexpr int MODIFIER = int(FishGUI::Modifier::Ctrl);
#endif
		bool appendMode = isMulti && (e->modifiers() & MODIFIER) != 0;
		bool rangeMode = isMulti &&
			(e->modifiers() & int(FishGUI::Modifier::Shift)) != 0 &&
			m_lastSelected != nullptr;
		
		if (appendMode)
		{
			if (selected)
				m_selected.erase(it);
			else
				Select(item);
//			selected = !selected;
		}
		else if (rangeMode)
		{
			m_rangeSelectionBegin = m_lastSelected;
			m_rangeSelectionEnd = item;
		}
		else	// single selection
		{
			ClearSelections();
			Select(item);
//			selected = true;
		}
		e->Accept();
	}
	
	void BeforeFrame()
	{
		m_visibleItems.clear();
		m_visibleItemRects.clear();
	}
	
	void AfterFrame(FishGUI::MouseEvent* e)
	{
		if (e == nullptr)
			return;
		
//		if (e->type()==FishGUI::MouseEvent::Type::MouseButtonPress)
//		{
//
//		}
		
		if (e->type() == FishGUI::MouseEvent::Type::MouseButtonPress)
		{
			if (!e->isAccepted())
			{
				ClearSelections();
				return;
			}
		
			if (m_type == SelectionType::Multi && m_rangeSelectionEnd != nullptr)
			{
	//			m_lastSelected = m_rangeSelectionBegin;
				auto it1 = std::find(m_visibleItems.begin(), m_visibleItems.end(), m_rangeSelectionBegin);
				auto it2 = std::find(m_visibleItems.begin(), m_visibleItems.end(), m_rangeSelectionEnd);
				
				if (it1 == m_visibleItems.end())
				{
				}
				else if (it2 == m_visibleItems.end())
				{
				}
				else
				{
					ClearSelections();
					if (it1 > it2)
						std::swap(it1, it2);
					it2++;
					for (auto it = it1; it != it2; ++it)
					{
						m_selected.insert(*it);
					}
					m_lastSelected = m_rangeSelectionBegin = *it1;
				}
				m_rangeSelectionEnd = nullptr;
			}
		}
	}
	
	void ClearSelections()
	{
		m_selected.clear();
		m_lastSelected = m_rangeSelectionBegin = m_rangeSelectionEnd = nullptr;
	}
	
	void SetSelectionType(SelectionType type)
	{
		m_type = type;
	}
	
	void AppendVisibleItem(T item, const FishGUI::Rect& r)
	{
		m_visibleItems.push_back(item);
		m_visibleItemRects.push_back(r);
	}
	
	//	typedef std::function<void(const std::vector<T*>&)> SelectionChangedCallback;
	//	void OnSelectionChanged();
	
	typedef std::function<void(T)> SelectionChangedCallback;
	void SetSelectionChangedCallback(SelectionChangedCallback callback)
	{
		m_onSelectionChanged = callback;
	}
	
	void SetItemsPerRow(int count)
	{
		assert(count > 0);
		m_itemsPerRow = count;
	}

private:
	void SelectionShanged()
	{
		if (m_onSelectionChanged != nullptr)
			m_onSelectionChanged(m_lastSelected);
	}
	
	SelectionChangedCallback m_onSelectionChanged;
	
private:
	SelectionType m_type = SelectionType::Single;
	int m_itemsPerRow = 1;	// 1D
	
	std::set<T> m_selected;
	// logically visible, not visually
	std::vector<T> m_visibleItems;
	std::vector<FishGUI::Rect> m_visibleItemRects;
	
	// for range selection
	T m_lastSelected = nullptr;
	T m_rangeSelectionBegin = nullptr;
	T m_rangeSelectionEnd = nullptr;
};
