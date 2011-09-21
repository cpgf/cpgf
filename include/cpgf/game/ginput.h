#ifndef __GINPUT_H
#define __GINPUT_H

#include "cpgf/game/gutility.h"
#include "cpgf/game/gevent.h"


namespace cpgf {


const int eventKeyFirst = 1;
const int eventKeyDown = eventKeyFirst;
const int eventKeyUp = eventKeyFirst + 1;
const int eventKeyDoublePress = eventKeyFirst + 2;
const int eventKeyLast = eventKeyDoublePress;

const int eventTouchFirst = eventKeyLast + 1;
const int eventTouchDown = eventTouchFirst;
const int eventTouchUp = eventTouchFirst + 1;
const int eventTouchMove = eventTouchFirst + 2;
const int eventTouchCancel = eventTouchFirst + 3;
const int eventTouchLast = eventTouchCancel;

const int eventSceneFirst = eventTouchLast + 1;
const int eventSceneEntered = eventSceneFirst;
const int eventSceneExited = eventSceneFirst + 1;
const int eventSceneLast = eventSceneExited;



const int keyInvalid = 0;

const int keyTouchFirst = 1;
const int keyTouch0 = keyTouchFirst; // left mouse button for mouse input, first touch for touch input
const int keyTouch1 = keyTouch0 + 1;  // right mouse button for mouse input, second touch for touch input
const int keyTouch2 = keyTouch0 + 2;  // middle mouse button for mouse input, third touch for touch input
const int keyTouch3 = keyTouch0 + 3;  // fourth touch for touch input
const int keyTouch4 = keyTouch0 + 4;  // fifth touch for touch input
const int keyTouch5 = keyTouch0 + 5;  // sixth touch for touch input
const int keyTouch6 = keyTouch0 + 6;  // seventh touch for touch input
const int keyTouch7 = keyTouch0 + 7;  // eighth touch for touch input
const int keyTouch8 = keyTouch0 + 8;  // ninth touch for touch input
const int keyTouch9 = keyTouch0 + 9;  // tenth touch for touch input
const int keyTouchLast = keyTouch9;

const int keyMouseLeft = keyTouch0;
const int keyMouseMiddle = keyTouch1;
const int keyMouseRight = keyTouch2;

const int keyFirst = keyTouchLast + 1;
const int keyEscape = keyFirst + 0;
const int keySpace = keyFirst + 1;
const int keyLeft = keyFirst + 2;
const int keyUp = keyFirst + 3;
const int keyRight = keyFirst + 4;
const int keyDown = keyFirst + 5;
const int keyLast = keyDown;


class GInputData
{
public:
	GInputData() {
	}

	GInputData(int message, int key, gcoord screenX, gcoord screenY)
		: message(message), key(key), screenX(screenX), screenY(screenY) {
	}

	int message;
	int key;
	gcoord screenX;
	gcoord screenY;
};

class GInputPool
{
private:
	typedef GIndexedArray<GInputData> InputPoolListType;

public:
	GInputPool(int inputPoolSize);
	virtual ~GInputPool();

	const GInputData * const peekNext();
	const GInputData * const getNext();

protected:
	void putToPool(const GInputData & inputData);
	void resetPoolState();

private:
	GScopedPointer<InputPoolListType> pool;
	int cursor;

	friend class GInput;
};


class GInput
{
public:
	GInput(int touchPoolSize = 100, int keyboardPoolSize = 30);
	virtual ~GInput();

	void updateInput();
	void resetInput();

	GInputPool * const getTouchPool() const;
	GInputPool * const getKeyboardPool() const;

protected:
	void putKeyInput(int message, int key);
	void putTouchInput(int message, int key, gcoord x, gcoord y);

private:
	GScopedPointer<GInputPool> touchPool;
	GScopedPointer<GInputPool> keyboardPool;

	friend class GRawInput;
};


class GRawInput
{
private:
	enum KeyImmediateState {
		ksIdle = 0, ksPress = 1, ksRelease = 2
	};

