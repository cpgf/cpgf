#ifndef __GRENDERENGINE2D_H
#define __GRENDERENGINE2D_H

#include "cpgf/game/gcore.h"
#include "cpgf/game/grenderengine.h"
#include "cpgf/game/gmath.h"
#include "cpgf/game/ggeometry.h"

#include "cpgf/gscopedptr.h"
#include "cpgf/gcallback.h"


#if defined(G_OS_WIN)
#include <windows.h>
#endif

#if defined(G_OPENGLES)
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>

	#define glOrtho glOrthof
#else
	#include <gl/gl.h>
#endif

#include <vector>

namespace cpgf {

const int colorShiftAlpha = 24;
const int colorShiftRed = 16;
const int colorShiftGreen = 8;
const int colorShiftBlue = 0;

const gcolor colorMaskChannel = 0xff;
const gcolor colorMaskAlpah = colorMaskChannel << colorShiftAlpha;
const gcolor colorMaskRed =	colorMaskChannel << colorShiftRed;
const gcolor colorMaskGreen = colorMaskChannel << colorShiftGreen;
const gcolor colorMaskBlue = colorMaskChannel << colorShiftBlue;

const gcolor colorWhite = 0xffffffff;
const gcolor colorBlack = 0xff000000;

inline gcolor makeColorARGB(gcolor a, gcolor r, gcolor g, gcolor b)
{
	return (a << colorShiftAlpha) + (r << colorShiftRed) + (g << colorShiftGreen) + (b << colorShiftBlue);
}

inline gcolor makeColorRGB(gcolor r, gcolor g, gcolor b)
{
	return makeColorARGB(colorMaskChannel, r, g, b);
}

inline gcolor makeColorOpaque(gcolor color)
{
	return color | colorMaskAlpah;
}

inline gcolor getColorAlpha(gcolor color)
{
	return (color >> colorShiftAlpha) & 0xff;
}

inline gcolor getColorRed(gcolor color)
{
	return (color >> colorShiftRed) & 0xff;
}

inline gcolor getColorGreen(gcolor color)
{
	return (color >> colorShiftGreen) & 0xff;
}

inline gcolor getColorBlue(gcolor color)
{
	return (color >> colorShiftBlue) & 0xff;
}

inline gcolor clearColorAlpha(gcolor color)
{
	return color & ~colorMaskAlpah;
}

inline gcolor clearColorRed(gcolor color)
{
	return color & ~colorMaskRed;
}

inline gcolor clearColorGreen(gcolor color)
{
	return color & ~colorMaskGreen;
}

inline gcolor clearColorBlue(gcolor color)
{
	return color & ~colorMaskBlue;
}

inline gcolor changeColorAlpha(gcolor color, gcolor alpha)
{
	return clearColorAlpha(color) | (alpha << colorShiftAlpha);
}

enum GBlendMode {
	blendModeZero = 0,
	blendModeOne,
	blendModeSourceColor,
	blendModeOneMinusSourceColor,
	blendModeDestColor,
	blendModeOneMinusDestColor,
	blendModeSourceAlpha,
	blendModeOneMinusSourceAlpha,
	blendModeDestAlpha,
	blendModeOneMinusDestAlpha,
	blendModeSourceAlphaSaturate,
};

const guint32 InvalidTextureName = guint32(-1);

typedef GMatrix4 GRenderMatrix;

class GRenderState
{
public:
	GRenderState() :
		matrix(NULL),
		clipRect(NULL)
	{
		this->reset();
	}

	GRenderState(const GRenderState &other) {
		this->assignFrom(other);
	}

	GRenderState & operator = (const GRenderState &other) {
		this->reset();

		this->assignFrom(other);

		return *this;
	}

	~GRenderState() {
		this->reset();
	}

	void reset() {
		this->color = colorBlack;
		this->zOrder = 0;
		this->blendMode = makeInt32From16(blendModeSourceAlpha, blendModeOneMinusSourceAlpha);
		this->linePointSize = makeInt32From16(1, 1);

		this->matrix.reset(NULL);
		this->clipRect.reset(NULL);
	}

