#ifndef __GUPDATE_H
#define __GUPDATE_H


#include "cpgf/game/gutility.h"


namespace cpgf {


class GRenderEngine;
class GInput;

class GUpdateContext
{
public:
	int durationTime; // millisecond
	GRenderEngine * renderEngine;
	GInput * input;
};

struct IUpdatable : public IGInterface
{
	virtual void executeUpdatable(GUpdateContext * context) = 0;
};

struct IRenderable : public IGInterface
{
	virtual void executeRenderable(GUpdateContext * context) = 0;
};

class GUpdatableList : public GWiseList<IUpdatable *>
{
private:
	typedef GWiseList<IUpdatable *> super;

public:
	void update(GUpdateContext * context) const {
		for(super::const_iterator it = this->begin(); it != this->end(); ++it) {
			(*it)->executeUpdatable(context);
		}
	}
};

class GRenderableList : public GWiseList<IRenderable *>
{
private:
	typedef GWiseList<IRenderable *> super;

public:
	void render(GUpdateContext * context) const {
		for(super::const_iterator it = this->begin(); it != this->end(); ++it) {
			(*it)->executeRenderable(context);
		}
	}
};

class GUpdateRunner
{
public:
	void update(GUpdateContext * context) const {
		this->updatableList.update(context);
	}

	void render(GUpdateContext * context) const {
		this->renderableList.render(context);
	}

	void addUpdatable(IUpdatable * updatable) {
		this->updatableList.push_back(updatable);
	}

	void removeUpdatable(IUpdatable * updatable) {
		this->updatableList.remove(updatable);
	}

	void addRenderable(IRenderable * renderable) {
		this->renderableList.push_back(renderable);
	}

	void removeRenderable(IRenderable * renderable) {
		this->renderableList.remove(renderable);
	}

private:
	GUpdatableList updatableList;
	GRenderableList renderableList;
};


} // namespace cpgf


#endif
