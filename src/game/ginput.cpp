#include "cpgf/game/ginput.h"
#include "cpgf/game/gapplication.h"
#include "cpgf/game/gdebug.h"


namespace cpgf {


GArray<GRawInput::KeyImmediateState> GRawInput::keyHoldStates(keyCount);
GArray<GRawInput::KeyImmediateState> GRawInput::keyImmediateStates(keyCount);
GArray<GRawInput::KeyImmediateState> GRawInput::keyDoubleStates(keyCount);
GArray<gmillisecond> GRawInput::keyPressTime(keyCount);

gmillisecond GRawInput::keyDoubleInterval = 300;
gmillisecond GRawInput::touchDoubleInterval = 500;

bool GRawInput::keyStateChanged = false;

static GInput * activeInput = NULL;


GInputPool::GInputPool(int inputPoolSize) : cursor(0)
{
	this->pool.reset(new InputPoolListType(inputPoolSize));
}

GInputPool::~GInputPool()
{
}

const GInputData * const GInputPool::peekNext()
{
	if(this->cursor >= this->pool->getCount()) {
		return NULL;
	}

	return &(*this->pool)[this->cursor];
}

const GInputData * const GInputPool::getNext()
{
	const GInputData * const next = this->peekNext();

	if(next != NULL) {
		this->cursor++;
	}

	return next;
}

void GInputPool::putToPool(const GInputData & inputData)
{
	if(this->pool->isFull()) {
		debugTrace("Input pool is full!!!");

		return;
	}

	this->pool->requireNext() = inputData;
}

void GInputPool::resetPoolState()
{
	this->cursor = 0;
	this->pool->reset();
}


GInput::GInput(int touchPoolSize, int keyboardPoolSize)
{
	debugAssert(activeInput == NULL, "GInput should be created for only once. Now multiple GInput are detected.");

	activeInput = this;

	this->touchPool.reset(new GInputPool(touchPoolSize));
	this->keyboardPool.reset(new GInputPool(keyboardPoolSize));
}

GInput::~GInput()
{
}

GInputPool * const GInput::getTouchPool() const
{
	return &(*this->touchPool);
}

GInputPool * const GInput::getKeyboardPool() const
{
	return &(*this->keyboardPool);
}

void GInput::putKeyInput(int message, int key)
{
	this->keyboardPool->putToPool(GInputData(message, key, 0.0f, 0.0f));
}

void GInput::putTouchInput(int message, int key, gcoord x, gcoord y)
{
	this->touchPool->putToPool(GInputData(message, key, x, y));
}

void GInput::updateInput()
{
}

void GInput::resetInput()
{
	this->touchPool->resetPoolState();
	this->keyboardPool->resetPoolState();

	GRawInput::resetKeyState();
}


void GRawInput::putKeyInput(int message, int key)
{
	if(activeInput != NULL) {
		activeInput->putKeyInput(message, key);
	}
}

void GRawInput::putTouchInput(int message, int key, gcoord x, gcoord y)
{
	if(activeInput != NULL) {
		activeInput->putTouchInput(message, key, x, y);
	}
}



} // namespace cpgf
