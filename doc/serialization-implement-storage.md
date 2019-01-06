# Inside cpgf serialization -- implement storage layer

cpgf serialization library contains three storage implementations, text stream, JSON, and XML. We may add new storage format by implementing the interface IMetaStorageWriter and IMetaStorageReader.

## Implement IMetaStorageWriter

IMetaStorageWriter is used to store data to the storage.
```c++

struct IMetaStorageWriter : public IObject
{
  virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
  virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
  virtual void G_API_CC writeNullPointer(const char * name) = 0;

  virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version) = 0;
  virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version) = 0;

  virtual void G_API_CC writeReferenceID(const char * name, uint32_t referenceArchiveID) = 0;
  virtual void G_API_CC writeMetaClass(uint32_t classTypeID, IMetaClass * metaClass) = 0;

  virtual void G_API_CC beginWriteArray(const char * name, uint32_t length) = 0;
  virtual void G_API_CC endWriteArray(const char * name, uint32_t length) = 0;
};
```
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```
Write a fundamental value to the storage.

name is the data member name. In streaming storage name can be omitted. In structured storage name can be used as the node name.
```c++

virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
```
Write a string to the storage. archiveID should be stored as well to track string reference.
```c++

virtual void G_API_CC writeNullPointer(const char * name) = 0;
```
Write a null pointer to the storage.
```c++

virtual void G_API_CC beginWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version) = 0;
```
Start writing an object.

classTypeID is the class type id for the object. The ID and the meta class name were written by the function writeMetaClass.

version is the object version number.
```c++

virtual void G_API_CC endWriteObject(const char * name, uint32_t archiveID, uint32_t classTypeID, uint32_t version) = 0;
```
Finish writing an object.
```c++

virtual void G_API_CC writeReferenceID(const char * name, uint32_t referenceArchiveID) = 0;
```
Write a reference ID to the storage.

This function is called when the archive layer meets an object, which at the same address of another object which had been written to the storage. referenceArchiveID is the archive ID of the "another object".
```c++

virtual void G_API_CC writeMetaClass(uint32_t classTypeID, IMetaClass * metaClass) = 0;
```
Write meta class information to the storage.
```c++

virtual void G_API_CC beginWriteArray(const char * name, uint32_t length) = 0;
```
Start writing an array.
```c++

virtual void G_API_CC endWriteArray(const char * name, uint32_t length) = 0;
```
Finish writing an array.

## Implement IMetaStorageReader

IMetaStorageReader is used to retrieve data from storage.
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```0
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```1
Get archive type for the data of "name". The types are defined in enum GMetaArchiveItemType.
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```2
Get the class type ID for the data of "name". Don't be confused, "name" is the name of the data, not the meta class.
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```3
Read a fundamental data into "outValue".
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```4
Read a string. The memory must be allocated by the allocator. The string archive ID should be returned into "outArchiveID".
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```5
Read a null pointer. A NULL is returned.
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```6
Start reading an object. The object version is stored into "outVersion". The function returns the object archive ID.
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```7
Finish reading an object.
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```8
Read a reference archive ID. The function returns the reference archive ID.
```c++

virtual void G_API_CC writeFundamental(const char * name, const GVariantData * value) = 0;
```9
Read the meta class and class type ID. The function returns the meta class and store the class type ID into "outClassTypeID".

This function is called when current archive type is matClassType.
```c++

virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
```0
Read meta class for classTypeID.

This function is called together with getClassTypeID.
```c++

virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
```1
Start reading an array. The function returns the array length.
```c++

virtual void G_API_CC writeString(const char * name, uint32_t archiveID, const char * value) = 0;
```2
Finish reading an array.
