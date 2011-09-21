#include "test_common.h"


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

GMETA_DEFINE_CLASS(Window, Window, "inheritance_multiple::Window") {
	using namespace cpgf;

	GMETA_METHOD(getName);
	GMETA_METHOD(getID);
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

GMETA_DEFINE_CLASS(BorderWindow, BorderWindow, "inheritance_multiple::BorderWindow", Window, Border) {
	using namespace cpgf;

	GMETA_METHOD(getID);
	GMETA_METHOD(born);

	GMETA_FIELD(borderWidth);
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

GMETA_DEFINE_CLASS(MenuWindow, MenuWindow, "inheritance_multiple::MenuWindow", Window) {
	using namespace cpgf;

	GMETA_METHOD(getID);
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

GMETA_DEFINE_CLASS(MainWindow, MainWindow, "inheritance_multiple::MainWindow", BorderWindow, MenuWindow) {
	using namespace cpgf;

	GMETA_METHOD(getName);
	GMETA_METHOD(getID);
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

