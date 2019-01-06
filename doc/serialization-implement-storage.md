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
struct IMetaStorageReader : public IObject
{
  virtual uint32_t G_API_CC getArchiveType(const char * name) = 0;
  virtual uint32_t G_API_CC getClassTypeID(const char * name) = 0;

  virtual void G_API_CC readFundamental(const char * name, GVariantData * outValue) = 0;
  virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID) = 0;
  virtual void * G_API_CC readNullPointer(const char * name) = 0;

  virtual uint32_t G_API_CC beginReadObject(const char * name, uint32_t * outVersion) = 0;
  virtual void G_API_CC endReadObject(const char * name, uint32_t version) = 0;

  virtual uint32_t G_API_CC readReferenceID(const char * name) = 0;
  virtual IMetaClass * G_API_CC readMetaClassAndTypeID(IMetaService * service, uint32_t * outClassTypeID) = 0;
  virtual IMetaClass * G_API_CC readMetaClass(IMetaService * service, uint32_t classTypeID) = 0;

  virtual uint32_t G_API_CC beginReadArray(const char * name) = 0;
  virtual void G_API_CC endReadArray(const char * name) = 0;
};
```
```c++
virtual uint32_t G_API_CC getArchiveType(const char * name) = 0;
```
Get archive type for the data of "name". The types are defined in enum GMetaArchiveItemType.
```c++
virtual uint32_t G_API_CC getClassTypeID(const char * name) = 0;
```
Get the class type ID for the data of "name". Don't be confused, "name" is the name of the data, not the meta class.
```c++
virtual void G_API_CC readFundamental(const char * name, GVariantData * outValue) = 0;
```
Read a fundamental data into "outValue".
```c++
virtual char * G_API_CC readString(const char * name, IMemoryAllocator * allocator, uint32_t * outArchiveID) = 0;
```
Read a string. The memory must be allocated by the allocator. The string archive ID should be returned into "outArchiveID".
```c++
virtual void * G_API_CC readNullPointer(const char * name) = 0;
```
Read a null pointer. A NULL is returned.
```c++
virtual uint32_t G_API_CC beginReadObject(const char * name, uint32_t * outVersion) = 0;
```
Start reading an object. The object version is stored into "outVersion". The function returns the object archive ID.
```c++
virtual void G_API_CC endReadObject(const char * name, uint32_t version) = 0;
```
Finish reading an object.
```c++
virtual uint32_t G_API_CC readReferenceID(const char * name) = 0;
```
Read a reference archive ID. The function returns the reference archive ID.
```c++
virtual IMetaClass * G_API_CC readMetaClassAndTypeID(IMetaService * service, uint32_t * outClassTypeID) = 0;
```
Read the meta class and class type ID. The function returns the meta class and store the class type ID into "outClassTypeID".

This function is called when current archive type is matClassType.
```c++
virtual IMetaClass * G_API_CC readMetaClass(IMetaService * service, uint32_t classTypeID) = 0;
```
Read meta class for classTypeID.

This function is called together with getClassTypeID.
```c++
virtual uint32_t G_API_CC beginReadArray(const char * name) = 0;
```
Start reading an array. The function returns the array length.
```c++
virtual void G_API_CC endReadArray(const char * name) = 0;
```
Finish reading an array.
