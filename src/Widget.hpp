#pragma once
#include <string>
#include <Vector.hpp>

#include <vector>

#include <cassert>

namespace FishGUI
{
	struct FishGUIContext;
	class Layout;

	enum class Alignment : uint8_t
	{
		Minimum = 0,	// Take only as much space as is required.
		Middle,			// Center align.
		Maximum,		// Take as much space as is allowed.
		Fill			// Fill according to preferred sizes.
	};

	enum class Orientation
	{
		Horizontal = 0,
		Vertical,
	};

	class Widget
	{
	public:
		Widget(FishGUIContext* context, const char* name = "") : m_context(context), m_name(name)
		{

		}

		const std::string& GetName() const { return m_name; }

		int	GetWidth() const { return m_rect.width; }
		int	GetHeight() const { return m_rect.height; }
		void SetWidth(int w) { m_rect.width = w; }
		void SetHeight(int h) { m_rect.height = h; }

		void SetMinSize(int w, int h) { m_minSize.width = w; m_minSize.height = h; }

		Layout* GetLayout() { return m_layout; }
		void SetLayout(Layout* layout);

		void SetRect(int x, int y, int w, int h)
		{
			m_rect.x = x;
			m_rect.y = y;
			m_rect.width = w;
			m_rect.height = h;
		}

		//Alignment	GetAlignment() { return m_alignment; }
		//void		SetAlignment(Alignment alignment) { m_alignment = alignment; }

		virtual void Draw();

		//virtual void AddChild(int index, Widget * widget);
		//void AddChild(Widget * child) { m_children.push_back(child); }

		virtual bool MouseButtonEvent(const Vector2i & p, int button, bool down, int modifiers)
		{
			return false;
		}

		virtual bool MouseMotionEvent(const Vector2i & p, const Vector2i & rel, int button, int modifiers)
		{
			return false;
		}

		// virtual bool MouseDragEvent(const Vector2i & p, const Vector2i & rel, int button, int modifiers)
		virtual bool MouseDragEvent()
		{
			return false;
		}

	protected:
		friend class SplitLayout;
		friend class HLayout;
		friend class VLayout;

		FishGUIContext* 	m_context = nullptr;
		//Widget*				m_parent = nullptr;
		Layout * 			m_layout = nullptr;
		std::string 		m_name;
		Rect 				m_rect = {0, 0, 1, 1};
		Size				m_fixedSize = { -1, -1 };
		Size				m_minSize = { 1, 1 };
		Size				m_maxSize = { 4096, 4096 };
		//Alignment			m_alignment = Alignment::Fill;
		//std::vector<Widget *> m_children;
	};

	class Layout
	{
	public:
		virtual void PerformLayout(const Rect& rect) = 0;
	};

	class Splitter : public Widget
	{
	public:
		static constexpr int Width = 3;

		Splitter(FishGUIContext* context, Orientation orientation)
			: Widget(context), m_orientation(orientation)
		{

		}

		virtual bool MouseDragEvent() override;

	private:
		friend class Layout;
		friend class SplitLayout;
		Orientation m_orientation;
		bool m_dirty = false;
		int m_pos = 0;
		//int m_min = 100;
		//int m_max = 700;
	};

	class SplitLayout : public Layout
	{
	public:
		SplitLayout(FishGUIContext* context, Orientation orientation)
			: m_orientation(orientation), m_splitter(context, orientation)
		{
		}

		void PerformLayout(const Rect& rect);

		static constexpr int INTERVAL = Splitter::Width;

		//void AddWidget(Widget * widget)
		//{
		//	m_widget->AddChild(widget);
		//}

		enum class PartType
		{
			EWidget,
			ELayout,
		};

		struct Part
		{
			union
			{
				Widget * w;
				SplitLayout * l;
			};
			PartType type;
			//int minSize = 1;
			//int maxSize = 4096;

			Size GetMinSize()
			{
				if (type == PartType::ELayout)
					return l->m_minSize;
				else
					return w->m_minSize;
			}
		};

		void Split(Widget* w1, Widget* w2)
		{
			assert(!m_done);
			part1.w = w1;
			part2.w = w2;
			part1.type = PartType::EWidget;
			part2.type = PartType::EWidget;
			m_done = true;
		}

		void Split(Widget* w, SplitLayout* l)
		{
			assert(!m_done);
			part1.w = w;
			part2.l = l;
			part1.type = PartType::EWidget;
			part2.type = PartType::ELayout;
			m_done = true;
		}

		void Split(SplitLayout* l, Widget* w)
		{
			assert(!m_done);
			part1.l = l;
			part2.w = w;
			part1.type = PartType::ELayout;
			part2.type = PartType::EWidget;
			m_done = true;
		}

		void Split(SplitLayout* l1, SplitLayout* l2)
		{
			assert(!m_done);
			part1.l = l1;
			part2.l = l2;
			part1.type = PartType::ELayout;
			part2.type = PartType::ELayout;
			m_done = true;
		}

	protected:
		friend class Widget;
		Orientation m_orientation;
		Splitter m_splitter;
		bool m_done = false;

		Size m_minSize = {1, 1};

		Part part1;
		Part part2;
	};

	class ToolBar : public Widget
	{
	public:
		ToolBar(FishGUIContext * context) : Widget(context, "ToolBar")
		{
			m_rect.height = 32;
		}

		virtual void Draw() override;
	};

	class StatusBar : public Widget
	{
	public:
		StatusBar(FishGUIContext * context) : Widget(context, "StatusBar")
		{
			m_rect.height = 20;
		}

		virtual void Draw() override;
	};


	class TabWidget : public Widget
	{
	public:
		int m_activeTabId = 0;

		TabWidget(FishGUIContext* context, const char* name) : Widget(context, name)
		{
		}
		
		void AddChild(Widget* w)
		{
			m_children.push_back(w);
		}

		virtual void Draw() override;

	protected:
		std::vector<Widget*> m_children;
	};

	class IMWidget : public Widget
	{
	public:
		IMWidget(FishGUIContext* context, const char* name) : Widget(context, name)
		{
		}
	};

}