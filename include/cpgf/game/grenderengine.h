#ifndef __GRENDERENGINE_H
#define __GRENDERENGINE_H


namespace cpgf {


class GRenderEngine
{
public:
	virtual void initializeEngine() {
	}

	virtual void finalizeEngine() {
	}

	virtual void beginRender() {
	}

	virtual void endRender() {
	}

	virtual void onEngineResize() {
	}

	void flush() {
		this->doBeginFlush();
		this->doFlush();
		this->doEndFlush();
	}

protected:
	virtual void doBeginFlush() = 0;
	virtual void doFlush() = 0;
	virtual void doEndFlush() = 0;
};



} // namespace cpgf


#endif
