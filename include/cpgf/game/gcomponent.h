#ifndef __GCOMPONENT_H
#define __GCOMPONENT_H

#include <map>
#include <vector>

#include "cpgf/game/gcore.h"
#include "cpgf/gcontainer.h"
#include "cpgf/gcallback.h"
#include "cpgf/gscopedptr.h"


namespace cpgf {


const int invalidComponentType = -1;

class GEntity;
class GUpdateContext;


class GComponent
{
public:
	typedef GCallback<bool (GComponent *, GUpdateContext *)> ComponentUpdaterType;
	typedef ComponentUpdaterType * ComponentUpdaterTypePtr;

public:
	static void componentInit() {
	}

	NonCopyableClass(GComponent);

public:
	GComponent(int type);
	virtual ~GComponent();

	virtual void attachToEntity(GEntity *entity);
	virtual void detachFromEntity(GEntity *entity);

	void setUpdater(ComponentUpdaterTypePtr updater);

	int getParentedLevel() const;

public:
	inline int getType() const {
		return this->type;
	}

	inline GEntity * getEntity() const {
		return this->ownerEntity;
	}

	inline void setEnabled(bool enabled) {
		this->enabled = enabled;
	}

	inline bool getEnabled() const {
		return this->enabled;
	}

	inline void setParentComponent(GComponent * parent) {
		this->parentComponent = parent;
	}

	inline GComponent * getParentComponent() const {
		return this->parentComponent;
	}

	inline int getUpdateOrder() const {
		return this->updateOrder;
	}

	inline void prepareUpdateOrder() {
		this->updateOrder = this->loadUpdateOrder();
	}

	inline void update(GUpdateContext * context) {
		if(this->updater && this->updater->invoke(this, context)) {
			return;
		}

		this->doUpdate(context);
	}

protected:
	virtual void doUpdate(GUpdateContext * context);
	virtual int loadUpdateOrder();

protected:
	int type;
	GEntity * ownerEntity;

	GScopedPointer<ComponentUpdaterType> updater;

	bool enabled;

	GComponent * parentComponent;

	int updateOrder;
};

inline bool compareComponentUpdateOrder(const GComponent * a, const GComponent * b)
{
	return a->getUpdateOrder() < b->getUpdateOrder();
}

inline bool revertCompareComponentUpdateOrder(const GComponent * a, const GComponent * b)
{
	return b->getUpdateOrder() < a->getUpdateOrder();
}


class GComponentRegister
{
private:
	typedef void (* ComponentInitType)();
	typedef GCallback<GComponent * (int)> ComponentCreatorType;
	typedef std::vector<ComponentInitType> ComponentInitListType;

private:
	class GComponentRegisterChildItem {
	public:
		GComponentRegisterChildItem() : creator() {
		};

		GComponentRegisterChildItem(const ComponentCreatorType & creator) : creator(creator) {
		};

		GComponentRegisterChildItem(const GComponentRegisterChildItem & other) {
			*this = other;
		}

		~GComponentRegisterChildItem() {
		}

		GComponentRegisterChildItem & operator = (const GComponentRegisterChildItem & other) {
			this->creator = other.creator;

			return *this;
		}

		void setCreator(const ComponentCreatorType & creator) {
			this->creator = creator;
		}

	private:
		ComponentCreatorType creator;
	};

	typedef std::map<std::string, GComponentRegisterChildItem> ChildItemMapType;

	class GComponentRegisterItem {
	public:
		// for used in map
		GComponentRegisterItem()
			: name(""), type(invalidComponentType) {
		}

		GComponentRegisterItem(std::string name, int type)
			: name(name), type(type) {
		}

		~GComponentRegisterItem() {
		}

		GComponentRegisterItem & operator = (const GComponentRegisterItem & other) {
			this->type = other.type;
			this->name = other.name;
			this->childItemmap = other.childItemmap;

			return * this;
		}

		int getType() const {
			return this->type;
		}

		const std::string & getName() const {
			return this->name;
		}

	public:
		ChildItemMapType childItemmap;

	private:
		std::string name;
		int type;
	};

private:
	typedef std::map<int, GComponentRegisterItem> MapType;

public:
	GComponentRegister();
	~GComponentRegister();

	void beginRegister();
	void endRegister();

	int registerComponent(const std::string & name, const std::string & className, const ComponentCreatorType & creator, ComponentInitType init);
	int getComponentTypeFromName(const std::string & name);

private:
	GComponentRegisterItem * getRegisterItemFromType(int type);
	GComponentRegisterItem * getRegisterItemFromName(const std::string & name);

private:
	MapType componentMap;
	int availableType;
	int updateCount;
	GScopedPointer<ComponentInitListType> componentInitList;
};

GComponentRegister * getComponentRegister();



} // namespace cpgf


#endif
