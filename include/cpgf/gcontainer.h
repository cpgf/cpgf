#ifndef __GCONTAINER_H
#define __GCONTAINER_H

#include <ctime>
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>

#include <math.h>
#include <string.h>
#include <assert.h>


namespace cpgf {


template <typename NodeType>
class GLinkedList
{
private:
	typedef NodeType * NodePtr;

public:
	GLinkedList() : head(NULL) {
	}

	void addNode(NodePtr node) {
		node->previous = NULL;
		node->next = head;

		if(head) {
			head->previous = node;
		}
		head = node;
	}

	void removeNode(NodePtr node) {
		if(node->previous) {
			node->previous->next = node->next;
		}
		if(node->next) {
			node->next->previous =node->previous;
		}

		if(node == head) {
			head = node->next;
		}
	}

	void clear() {
		this->head = NULL;
	}

	bool empty() const {
		return NULL == this->head;
	}

	NodePtr getFirst() const {
		return head;
	}

	NodePtr getNext(NodePtr node) const {
		return node->next;
	}

	bool contains(NodePtr node) const {
		NodePtr n = this->head;

		while(n && n != node) {
			n = n->next;
		}

		return n != NULL;
	}

	void swap(GLinkedList & other) {
		std::swap(this->head, other.head);
	}

private:
	NodePtr head;
};


namespace _internal {
	template <typename Ta, typename Tb, typename Comparer>
	inline bool compareEqual(Ta a, Tb b, Comparer compare)
	{
		return !compare(a, b) && !compare(b, a);
	}

	class GWiseIteratorIncDec_Normal
	{
	public:
		template <typename NodePtr>
		static NodePtr getNext(NodePtr node) {
			return node->getNext();
		}

		template <typename NodePtr>
		static NodePtr getPrevious(NodePtr node) {
			return node->getPrevious();
		}
	};

	class GWiseIteratorIncDec_Reverse
	{
	public:
		template <typename NodePtr>
		static NodePtr getNext(NodePtr node) {
			return node->getPrevious();
		}

		template <typename NodePtr>
		static NodePtr getPrevious(NodePtr node) {
			return node->getNext();
		}
	};

	class GWiseIteratorIncDec_Checked
	{
	public:
		template <typename NodePtr>
		static NodePtr getNext(NodePtr node) {
			return node ? node->getNext() : NULL;
		}

		template <typename NodePtr>
		static NodePtr getPrevious(NodePtr node) {
			return node ? node->getPrevious() : NULL;
		}
	};

	class GWiseIteratorIncDec_CheckedReverse
	{
	public:
		template <typename NodePtr>
		static NodePtr getNext(NodePtr node) {
			return node ? node->getPrevious() : NULL;
		}

		template <typename NodePtr>
		static NodePtr getPrevious(NodePtr node) {
			return node ? node->getNext() : NULL;
		}
	};

	template <typename NodeType>
	class GWiseIteratorRetainer;

	template <typename NodeType>
	class GWiseIteratorBase
	{
	private:
		typedef typename NodeType::value_type ValueType;

	protected:
		typedef GWiseIteratorRetainer<NodeType> RetainerType;

		friend class GWiseIteratorRetainer<NodeType>;

	public:
		GWiseIteratorBase()
			: node(NULL), retainer(NULL), previous(NULL), next(NULL) {
		}

		GWiseIteratorBase(NodeType * node, const RetainerType * retainer)
			: node(node), retainer(const_cast<RetainerType *>(retainer)), previous(NULL), next(NULL) {
		}

	protected:
		void attachToRetainer() {
			if(this->retainer != NULL) {
				this->retainer->attachIterator(this);
			}
		}

		void detachFromRetainer() {
			if(this->retainer != NULL) {
				this->retainer->detachIterator(this);
				this->retainer = NULL;
			}
		}

	public:
		NodeType * node;

	protected:
		RetainerType * retainer;

		GWiseIteratorBase * previous;
		GWiseIteratorBase * next;

		template <typename T>
		friend class GLinkedList;
	};

	template <typename IteratorCategory, typename NodeType, typename ValueRef, typename ValuePtr, typename IncType = GWiseIteratorIncDec_Normal>
	class GWiseIterator : public GWiseIteratorBase<NodeType> {
	private:
		typedef GWiseIteratorBase<NodeType> super;

		typedef typename super::RetainerType RetainerType;
		typedef typename NodeType::value_type ValueType;

	public:
		typedef IteratorCategory iterator_category;
		typedef ptrdiff_t difference_type;
		typedef ValueType value_type;
		typedef ValuePtr pointer;
		typedef ValueRef reference;

	public:
		GWiseIterator() : super() {
		}

		GWiseIterator(NodeType * node)
            : super(node, NULL) {
		}