	GRenderMatrix *getMatrix() {
		if(this->matrix == NULL) {
			this->matrix.reset(new GRenderMatrix());
		}

		return this->matrix.get();
	}

	GRect *getClipRect() {
		if(this->clipRect == NULL) {
			this->clipRect.reset(new GRect(0, -1, 0, -1));
		}

		return this->clipRect.get();
	}

	bool hasMatrix() const {
		return this->matrix != NULL;
	}

	bool hasClipRect() const {
		return this->clipRect != NULL && this->clipRect->isNormalized();
	}

private:
	GRenderMatrix *cloneMatrix() const {
		return this->hasMatrix() ? new GRenderMatrix(*this->matrix) : NULL;
	}

	GRect *cloneClipRect() const {
		return this->hasClipRect() ? new GRect(*this->clipRect) : NULL;
	}

	void assignFrom(const GRenderState &other) {
		this->color = other.color;
		this->zOrder = other.zOrder;
		this->blendMode = other.blendMode;
		this->linePointSize = other.linePointSize;
		this->matrix.reset(other.cloneMatrix());
		this->clipRect.reset(cloneClipRect());
	}

public:
	gcolor color;
	gint32 zOrder;
	gint32 blendMode; // high 16 bits -> source mode, low 16 bits -> dest mode
	gint32 linePointSize; // high 16 bits -> line width, low 16 bits -> point size
	GScopedPointer<GRenderMatrix> matrix;
	GScopedPointer<GRect> clipRect;
};

class GRenderEngine2D : public GRenderEngine
{
private:
	typedef GRenderEngine super;

public:
	typedef GCallback<void (GRenderEngine2D *)> CustomizeRenderCallback;

public:
	virtual void initializeEngine();
	virtual void finalizeEngine();

	virtual void beginRender();
	virtual void endRender();

	virtual void onEngineResize();

	inline void setColor(gcolor color) {
		this->renderState.color = color;
	}

	inline gcolor getColor() const {
		return this->renderState.color;
	}

	inline void setZOrder(int zOrder) {
		this->renderState.zOrder = zOrder;
	}

	inline int getZOrder() const {
		return this->renderState.zOrder;
	}

	inline void setBlendMode(GBlendMode sourceBlendMode, GBlendMode destBlendMode) {
		this->renderState.blendMode = makeInt32From16(sourceBlendMode, destBlendMode);
	}

	inline GBlendMode getBlendModeSource() const {
		return static_cast<GBlendMode>(getHigh16(this->renderState.blendMode));
	}

	inline GBlendMode getBlendModeDest() const {
		return static_cast<GBlendMode>(getLow16(this->renderState.blendMode));
	}

	inline GMatrix4 *getMatrix() {
		return this->renderState.getMatrix();
	}

	inline GRect *getClipRect() {
		return this->renderState.getClipRect();
	}

	inline void pushRenderState() {
		this->renderStateStack.push_back(this->renderState);
	}

	inline void popRenderState() {
		this->renderState = this->renderStateStack.back();
		this->renderStateStack.pop_back();
	}

	// render commands
	virtual void fillRect(const GRect *rect) = 0;
	virtual void drawLineStrip(const gfloat *lines, int lineCount) = 0;
	virtual void customizeRender(const CustomizeRenderCallback & cb) = 0;

protected:
	virtual void doBeginFlush() = 0;
	virtual void doFlush() = 0;
	virtual void doEndFlush() = 0;

protected:
	inline GRenderState & getRenderState() {
		return this->renderState;
	}

	inline GRenderState & getDefaultRenderState() {
		return this->defaultRenderState;
	}

private:
	GRenderState renderState;
	std::vector<GRenderState> renderStateStack;

	GRenderState defaultRenderState;
};


struct GVertexVector
{
	gint16 x;
	gint16 y;
};

struct GTextureVector
{
	GLfloat u;
	GLfloat v;
};

class GRenderCommand
{
public:
	GRenderCommand() : command(-1), textureName(InvalidTextureName) {
	}

