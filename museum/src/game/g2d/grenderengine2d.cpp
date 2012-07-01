#include "cpgf/game/g2d/grenderengine2d.h"
#include "cpgf/game/gapplication.h"
#include "cpgf/game/gdebug.h"


namespace cpgf {


const int blendModeMap[] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_SRC_ALPHA_SATURATE,
};

const int renderCommand_CustomizedRender = 0;
const int renderCommand_DrawImage = 1;
const int renderCommand_DrawLineStrip = 2;
const int renderCommand_FillRect = 3;

void GRenderEngine2D::initializeEngine()
{
	this->defaultRenderState = this->renderState;
}

void GRenderEngine2D::finalizeEngine()
{
}

void GRenderEngine2D::onEngineResize()
{
}

void GRenderEngine2D::beginRender()
{
	this->renderStateStack.clear();

	this->renderState = this->defaultRenderState;
}

void GRenderEngine2D::endRender()
{
}




GBatchRenderEngine::GBatchRenderEngine()
{
	this->commandList.reserve(256);
}

void GBatchRenderEngine::beginRender()
{
	super::beginRender();
}

void GBatchRenderEngine::endRender()
{
	super::endRender();
}

GRenderCommand * GBatchRenderEngine::requreCommand(int command)
{
	GRenderCommand *item = &this->commandList.requireItem();

	item->command = command;
	item->renderState = this->getRenderState();

	return item;
}

void GBatchRenderEngine::fillRect(const GRect *rect)
{
	GRenderCommand *item = this->requreCommand(renderCommand_FillRect);

	GVertexVector *verts = item->vertextPoints;

	verts[0].x = verts[2].x = static_cast<gint16>(rect->left);
	verts[0].y = verts[1].y = static_cast<gint16>(rect->top);
	verts[1].x = verts[3].x = static_cast<gint16>(rect->right);
	verts[2].y = verts[3].y = static_cast<gint16>(rect->bottom);
}

void GBatchRenderEngine::drawLineStrip(const gfloat *lines, int lineCount)
{
	GRenderCommand *item = this->requreCommand(renderCommand_DrawLineStrip);
	GRenderCommandParam_LineStrip & param = this->commandParamList_LineStrip.requireItem();
	item->param = &param;

	param.lineCount = lineCount;
	param.lines = lines;
}

void GBatchRenderEngine::customizeRender(const CustomizeRenderCallback & cb)
{
	GRenderCommand *item = this->requreCommand(renderCommand_CustomizedRender);
	GRenderCommandParam_CustomizeRender & param = this->commandParamList_CustomizeRender.requireItem();
	item->param = &param;

	param.cb = cb;
}

void GBatchRenderEngine::doBeginFlush()
{
}

void GBatchRenderEngine::doFlush()
{
}

void GBatchRenderEngine::doEndFlush()
{
	this->commandList.reset();
	this->commandParamList_LineStrip.reset();
	this->commandParamList_CustomizeRender.reset();
}

inline void setOpenGLColor(gcolor color)
{
	glColor4ub(getColorRed(color), getColorGreen(color), getColorBlue(color), getColorAlpha(color));
}


void changeViewport(GRect rect)
{
	gcoord viewWidth = gEngine->getDevice()->getViewportWidth();
	gcoord viewHeight = gEngine->getDevice()->getViewportHeight();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(rect.right > viewWidth)
		rect.right = viewWidth;
	if(rect.bottom > viewHeight)
		rect.bottom = viewHeight;

	glViewport(static_cast<int>(rect.left), static_cast<int>(viewHeight - rect.bottom),
		static_cast<int>(rect.getWidth()), static_cast<int>(rect.getHeight()));

	debugCheckGL();

	glOrtho(rect.left, rect.right, rect.bottom, rect.top, -1.0f, 1.0f);

	debugCheckGL();

	glMatrixMode(GL_MODELVIEW);
}

void GOpenGLRenderEngineOpenGL::initializeEngine()
{
	super::initializeEngine();

	changeViewport(gEngine->getDevice()->getViewRect());
}

void GOpenGLRenderEngineOpenGL::finalizeEngine()
{
	super::finalizeEngine();
}

void GOpenGLRenderEngineOpenGL::onEngineResize()
{
	super::onEngineResize();

	changeViewport(gEngine->getDevice()->getViewRect());
}

void GOpenGLRenderEngineOpenGL::doBeginFlush()
{
	super::doBeginFlush();

	glEnable(GL_BLEND);
	glBlendFunc(blendModeMap[blendModeSourceAlpha], blendModeMap[blendModeOneMinusSourceAlpha]);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GOpenGLRenderEngineOpenGL::doEndFlush()
{
	super::doEndFlush();
}

void GOpenGLRenderEngineOpenGL::doFlush()
{
	super::doFlush();

	if(this->commandList.getCount() == 0) {
		return;
	}

	this->commandList.sort(GRenderCommand::sortCompare);

	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	GRenderState & defaultState = this->getDefaultRenderState();
	setOpenGLColor(defaultState.color);

	GRenderCommand dummyCommand;

	GRenderCommand &previousCommand = dummyCommand;

	int commandCount = this->commandList.getCount();

	for(int i = 0; i < commandCount; ++i) {
		GRenderCommand &currentCommand = this->commandList.getItem(i);
		int matrixPushedCount = 0;

		if(currentCommand.textureName != previousCommand.textureName) {
			if(currentCommand.textureName == InvalidTextureName) {
				glDisable(GL_TEXTURE_2D);
			}
			else {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, currentCommand.textureName);
			}

			debugCheckGL();
		}

		if(currentCommand.renderState.color != previousCommand.renderState.color) {
			setOpenGLColor(currentCommand.renderState.color);
		}

		if(currentCommand.renderState.linePointSize != previousCommand.renderState.linePointSize) {
			glLineWidth(static_cast<float>(getHigh16(currentCommand.renderState.linePointSize)));
			glPointSize(static_cast<float>(getLow16(currentCommand.renderState.linePointSize)));

			debugCheckGL();
		}

		if(currentCommand.renderState.hasClipRect() || previousCommand.renderState.hasClipRect()) {
			if(currentCommand.renderState.hasClipRect()) {
				changeViewport(*currentCommand.renderState.getClipRect());
			}
			else {
				changeViewport(gEngine->getDevice()->getViewRect());
			}
		}

		if(currentCommand.renderState.hasMatrix()) {
			glPushMatrix();
			matrixPushedCount++;

			gfloat data[16];
			currentCommand.renderState.getMatrix()->getDataColumnMajor(data);
			glLoadMatrixf(data);

			debugCheckGL();
		}

		switch(currentCommand.command) {
			case renderCommand_FillRect:
			{
				glVertexPointer(2, GL_SHORT, 0, currentCommand.vertextPoints);
				glTexCoordPointer(2, GL_FLOAT, 0, currentCommand.texturePoints);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
				debugCheckGL();

				break;

			case renderCommand_DrawLineStrip:
			{
				GRenderCommandParam_LineStrip *lineStrip = static_cast<GRenderCommandParam_LineStrip *>(currentCommand.param);
				glVertexPointer(2, GL_FLOAT, 0, lineStrip->lines);
				glDrawArrays(GL_LINE_STRIP, 0, lineStrip->lineCount + 1);
			}
				break;

			case renderCommand_CustomizedRender:
			{
				CustomizeRenderCallback *cb = static_cast<CustomizeRenderCallback *>(currentCommand.param);
				cb->invoke(this);
			}
				break;
		}

		while(matrixPushedCount-- > 0) {
			glPopMatrix();
		}

		previousCommand = currentCommand;
	}
}



} // namespace cpgf

