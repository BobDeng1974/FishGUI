#ifndef RFGUI_hpp
#define RFGUI_hpp

//#define GLEW_STATIC
//#include <gl/glew.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <string>

#include <functional>
#include <list>


namespace RFGUI {
    struct Vector2 {
		float x = 0;
		float y = 0;
    };
	
	struct Vector3 {
		float x, y, z;
	};
	
	struct Vector4 {
		float x, y, z, w;
	};
	
	struct Rect {
//		int x, y, z, w;
		int x, y, width, height;
	};
	
	enum class CursorType {
		Arrow = 0,
		IBeam,
		Crosshair,
		Hand,
		HResize,
		VResize,
		// not a cursor
		CursorCount
	};
    
    struct RenderContext
	{
        GLFWwindow* window = nullptr;
        int width;
        int height;
		
		// stop all rendering if isAllWindowInvisiable == true
		bool isAllWindowInvisiable = false;
    };
	
    static RenderContext g_renderContext;
	
    enum class TabPosition
	{
        Left,
        Right,
        Top,
        Bottom,
        Floating,
    };
	
	struct Window;
	
	struct Tab
	{
		std::string		m_title;
		TabPosition		m_position;
//		GLFWwindow*		m_window = nullptr;
		Window*			m_window;
		int				m_size = 1;
		int				m_minimalSize = 128;
		bool			m_resizing = false;
		bool			m_moving = false;
		std::function<void(void)> m_renderFunction;
		
		Tab(const std::string& title, TabPosition pos)
			: m_title(title), m_position(pos) {
			
		}
	};
    
	enum class GUIAlignment {
        HoriontallyLeft     = 1 << 0,
        HoriontallyCenter   = 1 << 1,
        HoriontallyRight    = 1 << 2,
        VerticallyTop       = 1 << 3,
        VerticallyCenter    = 1 << 4,
        VerticallyBottom    = 1 << 5,
        Center = HoriontallyCenter | VerticallyCenter
    };
    
    void Init(GLFWwindow * window);
	
	void MakeCurrent(GLFWwindow * window);
	
	Tab* CreateTab(const char* title, TabPosition tabPosition, int widthOrHeight);
    
    void Begin();
    void End();
	
	void RenderWindows();
	
	void BeginTab(Tab * tab);
	void EndTab();
    bool Button(const char* text);
	void Label(const char* text, GUIAlignment alignment = GUIAlignment::Center);
	bool CheckBox(const char * label, bool* v);
	bool InputText(const char* label, std::string& text);
	bool Slider(const char* label, float *value, float minValue, float maxValue);
}

#endif // RFGUI_hpp
