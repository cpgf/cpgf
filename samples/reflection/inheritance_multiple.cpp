#include "reflect_common.h"


namespace inheritance_multiple {

class Border { // not reflected
public:
	virtual int getWidth() const {
		return 1;
	}
};

class Window
{
public:
	static const int ID = 1;

public:
	virtual std::string getName() = 0;

	int getID() {
		return ID;
	}

public:
	int value;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<Window>
		::define("inheritance_multiple::Window")
	
		._method("getName", &Window::getName)
		._method("getID", &Window::getID)
	;
}


class BorderWindow : public virtual Window, public Border
{
public:
	static const int ID = 2;
	static const std::string Name;

public:
	virtual std::string getName() {
		return Name;
	}

	int getID() {
		return ID;
	}

	virtual int born() {
		return 100;
	}

	int borderWidth;

};
const std::string BorderWindow::Name = "border_window";


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<BorderWindow, Window, Border>
		::define("inheritance_multiple::BorderWindow")
	
		._method("getID", &BorderWindow::getID)
		._method("born", &BorderWindow::born)
		._field("borderWidth", &BorderWindow::borderWidth)
	;
}


class MenuWindow : public virtual Window
{
public:
	static const int ID = 3;
	static const std::string Name;

public:
	virtual std::string getName() {
		return Name;
	}

	int getID() {
		return ID;
	}

	virtual int born() {
		return 101;
	}

};
const std::string MenuWindow::Name = "menu_window";


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<MenuWindow, Window>
		::define("inheritance_multiple::MenuWindow")
	
		._method("getID", &MenuWindow::getID)
	;
}


class MainWindow : public BorderWindow, public MenuWindow
{
public:
	static const int ID = 3;
	static const std::string Name;

public:
	virtual std::string getName() {
		return Name;
	}

	int getID() {
		return ID;
	}

	virtual int born() {
		return 101;
	}

};
const std::string MainWindow::Name = "main_window";


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<MainWindow, BorderWindow, MenuWindow>
		::define("inheritance_multiple::MainWindow")
	
		._method("getName", &MainWindow::getName)
		._method("getID", &MainWindow::getID)
	;
}


void doTest()
{
    using namespace cpgf;

	std::cout << "inheritance_multiple" << std::endl;

	const GMetaClass * mainWindow;
	const GMetaMethod * method;
	const GMetaField * field;

	mainWindow = findMetaClass("inheritance_multiple::MainWindow"); testCheckAssert(mainWindow != NULL);

	{
		void * window = mainWindow->createInstance();
		method = mainWindow->getMethodInHierarchy("getName", &window); testCheckAssert(method != NULL);
		testCheckStringEqual(fromVariant<std::string>(method->invoke(window)), MainWindow::Name);

		mainWindow->destroyInstance(window);
	}

	{
		void * window = mainWindow->createInstance();
		field = mainWindow->getFieldInHierarchy("borderWidth", &window); testCheckAssert(field != NULL);
		field->set(window, 3);
		testCheckEqual(((MainWindow *)window)->borderWidth, 3);

		mainWindow->destroyInstance(window);
	}



}


gTestCase(doTest);


}