	GRenderCommand & operator = (const GRenderCommand &other) {
		this->renderState = other.renderState;
		this->textureName = other.textureName;

		return *this;
	}

	void reset() {
		this->command = -1;
		this->textureName = InvalidTextureName;
		this->renderState.reset();
	}

	static int compare(const GRenderCommand * command1, const GRenderCommand * command2) {
		int result;

		result = compareValue(command1->renderState.zOrder, command2->renderState.zOrder);

		if(result) {
			return result;
		}

		result = compareValue(command1->textureName, command2->textureName);

		if(result) {
			return result;
		}

		return 0;
	}

	static bool sortCompare(const GRenderCommand * command1, const GRenderCommand * command2) {
		return compare(command1, command2) < 0;
	}

public:
	int command;

	GRenderState renderState;

	guint32 textureName;
	GVertexVector vertextPoints[4];
	GTextureVector texturePoints[4];

	void *param;
};

class GRenderCommandParam_LineStrip
{
public:
	void reset() {
		this->lineCount = 0;
	}

public:
	const gfloat *lines;
	int lineCount;
};

class GRenderCommandParam_CustomizeRender
{
public:
	GRenderCommandParam_CustomizeRender() : cb() {
	}

	~GRenderCommandParam_CustomizeRender() {
	}

	void reset() {
	}

public:
	GRenderEngine2D::CustomizeRenderCallback cb;
};

template <typename T>
class GBatchResetableList
{
private:
	typedef std::vector<T *> ListType;

public:
	GBatchResetableList() : itemCount(0) {
	}

	~GBatchResetableList() {
		this->clear();
	}

	inline int getCount() const {
		return this->itemCount;
	}

	T & requireItem() {
		if(this->itemCount == static_cast<int>(this->itemList.size())) {
			this->itemList.push_back(NULL);
		}
		T * item = this->itemList[this->itemCount];
		if(item == NULL) {
			item = new T();
			this->itemList[this->itemCount] = item;
		}
		++this->itemCount;

		return *item;
	}

	inline T & getItem(int index) {
		return *this->itemList[index];
	}

	void reserve(int n) {
		this->itemList.reserve(n);
	}

	void reset() {
		for(int i = 0; i < this->itemCount; ++i) {
			itemList[i]->reset();
		}

		this->itemCount = 0;
	}

	void clear() {
		for(typename ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
			delete *it;
		}
		this->itemList.clear();
		this->itemCount = 0;
	}

	template<typename Compare>
	void sort(Compare compare) {
		std::sort(itemList.begin(), itemList.end(), compare);
	}

private:
	int itemCount;
	ListType itemList;
};

class GBatchRenderEngine : public GRenderEngine2D
{
private:
	typedef GRenderEngine2D super;

public:
	GBatchRenderEngine();

	virtual void beginRender() override;
	virtual void endRender() override;

	virtual void fillRect(const GRect *rect) override;
	virtual void drawLineStrip(const gfloat *lines, int lineCount) override;
	virtual void customizeRender(const CustomizeRenderCallback & cb) override;

protected:
	virtual void doBeginFlush() override;
	virtual void doFlush() override;
	virtual void doEndFlush() override;

	GRenderCommand *requreCommand(int command);

protected:
	GBatchResetableList<GRenderCommand> commandList;
	GBatchResetableList<GRenderCommandParam_LineStrip> commandParamList_LineStrip;
	GBatchResetableList<GRenderCommandParam_CustomizeRender> commandParamList_CustomizeRender;
};


class GOpenGLRenderEngineOpenGL : public GBatchRenderEngine
{
private:
	typedef GBatchRenderEngine super;

public:
	virtual void initializeEngine() override;
	virtual void finalizeEngine() override;

	virtual void onEngineResize() override;

protected:
	virtual void doBeginFlush() override;
	virtual void doFlush() override;
	virtual void doEndFlush() override;

private:
};


} // namespace cpgf


#endif