		GWiseIterator(NodeType * node, const RetainerType * retainer)
            : super(node, retainer) {
			this->attachToRetainer();
		}

		GWiseIterator(const GWiseIterator &it) : super(it.node, it.retainer) {
			this->attachToRetainer();
		}

		~GWiseIterator() {
			this->detachFromRetainer();
		}

		GWiseIterator & operator = (const GWiseIterator & it) {
			if(this != &it) {
				this->node = it.node;

				if(this->retainer != it.retainer) {
					this->detachFromRetainer();
					this->retainer = it.retainer;
					this->attachToRetainer();
				}
			}

			return *this;
		}

		bool operator == (const GWiseIterator & other) const {
			return this->node == other.node;
		}

		bool operator != (const GWiseIterator & other) const {
			return ! (*this == other);
		}

		// preincrement
		GWiseIterator & operator ++ () {
			this->node = IncType::getNext(this->node);

			return *this;
		}

		// postincrement
		GWiseIterator operator ++ (int) {
			GWiseIterator it(*this);

			++ *this;

			return it;
		}

		// predecrement
		GWiseIterator & operator -- () {
			this->node = IncType::getPrevious(this->node);

			return *this;
		}

		// postdecrement
		GWiseIterator operator -- (int) {
			GWiseIterator it(*this);

			-- *this;

			return it;
		}

		ValueRef operator * () const {
			return this->node->value;
		}

		ValuePtr operator -> () const {
			return &this->node->value;
		}
	};

	template <typename NodeType>
	class GWiseIteratorRetainer
	{
	private:
		typedef GWiseIteratorBase<NodeType> IteratorType;
		typedef IteratorType * IteratorPtr;
		typedef GLinkedList<IteratorType> ListType;

	public:
		GWiseIteratorRetainer() {
		}

		GWiseIteratorRetainer(const GWiseIteratorRetainer &other) {
		}

		~GWiseIteratorRetainer() {
			IteratorPtr it = this->iteratorList.getFirst();
			while(it) {
				it->retainer = NULL;
				it = this->iteratorList.getNext(it);
			}
		}

		GWiseIteratorRetainer & operator = (const GWiseIteratorRetainer &other) {
			(void)other;

			return *this;
		}

		void swap(GWiseIteratorRetainer & other) {
			this->iteratorList.swap(other.iteratorList);
		}

		void attachIterator(IteratorPtr i) {
			assert(! this->iteratorList.contains(i));

			this->iteratorList.addNode(i);
		}

		void detachIterator(IteratorPtr i) {
			this->iteratorList.removeNode(i);
		}

		void willDestroyNode(const NodeType * node) {
			IteratorPtr it = this->iteratorList.getFirst();
			while(it) {
				if(it->node == node) {
					it->node = it->node->getPrevious();
				}

				it = this->iteratorList.getNext(it);
			}
		}

	private:
		ListType iteratorList;
	};

}

template <typename ValueType, typename Allocator = std::allocator<ValueType> >
class GWiseList
{
private:
	class GListNode;

	class GListNodeBase
	{
	public:
		typedef ValueType value_type;

	public:
		GListNodeBase() : previous(NULL), next(NULL) {
		}

		inline GListNode * getPrevious() const {
			return this->previous;
		}

		inline GListNode * getNext() const {
			return this->next;
		}

	public:
		GListNode * previous;
		GListNode * next;
	};

	class GListNode : public GListNodeBase
	{
	private:
		typedef GListNodeBase super;

	public:
		GListNode(const ValueType & value) : super(), value(value) {
		}

		~GListNode() {
		}

	public:
		ValueType value;
	};

public:
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GListNode, ValueType &, ValueType *> iterator;
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GListNode, const ValueType &, const ValueType *> const_iterator;
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GListNode, ValueType &, ValueType *, _internal::GWiseIteratorIncDec_Reverse> reverse_iterator;
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GListNode, const ValueType &, const ValueType *, _internal::GWiseIteratorIncDec_Reverse> const_reverse_iterator;

public:
	GWiseList() : itemCount(0) {
		this->init();
	}

	GWiseList(const GWiseList & other) : itemCount(0) {
		this->init();

		this->insert(this->begin(), other.begin(), other.end());
	}

	~GWiseList() {
		this->clear();

		delete this->head;
	}

	GWiseList & operator = (const GWiseList & other) {
		if(this != &other) {
			this->retainer = other.retainer;

			iterator myFirst = this->begin();
			iterator myLast = this->end();

			const_iterator otherFirst = other.begin();
			const_iterator otherLast = other.end();

			while(myFirst != myLast && otherFirst != otherLast) {
				*myFirst = *otherFirst;
				++myFirst;
				++otherFirst;
			}

			if(otherFirst == otherLast) {
				this->erase(myFirst, myLast);
			}
			else {
				this->insert(myLast, otherFirst, otherLast);
			}

			this->itemCount = other.itemCount;
		}

		return *this;
	}