	static const int rawEventKeyFirst = eventKeyFirst;
	static const int rawEventKeyDown = eventKeyDown;
	static const int rawEventKeyUp = eventKeyUp;
	static const int rawEventKeyDoublePress = eventKeyDoublePress;
	static const int rawEventKeyLast = eventKeyLast;

	static const int rawEventTouchFirst = eventTouchFirst;
	static const int rawEventTouchDown = eventTouchDown;
	static const int rawEventTouchUp = eventTouchUp;
	static const int rawEventTouchMove = eventTouchMove;
	static const int rawEventTouchCancel = eventTouchCancel;
	static const int rawEventTouchLast = eventTouchLast;


	static const int keyCount = 20;

private:
	struct RawInputEvent {
		int message;
		int key;
		gcoord screenX;
		gcoord screenY;
	};

public:
	inline static void resetKeyState() {
		if(keyStateChanged) {
			keyStateChanged = false;

			keyImmediateStates.fillZero();
			keyDoubleStates.fillZero();
		}
	}

	inline static bool isKeyHold(int key) {
		return keyHoldStates[key] == ksPress;
	}

	inline static bool isKeyJustPressed(int key) {
		return keyImmediateStates[key] == ksPress;
	}

	inline static bool isKeyDoulbePressed(int key) {
		return keyDoubleStates[key] == ksPress;
	}

	inline static bool isAnyKeyHold() {
		return keyHoldStates.anyEqual(ksPress);
	}

	inline static bool isAnyKeyJustPressed() {
		return keyImmediateStates.anyEqual(ksPress);
	}

	inline static bool isAnyKeyDoublePressed() {
		return keyDoubleStates.anyEqual(ksPress);
	}

	inline static bool keyIsTouch(int key) {
		return key >= keyTouchFirst && key <= keyTouchLast;
	}

	inline static bool keyIsKeyboard(int key) {
		return key >= keyFirst && key <= keyLast;
	}

	inline static void keyDown(int key) {
		doKeyDown(key, false);
	}

	inline static void keyRelease(int key) {
		doKeyRelease(key);
	}

	inline static void touchDown(int touch, gcoord x, gcoord y) {
		putTouchInput(rawEventTouchDown, touch, x, y);
	}

	inline static void touchUp(int touch, gcoord x, gcoord y) {
		putTouchInput(rawEventTouchUp, touch, x, y);
	}

	inline static void touchMove(int touch, gcoord x, gcoord y) {
		putTouchInput(rawEventTouchMove, touch, x, y);
	}

	inline static void touchCancel(int touch) {
		putTouchInput(rawEventTouchCancel, touch, 0.0f, 0.0f);
	}

protected:
	inline static void doKeyDown(int key, bool isTouch) {
		gmillisecond ct = getCurrentMillisecond();
		gmillisecond t = ct - keyPressTime[key];

		if(keyHoldStates[key] == ksPress
            && keyDoubleStates[key] != ksPress
            && (isTouch && t <= touchDoubleInterval || !isTouch && t <= keyDoubleInterval)) {
			keyDoubleStates[key] = ksPress;
			putKeyInput(rawEventKeyDoublePress, key);
		}

		keyPressTime[key] = ct;

		keyImmediateStates[key] = ksPress;
		keyHoldStates[key] = ksPress;

		putKeyInput(rawEventKeyDown, key);

		keyStateChanged = true;
	}

	inline static void doKeyRelease(int key) {
		keyImmediateStates[key] = ksRelease;
		keyHoldStates[key] = ksIdle;
		keyDoubleStates[key] = ksIdle;

		keyStateChanged = true;

		putKeyInput(rawEventKeyUp, key);
	}

	static void putKeyInput(int message, int key);
	static void putTouchInput(int message, int key, gcoord x, gcoord y);

private:
	static GArray<KeyImmediateState> keyHoldStates;
	static GArray<KeyImmediateState> keyImmediateStates;
	static GArray<KeyImmediateState> keyDoubleStates;
	static GArray<gmillisecond> keyPressTime;

	static gmillisecond keyDoubleInterval;
	static gmillisecond touchDoubleInterval;

	static bool keyStateChanged;
};



} // namespace cpgf


#endif

