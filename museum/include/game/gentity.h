#ifndef __GENTITY_H
#define __GENTITY_H

#include <algorithm>
#include <vector>

#include "cpgf/game/gutility.h"
#include "cpgf/gcontainer.h"
#include "cpgf/game/gcomposite.h"
#include "cpgf/game/gevent.h"
#include "cpgf/game/gdebug.h"


namespace cpgf {


class GComponent;
class GSubSystemManager;

class GEntity : public GEventDispatcher
{
private:
	typedef GSparseComposite<GComponent> CompositeType;

private:
	class GComponentInstallSession
	{
	public:
		GComponentInstallSession(GEntity * ownerEntity);

		void beginInstall();
		bool endInstall();

		inline void install(GComponent * component);

	private:
		std::vector<GComponent *> installList;
		GEntity * ownerEntity;
		int sessionCount;
	};

	NonCopyableClass(GEntity);

public:
	GEntity(GSubSystemManager * subSystemManager);
	virtual ~GEntity();

public:
	void beginInstallComponent();
	void endInstallComponent();
	void installComponent(GComponent * component);
	void uninstallComponent(int type);

	GSubSystemManager * getSubSystemManager() const {
		return this->subSystemManager;
	}

	GComponent * getComponent(int type) const {
		return this->composite.getElement(type);
	}

private:
	GScopedPointer<GComponentInstallSession> installSession;
	GSubSystemManager * subSystemManager;

	CompositeType composite;

private:
	GEntity * previous;
	GEntity * next;

	friend class GLinkedList<GEntity>;
};


class GEntityPool
{
public:
	virtual ~GEntityPool() {
		this->clear();
	}

	void addEntity(GEntity * entity) {
		this->pool.addNode(entity);
	}

	void removeEntity(GEntity * entity) {
		this->pool.removeNode(entity);
		this->deleteEntity(entity);
	}

protected:
	void clear() {
		GEntity * entity = this->pool.getFirst();

		while(entity != NULL) {
			GEntity * next = this->pool.getNext(entity);

			this->deleteEntity(entity);

			entity = next;
		}

		this->pool.clear();
	}

	virtual void deleteEntity(GEntity * entity) {
		delete entity;
	}

private:
	GLinkedList<GEntity> pool;
};


} // namespace cpgf


#endif