	void swap(GWiseList & other) {
		std::swap(this->head, other.head);
		std::swap(this->itemCount, other.itemCount);
		this->retainer.swap(other.retainer);
	}

	void clear() {
		GListNode * node = this->head->next;

		while(node != this->head) {
			node = this->removeNode(node);
		}
	}

	ValueType & front() {
		assert(! this->empty());

		return this->head->next->value;
	}

	const ValueType & front() const {
		assert(! this->empty());

		return this->head->next->value;
	}

	ValueType & back() {
		assert(! this->empty());

		return this->head->previous->value;
	}

	const ValueType & back() const {
		assert(! this->empty());

		return this->head->previous->value;
	}

	iterator begin() {
		return iterator(this->head->next, &this->retainer);
	}

	const_iterator begin() const {
		return const_iterator(this->head->next, &this->retainer);
	}

	iterator end() {
		return iterator(this->castHead);
	}

	const_iterator end() const {
		return const_iterator(this->castHead);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(this->head->previous, &this->retainer);
	}

	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(this->head->previous, &this->retainer);
	}

	reverse_iterator rend() {
		return reverse_iterator(this->castHead);
	}

	const_reverse_iterator rend() const {
		return const_reverse_iterator(this->castHead);
	}

	iterator erase(const iterator & it) {
		return iterator(this->removeNode(it.node));
	}

	iterator erase(iterator first, iterator last) {
		while(first != last) {
			erase(first++);
		}

		return last;
	}

	reverse_iterator erase(const reverse_iterator & it) {
		return reverse_iterator(this->removeNode(it.node));
	}

	void remove(const ValueType & value) {
		GListNode * node = this->findNode(value);

		if(node != NULL) {
			this->removeNode(node);
		}
	}

	bool empty() const {
		return this->head->next == this->head;
	}

	size_t size() const {
		return this->itemCount;
	}

	void push_front(const ValueType & value) {
		GListNode * newNode = this->createNode(value);

		newNode->next = head->next;
		newNode->next->previous = newNode;
		head->next = newNode;
		newNode->previous = castHead;
	}

	void push_back(const ValueType & value) {
		GListNode * newNode = this->createNode(value);

		newNode->previous = head->previous;
		newNode->previous->next = newNode;
		head->previous = newNode;
		newNode->next = castHead;
	}

	// insert before "it"
	void insert(const iterator & it, const ValueType & value) {
		this->insertBefore(it, value);
	}

	template <typename InputIterator>
	void insert(const iterator & it, InputIterator first, InputIterator last) {
		for(; first != last; ++first) {
			insert(it, *first);
		}
	}

	void insertBefore(const iterator & it, const ValueType & value) {
		GListNode * newNode = this->createNode(value);
		GListNode * node = it.node;

		newNode->previous = node->previous;
		newNode->previous->next = newNode;
		node->previous = newNode;
		newNode->next = node;
	}

	void insertAfter(const iterator & it, const ValueType & value) {
		GListNode * newNode = this->createNode(value);
		GListNode * node = it.node;

		newNode->next = node->next;
		newNode->next->previous = newNode;
		node->next = newNode;
		newNode->previous = node;
	}

	void reverse() {
		GListNode * p = this->castHead;

		do {
			std::swap(p->previous, p->next);
			p = p->previous;
		} while(p != this->castHead);
	}

protected:
	void init() {
		this->head = new GListNodeBase();
		this->head->next = this->castHead;
		this->head->previous = this->castHead;
	}

	GListNode * createNode(const ValueType & value) {
		GListNode * newNode = new GListNode(value);

		++this->itemCount;

		return newNode;
	}

	GListNode * removeNode(GListNode * node) {
		GListNode * next;
		if(node != this->head) {
			this->retainer.willDestroyNode(node);

			next = node->next;

			node->previous->next = node->next;
			node->next->previous = node->previous;

			delete node;

			--this->itemCount;
		}
		else {
			next = this->castHead;
		}

		return next;
	}

	GListNode * findNode(const ValueType & value) {
		GListNode * next = this->head->next;

		while(next != this->head) {
			if(next->value == value) {
				return next;
			}

			next = next->next;
		}

		return NULL;
	}

private:
	union {
		GListNodeBase * head;
		GListNode * castHead;
	};

	size_t itemCount;

	_internal::GWiseIteratorRetainer<GListNode> retainer;
};

template <typename ValueType>
void swap(GWiseList<ValueType> & a, GWiseList<ValueType> & b) {
	a.swap(b);
}


template <typename ValueType, typename Comparer = std::less<ValueType>, typename Allocator = std::allocator<ValueType>  >
class GWiseSkipList
{
private:
	class GSkipListNode;

