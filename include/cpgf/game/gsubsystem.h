#ifndef __GSUBSYSTEM_H
#define __GSUBSYSTEM_H

#include <vector>
#include <map>
#include <string>

#include "cpgf/gcontainer.h"
#include "cpgf/game/gcomposite.h"
#include "cpgf/gcallback.h"
#include "cpgf/game/gupdate.h"


namespace cpgf {


class GUpdateContext;
class GComponent;

const int invalidSubSystemType = -1;

class GSubSystem
{
private:
	class GComponentSortComparer
	{
	public:
		GComponentSortComparer() : subSystem(NULL) {
		}

		GComponentSortComparer(GSubSystem * subSystem) : subSystem(subSystem) {
		}

		inline bool operator () (const GComponent * a, const GComponent * b) {
			if(subSystem->componentCompare == NULL) {
				return a < b;
			}
			if((*subSystem->componentCompare)(a, b)) {
				return true;
			}
			if((*subSystem->componentCompare)(b, a)) {
				return false;
			}
			return a < b;
		}

	private:
		GSubSystem * subSystem;
	};

protected:
	typedef GWiseSkipList<GComponent *, GComponentSortComparer> ListType;
	typedef bool (*CompareType)(const GComponent *, const GComponent *);

	NonCopyableClass(GSubSystem);

public:
	GSubSystem(int type, int updateOrder, CompareType compare = NULL);
	virtual ~GSubSystem();

	virtual bool canHandle(int componentType);

	virtual void addComponent(GComponent * component);
	virtual void removeComponent(GComponent * component);

	void updateSubSystem(GUpdateContext * context);

public:
	inline int getType() const {
		return this->type;
	}

	inline int getUpdateOrder() const {
		return this->updateOrder;
	}

	virtual bool isRender() {
		return false;
	}

protected:
	virtual void doUpdateSubSystem(GUpdateContext * context);

protected:
	int type;
	int updateOrder;

	CompareType componentCompare;

	GScopedPointer<ListType> componentList;
};


class GSubSystemManager : public GObject, public IUpdatable, public IRenderable
{
private:
	typedef GObject super;
	typedef GSparseComposite<GSubSystem> CompositeType;

	class GSubSystemSortComparer
	{
	public:
		GSubSystemSortComparer() {
		}

		inline bool operator () (GSubSystem * a, GSubSystem * b) {
			if(a->getUpdateOrder() < b->getUpdateOrder()) {
				return true;
			}
			if(a->getUpdateOrder() > b->getUpdateOrder()) {
				return false;
			}
			return a < b;
		}
	};

	typedef GWiseSkipList<GSubSystem *, GSubSystemSortComparer> SortedListType;

	class GSubSystemSortedList
	{
	public:
		void addSubSystem(GSubSystem * subSystem);
		void removeSubSystem(GSubSystem * subSystem);

		void addComponent(GComponent * component);
		void removeComponent(GComponent * component);

		void execute(GUpdateContext * context);

	public:
		SortedListType sortedList;
	};

	NonCopyableClass(GSubSystemManager);

public:
	GSubSystemManager();
	virtual ~GSubSystemManager();

	void addSubSystem(GSubSystem * subSystem);
	void removeSubSystem(int type);

	void addComponent(GComponent * component);
	void removeComponent(GComponent * component);

	// IUpdatable
	virtual void executeUpdatable(GUpdateContext * context);
	// IRenderable
	virtual void executeRenderable(GUpdateContext * context);

	inline GSubSystem * getSubSystem(int type) const {
		return this->composite.getElement(type);
	}

protected:
	void clear();

private:
	CompositeType composite;
	GSubSystemSortedList updateList;
	GSubSystemSortedList renderList;
};


class GSubSystemRegister
{
public:
	typedef GCallback<GSubSystem * (int, int)> SubSystemCreatorType;

private:
	class GSubSystemRegisterItem {
	private:
		typedef std::map<int, bool> ComponentMapType;

	public:
		// for used in map
		GSubSystemRegisterItem()
			: name(""), type(-1), updateOrder(0), creator() {
		}

		GSubSystemRegisterItem(std::string name, int type, int updateOrder, const SubSystemCreatorType creator)
			: name(name), type(type), updateOrder(updateOrder), creator(creator) {
		}

		GSubSystemRegisterItem(const GSubSystemRegisterItem & other) {
			*this = other;
		}

		~GSubSystemRegisterItem() {
		}

		GSubSystemRegisterItem & operator = (const GSubSystemRegisterItem & other) {
			this->type = other.type;
			this->name = other.name;
			this->updateOrder = other.updateOrder;
			this->componentMap = other.componentMap;
			this->creator = other.creator;

			return * this;
		}

		void setCreator(const SubSystemCreatorType & creator) {
			this->creator = creator;
		}

		void setUpdateOrder(int updateOrder) {
			this->updateOrder = updateOrder;
		}

		GSubSystem * createSubSystem() const {
			return this->creator.invoke(this->type, this->updateOrder);
		}

		void addComponentType(int componentType) {
			this->componentMap[componentType] = true;
		}

		void removeComponentType(int componentType) {
			this->componentMap.erase(componentType);
		}

		void removeAllComponentType() {
			this->componentMap.clear();
		}

		bool containsComponentType(int componentType) {
			return this->componentMap.find(componentType) != this->componentMap.end();
		}

		int getType() const {
			return this->type;
		}

		const std::string & getName() const {
			return this->name;
		}

	private:
		std::string name;
		int type;
		int updateOrder;
		SubSystemCreatorType creator;
		ComponentMapType componentMap;
	};

private:
	typedef std::map<int, GSubSystemRegisterItem> MapType;

public:
	GSubSystemRegister();
	~GSubSystemRegister();

	int registerSubSystem(std::string name, int updateOrder, const SubSystemCreatorType & creator);

	void connectComponentToSubSystem(const std::string & subSystemName, const std::string & componentName);
	void connectComponentToSubSystem(int subSystemType, int componentType);
	void disconnectComponentFromSubSystem(int subSystemType, int componentType);
	void disconnectAllComponentFromSubSystem(int subSystemType);
	bool hasConnectedComponentToSubSystem(int subSystemType, int componentType);

	void fillSubSystemManager(GSubSystemManager * manager) const;

	int getSubSystemTypeFromName(const std::string & name);

private:
	GSubSystemRegisterItem * getRegisterItemFromType(int type);
	GSubSystemRegisterItem * getRegisterItemFromName(const std::string & name);

private:
	MapType subSystemMap;
	int availableType;
};

GSubSystemRegister * getSubSystemRegister();


} // namespace cpgf


#endif
