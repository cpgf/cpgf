#ifndef __GCOMPOSITE_H
#define __GCOMPOSITE_H

#include "cpgf/game/gcore.h"

#include <vector>
#include <algorithm>

namespace cpgf {


template <typename ElementType>
class GSparseComposite
{
private:
	typedef int TypeType;
	typedef ElementType * ElementPtr;
	typedef ElementPtr * ElementPtrArray;
	typedef std::vector<ElementPtrArray> ElementPtrVector;

	static const int rowSize = 8;


	NonCopyableClass(GSparseComposite);

public:
	GSparseComposite() {
	}

	~GSparseComposite() {
		this->clear();
	}

	void addElement(const TypeType & type, ElementPtr element) {
		TypeType rowIndex, columnIndex;

		this->splitType(type, &rowIndex, &columnIndex);

		while(rowIndex >= (int)this->rowList.size()) {
			this->rowList.push_back(NULL);
		}

		ElementPtrArray row = this->rowList[rowIndex];
		if(row == NULL) {
			row = this->createRow();
			this->rowList[rowIndex] = row;
		}

		if(row[columnIndex] != NULL) {
			delete row[columnIndex];
		}

		row[columnIndex] = element;
	}

	void removeElement(const TypeType & type) {
		TypeType rowIndex, columnIndex;

		this->splitType(type, &rowIndex, &columnIndex);

		if(rowIndex < (int)this->rowList.size()) {
			ElementPtrArray row = this->rowList[rowIndex];
			if(row != NULL && row[columnIndex] != NULL) {
				delete row[columnIndex];
				row[columnIndex] = NULL;
			}
		}
	}

	ElementPtr getElement(const TypeType & type) const {
		TypeType rowIndex, columnIndex;

		this->splitType(type, &rowIndex, &columnIndex);

		if(rowIndex < (int)this->rowList.size()) {
			ElementPtrArray row = this->rowList[rowIndex];
			return row != NULL ? row[columnIndex] : NULL;
		}
		else {
			return NULL;
		}
	}

protected:
	void splitType(const TypeType & type, TypeType * rowIndex, TypeType * columnIndex) const {
		*rowIndex = type / rowSize;
		*columnIndex = type % rowSize;
	}

	ElementPtrArray createRow() const {
		ElementPtrArray row = new ElementPtr[rowSize];
		memset(row, 0, rowSize * sizeof(ElementPtr));

		return row;
	}

	void clear() {
		for(typename ElementPtrVector::iterator it = this->rowList.begin(); it != this->rowList.end(); ++it) {
			if(*it != NULL) {
				for(int i = 0; i < rowSize; ++i) {
					if((*it)[i] != NULL) {
						delete (*it)[i];
					}
				}

				delete *it;
			}
		}

		this->rowList.clear();
	}

private:
	ElementPtrVector rowList;
};


} // namespace cpgf



#endif