	class GSkipListNodeBase
	{
	public:
		typedef ValueType value_type;

	public:
		GSkipListNodeBase(int level) {
			previous = NULL;
			forward = new GSkipListNode * [level + 1];
		}

		~GSkipListNodeBase() {
			delete [] forward;
		}

		inline GSkipListNode * getNext() const {
			return this->forward[0];
		}

		inline GSkipListNode * getPrevious() const {
			return this->previous;
		}

	public:
		GSkipListNode ** forward;
		GSkipListNode * previous;
	};

	class GSkipListNode : public GSkipListNodeBase
	{
	private:
		typedef GSkipListNodeBase super;

	public:
		GSkipListNode(int level, const ValueType & value) : super(level), value(value) {
		}

		~GSkipListNode() {
		}

	public:
		ValueType value;
	};


	typedef GWiseSkipList<ValueType> ThisType;

private:
	enum { defaultActiveMaxLevel = 3, defaultMostMaxLevel = sizeof(int) * 8 - 2, maxMostMaxLevel = sizeof(int) * 8 - 2 };

public:
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GSkipListNode, ValueType &, ValueType *> iterator;
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GSkipListNode, const ValueType &, const ValueType *> const_iterator;
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GSkipListNode, ValueType &, ValueType *, _internal::GWiseIteratorIncDec_Reverse> reverse_iterator;
	typedef _internal::GWiseIterator<std::bidirectional_iterator_tag, GSkipListNode, const ValueType &, const ValueType *, _internal::GWiseIteratorIncDec_Reverse> const_reverse_iterator;

public:
	GWiseSkipList(const Comparer & compare = Comparer())
		: activeMaxLevel(defaultActiveMaxLevel), mostMaxLevel(defaultMostMaxLevel), probability(0.5f), compare(compare), currentLevel(0), itemCount(0) {
		this->init();
	}

	GWiseSkipList(int maxLevel, int mostMaxLevel, float probability = 0.5f, const Comparer & compare = Comparer())
		: activeMaxLevel(maxLevel), mostMaxLevel(mostMaxLevel), probability(probability), compare(compare), currentLevel(0), itemCount(0) {
		this->init();
	}

	GWiseSkipList(const GWiseSkipList & other)
		: activeMaxLevel(other.activeMaxLevel), mostMaxLevel(other.mostMaxLevel), probability(other.probability), compare(other.compare), currentLevel(0), itemCount(0) {
		this->init();

		for(const_iterator it = other.begin(); it != other.end(); ++it) {
			this->insert(*it);
		}
	}

	~GWiseSkipList() {
		this->clear();
		delete this->head;
		delete[] this->updateBuffer;
	}

	GWiseSkipList & operator = (GWiseSkipList other) {
		this->swap(other);

		return *this;
	}

	void swap(GWiseSkipList & other) {
		std::swap(this->head, other.head);
		std::swap(this->itemCount, other.itemCount);
		std::swap(this->activeMaxLevel, other.activeMaxLevel);
		std::swap(this->mostMaxLevel, other.mostMaxLevel);
		std::swap(this->compare, other.compare);
		std::swap(this->currentLevel, other.currentLevel);
		std::swap(this->nextLevelUpItemCount, other.nextLevelUpItemCount);
		std::swap(this->updateBuffer, other.updateBuffer);
		this->retainer.swap(other.retainer);
	}

	void clear() {
		GSkipListNode * node = this->head->getNext();

		while(node != this->castTail) {
			GSkipListNode * next = node->getNext();

			delete node;

			node = next;
		}

		for(int i = 0; i <= this->mostMaxLevel; ++i) {
			this->head->forward[i] = this->castTail;
		}

		this->itemCount = 0;
	}

	iterator begin() {
		return iterator(this->head->getNext(), &this->retainer);
	}

	const_iterator begin() const {
		return const_iterator(this->head->getNext(), &this->retainer);
	}

	iterator end() {
		return iterator(this->castTail);
	}

	const_iterator end() const {
		return const_iterator(this->castTail);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(this->tail->previous, &this->retainer);
	}

	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(this->tail->previous, &this->retainer);
	}

	reverse_iterator rend() {
		return reverse_iterator(this->castHead);
	}

	const_reverse_iterator rend() const {
		return const_reverse_iterator(this->castHead);
	}

	iterator erase(const iterator & it) {
		return iterator(this->doRemove(it.node->value));
	}

	reverse_iterator erase(const reverse_iterator & it) {
		return reverse_iterator(this->doRemove(it.node->value)->previous);
	}

	template <typename ValueT>
	iterator remove(const ValueT & value) {
		return iterator(this->doRemove(value));
	}

	bool empty() const {
		return this->head->forward[0] == this->castTail;
	}

	size_t size() const {
		return this->itemCount;
	}

	template <typename ValueT>
	bool contains(const ValueT & value) const {
		GSkipListNode * node = this->findNearestNode(value);

		return node != this->castTail && _internal::compareEqual(node->value, value, compare);
	}

	template <typename ValueT>
	iterator find(const ValueT & value) {
		GSkipListNode * node = this->findNearestNode(value);

		if(node != this->castTail && _internal::compareEqual(node->value, value, compare)) {
			return iterator(node);
		}
		else {
			return this->end();
		}
	}

	template <typename ValueT>
	const_iterator find(const ValueT & value) const {
		GSkipListNode * node = this->findNearestNode(value);

		if(node != this->castTail && _internal::compareEqual(node->value, value, compare)) {
			return const_iterator(node);
		}
		else {
			return this->end();
		}
	}

	iterator insert(const ValueType & value) {
		return this->insert(value, false);
	}

	iterator insert(const ValueType & value, bool allowDuplicate) {
		return iterator(this->doInsert(value, allowDuplicate));
	}

	iterator replace(const iterator &it, const ValueType & newValue) {
		return iterator(this->doReplace(it.node, newValue, false));
	}

	iterator replace(const iterator &it, const ValueType & newValue, bool allowDuplicate) {
		return iterator(this->doReplace(it.node, newValue, allowDuplicate));
	}

	reverse_iterator replace(const reverse_iterator &it, const ValueType & newValue) {
		return reverse_iterator(this->doReplace(it.node, newValue, false));
	}

	reverse_iterator replace(const reverse_iterator &it, const ValueType & newValue, bool allowDuplicate) {
		return reverse_iterator(this->doReplace(it.node, newValue, allowDuplicate));
	}

protected:
	GSkipListNode * doInsert(const ValueType & value, bool allowDuplicate) {
		GSkipListNode * node = this->castHead;
		GSkipListNode ** update = this->updateBuffer;

		this->resetUpdateBuffer();

		for(int i = this->currentLevel; i >= 0; i--) {
			while(node->forward[i] != this->castTail && compare(node->forward[i]->value, value)) {
				node = node->forward[i];
			}
			update[i] = node;
		}
		node = node->forward[0];


		if(node == this->castTail || allowDuplicate || !(_internal::compareEqual(node->value, value, compare))) {
			int level = this->getNewNodeLevel();

			if(level > this->currentLevel) {
				for (int i = this->currentLevel + 1; i <= level; ++i) {
					update[i] = this->castHead;
				}
				this->currentLevel = level;
			}

			node = new GSkipListNode(level, value);

			node->previous = update[0];

			for (int i = 0; i <= level; ++i) {
				node->forward[i] = update[i]->forward[i];
				update[i]->forward[i] = node;
			}

			node->forward[0]->previous = node;

			++this->itemCount;

			if(this->activeMaxLevel < this->mostMaxLevel) {
				if(this->itemCount >= this->nextLevelUpItemCount) {
					++this->activeMaxLevel;

					this->reloadNextLevelUpItemCount();
				}
			}

		}

		return node;
	}

	// return next node
	template <typename ValueT>
	GSkipListNode * doRemove(const ValueT & value) {
		GSkipListNode * node = this->castHead;
		GSkipListNode ** update = this->updateBuffer;

		this->resetUpdateBuffer();

		for(int i = this->currentLevel; i >= 0; i--) {
			while(node->forward[i] != this->castTail && compare(node->forward[i]->value, value)) {
				node = node->forward[i];
			}
			update[i] = node;
		}
		node = node->forward[0];


		GSkipListNode * next = NULL;

		if(node != this->castTail && _internal::compareEqual(node->value, value, compare)) {
			for(int i = 0; i <= this->currentLevel; ++i) {
				if (update[i]->forward[i] != node) {
					break;
				}
				update[i]->forward[i] = node->forward[i];
			}

			next = node->forward[0];

			next->previous = node->previous;

			this->retainer.willDestroyNode(node);

			delete node;

			while(this->currentLevel > 0 && this->head->forward[this->currentLevel] == this->castTail) {
				--this->currentLevel;
			}

			--this->itemCount;
		}

		return next;
	}

	GSkipListNode * doReplace(GSkipListNode * node, const ValueType & newValue, bool allowDuplicate) {
		if((node->previous == this->castHead || this->compare(node->value, newValue))
			&& (node->getNext() == this->castTail || this->compare(newValue, node->value))
			) {
			node->value = newValue;
		}
		else {
			this->doRemove(node->value);
			node = this->doInsert(newValue, allowDuplicate);
		}

		return node;
	}

	template <typename ValueT>
	GSkipListNode * findNearestNode(const ValueT & value) const {
		GSkipListNode * node = this->castHead;
		for(int i = this->currentLevel; i >= 0; i--) {
			while(node->forward[i] != this->castTail && compare(node->forward[i]->value, value)) {
				node = node->forward[i];
			}
		}
		node = node->forward[0];

		return node;
	}

private:
	void init() {
		if(this->activeMaxLevel < 0) {
			this->activeMaxLevel = defaultActiveMaxLevel;
		}
		else if(this->activeMaxLevel > maxMostMaxLevel) {
			this->activeMaxLevel = maxMostMaxLevel;
		}

		if(this->mostMaxLevel < 0) {
			this->mostMaxLevel = defaultMostMaxLevel;
		}
		else if(this->mostMaxLevel > maxMostMaxLevel) {
			this->mostMaxLevel = maxMostMaxLevel;
		}

		srand((unsigned int)time(NULL));

		updateBuffer = new GSkipListNode * [this->mostMaxLevel + 1];

		head = new GSkipListNodeBase(this->mostMaxLevel);
		for(int i = 0; i <= this->mostMaxLevel; ++i) {
			this->head->forward[i] = castTail;
		}
		tail->previous = castHead;

		this->reloadNextLevelUpItemCount();
	}

	int getNewNodeLevel() const {
		int level = 0;

		for(;;) {
			float p = (float)rand() / (float)RAND_MAX;

			if(p >= this->probability) {
				break;
			}

			++level;

			if(level >= this->activeMaxLevel) {
				break;
			}
		}

		if(level >= this->activeMaxLevel) {
			return this->activeMaxLevel;
		}
		else {
			return level;
		}
	}

	void resetUpdateBuffer() {
		memset(this->updateBuffer, 0, sizeof(GSkipListNode *) * (this->mostMaxLevel + 1));
	}

	void reloadNextLevelUpItemCount() {
		this->nextLevelUpItemCount = 1u << (this->activeMaxLevel + 1);
	}

private:
	int activeMaxLevel;
	int mostMaxLevel;
	const float probability;

	Comparer compare;

	int currentLevel;

	size_t itemCount;

	unsigned int nextLevelUpItemCount;

	union {
		GSkipListNodeBase * head;
		GSkipListNode * castHead;
		GSkipListNodeBase * tail;
		GSkipListNode * castTail;
	};

	GSkipListNode ** updateBuffer;
	_internal::GWiseIteratorRetainer<GSkipListNode> retainer;
};

template <typename ValueType, typename Comparer>
void swap(GWiseSkipList<ValueType, Comparer> & a, GWiseSkipList<ValueType, Comparer> & b) {
	a.swap(b);
}


namespace _internal {
	template<typename KeyType, typename ValueType>
	class GMapPair
	{
	public:
		GMapPair() : first(KeyType()), second(ValueType()) {
		}

		GMapPair(KeyType key, ValueType value) : first(key), second(value) {
		}

		GMapPair(KeyType key) : first(key), second(ValueType()) {
		}

		~GMapPair() {
		}

		bool operator == (const GMapPair & other) const {
			return this->first == other.first;
		}

		bool operator != (const GMapPair & other) const {
			return !(*this == other);
		}

		const KeyType & key() const {
			return first;
		}

		ValueType & value() const {
			return second;
		}

		KeyType first;
		ValueType second;
	};
}

template<typename KeyType, typename ValueType, typename KeyComparer = std::less<KeyType>, typename Allocator = std::allocator<_internal::GMapPair<KeyType, ValueType> > >
class GWiseMap
{
public:
	typedef _internal::GMapPair<KeyType, ValueType> PairType;

private:
	class GComparePair
	{
	public:
		GComparePair() : keyCompare(KeyComparer()) {
		}

		GComparePair(const KeyComparer & keyCompare) : keyCompare(keyCompare) {
		}

	public:
		bool operator () (const PairType & p1, const PairType & p2) const {
			return this->keyCompare(p1.first, p2.first);
		}

		bool operator () (const PairType & p, const KeyType & k) const {
			return this->keyCompare(p.first, k);
		}

		bool operator () (const KeyType & k, const PairType & p) const {
			return this->keyCompare(k, p.first);
		}

	private:
		KeyComparer keyCompare;
	};

private:
	typedef GWiseSkipList<PairType, GComparePair> ImplementType;

public:
	typedef typename ImplementType::iterator iterator;
	typedef typename ImplementType::const_iterator const_iterator;
	typedef typename ImplementType::reverse_iterator reverse_iterator;
	typedef typename ImplementType::const_reverse_iterator const_reverse_iterator;

public:
	GWiseMap(const KeyComparer & keyCompare = KeyComparer())
		: implementList(ImplementType(keyCompare)) {
	};

	GWiseMap(int maxLevel, int mostMaxLevel)
		: implementList(ImplementType(maxLevel, mostMaxLevel)) {
	};

	GWiseMap(const GWiseMap & other)
		: implementList(other.implementList) {
	}

	GWiseMap & operator = (const GWiseMap & other) {
		this->implementList = other.implementList;

		return *this;
	}

	void swap(GWiseMap & other) {
		this->implementList.swap(other.implementList);
	}

	iterator find(const KeyType & key) {
		return this->implementList.find(key);
	}

	const_iterator find(const KeyType & key) const {
		return this->implementList.find(key);
	}

	bool contains(const KeyType & key) const {
		return this->implementList.contains(key);
	}

	iterator begin() {
		return this->implementList.begin();
	}

	const_iterator begin() const {
		return this->implementList.begin();
	}

	iterator end() {
		return this->implementList.end();
	}

	const_iterator end() const {
		return this->implementList.end();
	}

	reverse_iterator rbegin() {
		return this->implementList.rbegin();
	}

	const_reverse_iterator rbegin() const {
		return this->implementList.rbegin();
	}

	reverse_iterator rend() {
		return this->implementList.rend();
	}

	const_reverse_iterator rend() const {
		return this->implementList.rend();
	}

	size_t size() const {
		return this->implementList.size();
	}

	bool empty() const {
		return this->implementList.empty();
	}

	iterator erase(const iterator & it) {
		return this->implementList.erase(it);
	}

	reverse_iterator erase(const reverse_iterator & it) {
		return this->implementList.erase(it);
	}

	iterator remove(const KeyType & key) {
		return this->implementList.remove(key);
	}

	iterator insert(const KeyType & key, const ValueType & value) {
		return this->insert(key, value, false);
	}

	iterator insert(const KeyType & key, const ValueType & value, bool allowDuplicate) {
		iterator it = this->implementList.insert(PairType(key, value), allowDuplicate);
		it->second = value;

		return it;
	}

	// return value is not determined if there are duplicates
	ValueType & operator[] (const KeyType & key) {
		return this->implementList.insert(key)->second;
	}

	void clear() {
		this->implementList.clear();
	}

private:
	ImplementType implementList;
};

template<typename KeyType, typename ValueType, typename KeyComparer>
void swap(GWiseMap<KeyType, ValueType, KeyComparer> & a, GWiseMap<KeyType, ValueType, KeyComparer> & b) {
	a.swap(b);
}


template <typename ValueType>
class GQuadNode
{
public:
	typedef ValueType value_type;

private:
	typedef GQuadNode * NodePtr;

	class NodeLink
	{
	public:
		NodeLink() {
			this->previousLink = this;
			this->nextLink = this;
			this->parentNode = NULL;
			this->childNode = NULL;
		}

		~NodeLink() {
			this->parentNode = NULL;
			this->childNode = NULL;
		}

		NodeLink *previousLink;
		NodeLink *nextLink;
		NodePtr parentNode;
		NodePtr childNode;
	};

public:
	GQuadNode() : value(ValueType()) {
		this->parentLink.childNode = this;
		this->childLink.parentNode = this;
	}

	explicit GQuadNode(ValueType value) : value(value) {
		this->parentLink.childNode = this;
		this->childLink.parentNode = this;
	}

protected:
	virtual ~GQuadNode() {
		this->clearChildren();

		if(this->getParentNode() != NULL) {
			this->setParentNode(NULL);
		}
	}

public:
	inline NodePtr getParentNode() const {
		return this->parentLink.parentNode;
	}

	void setParentNode(NodePtr parent) {
		this->unlinkFromParent();

		this->parentLink.parentNode = parent;

		if(parent != NULL) {
			this->parentLink.previousLink = parent->childLink.previousLink;
			this->parentLink.nextLink = &parent->childLink;
			parent->childLink.previousLink->nextLink = &this->parentLink;
			parent->childLink.previousLink = &this->parentLink;
		}
		else {
			delete this;
		}
	}

	void insertNodeAfterSibling(NodePtr sibling) {
		this->unlinkFromParent();

		this->parentLink.parentNode = sibling->getParentNode();

		this->parentLink.previousLink = &sibling->parentLink;
		this->parentLink.nextLink = sibling->parentLink.nextLink;
		sibling->parentLink.nextLink->previousLink = &this->parentLink;
		sibling->parentLink.nextLink = &this->parentLink;
	}

	void insertNodeBeforeSibling(NodePtr sibling) {
		this->unlinkFromParent();

		this->parentLink.parentNode = sibling->getParentNode();

		this->parentLink.nextLink = &sibling->parentLink;
		this->parentLink.previousLink = sibling->parentLink.previousLink;
		sibling->parentLink.previousLink->nextLink = &this->parentLink;
		sibling->parentLink.previousLink = &this->parentLink;
	}

	inline NodePtr getNextSibling() const {
		if(this->getParentNode() != NULL) {
			return this->parentLink.nextLink->childNode;
		}
		else {
			return NULL;
		}
	}

	inline NodePtr getPreviousSibling() const {
		if(this->getParentNode() != NULL) {
			return this->parentLink.previousLink->childNode;
		}
		else {
			return NULL;
		}
	}

	inline NodePtr getFirstChild() const {
		if(this->childLink.nextLink->childNode != this) {
			return this->childLink.nextLink->childNode;
		}
		else {
			return NULL;
		}
	}

	inline NodePtr getLastChild() const {
		if(this->childLink.previousLink->childNode != this) {
			return this->childLink.previousLink->childNode;
		}
		else {
			return NULL;
		}
	}

	void clearChildren() {
		NodeLink * link = &this->childLink;
		NodeLink * nextLink = link->nextLink;
		while(true) {
			link = nextLink;
			nextLink = link->nextLink;

			if(link->childNode == NULL) {
				break;
			}

			link->childNode->setParentNode(NULL);
		}
	}

	int getDepth() const {
		int depth = 0;

		const GQuadNode * node = const_cast<const GQuadNode *>(this);

		while(true) {
			node = node->getParentNode();
			if(node == NULL) {
				break;
			}
			++depth;
		}

		return depth;
	}

protected:
	void unlinkFromParent() {
		if(this->getParentNode() != NULL) {
			this->parentLink.parentNode = NULL;
			this->parentLink.previousLink->nextLink = this->parentLink.nextLink;
			this->parentLink.nextLink->previousLink = this->parentLink.previousLink;
		}
	}

private:
	inline NodePtr getNext() const {
		return this->getNextSibling();
	}

	inline NodePtr getPrevious() const {
		return this->getPreviousSibling();
	}

	GQuadNode(const GQuadNode &);
	GQuadNode & operator = (const GQuadNode &);

	friend class _internal::GWiseIteratorIncDec_Normal;
	friend class _internal::GWiseIteratorIncDec_Reverse;
	friend class _internal::GWiseIteratorIncDec_Checked;
	friend class _internal::GWiseIteratorIncDec_CheckedReverse;
	template <typename NodeType> friend class _internal::GWiseIteratorRetainer;

private:
	NodeLink parentLink;
	NodeLink childLink;

public:
	ValueType value;
};

template <typename ValueType>
class GWiseQuadNode : public GQuadNode<ValueType>
{
private:
	typedef GQuadNode<ValueType> super;
	typedef GQuadNode<ValueType> NodeType;

public:
	typedef _internal::GWiseIterator<std::forward_iterator_tag, NodeType, ValueType &, ValueType *, _internal::GWiseIteratorIncDec_Checked> iterator;
	typedef _internal::GWiseIterator<std::forward_iterator_tag, NodeType, const ValueType &, const ValueType *, _internal::GWiseIteratorIncDec_Checked> const_iterator;
	typedef _internal::GWiseIterator<std::forward_iterator_tag, NodeType, ValueType &, ValueType *, _internal::GWiseIteratorIncDec_CheckedReverse> reverse_iterator;
	typedef _internal::GWiseIterator<std::forward_iterator_tag, NodeType, const ValueType &, const ValueType *, _internal::GWiseIteratorIncDec_CheckedReverse> const_reverse_iterator;

public:
	GWiseQuadNode() : super() {
	}

	explicit GWiseQuadNode(ValueType value) : super(value) {
	}

	iterator begin() {
		return iterator(this->getFirstChild(), &this->retainer);
	}

	const_iterator begin() const {
		return const_iterator(this->getFirstChild(), &this->retainer);
	}

	iterator end() {
		return iterator(NULL);
	}

	const_iterator end() const {
		return const_iterator(NULL);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(this->getLastChild(), &this->retainer);
	}

	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(this->getLastChild(), &this->retainer);
	}

	reverse_iterator rend() {
		return reverse_iterator(NULL);
	}

	const_reverse_iterator rend() const {
		return const_reverse_iterator(NULL);
	}

	iterator erase(iterator it) {
		NodeType * node = static_cast<_internal::GWiseIteratorBase<NodeType> *>(&it)->node;
		this->retainer.willDestroyNode(node);
		node->setParentNode(NULL);
		return ++it;
	}

	reverse_iterator erase(reverse_iterator it) {
		NodeType * node = static_cast<_internal::GWiseIteratorBase<NodeType> *>(&it)->node;
		this->retainer.willDestroyNode(node);
		node->setParentNode(NULL);
		return ++it;
	}

	void clear() {
		this->clearChildren();
	}
	
private:
	GWiseQuadNode(const GWiseQuadNode &);
	GWiseQuadNode & operator = (const GWiseQuadNode &);

private:
	_internal::GWiseIteratorRetainer<GQuadNode<ValueType> > retainer;
};



} // namespace cpgf


#endif



