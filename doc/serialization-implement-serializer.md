

# Inside cpgf serialization -- implement serializer

Serializer is a meta traits that is used by the serialization library to allow new data types being serialized.
```c++

struct IMetaSerializer : public IObject
{
  virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) = 0;
  virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) = 0;
  
  virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) = 0;
  virtual void G_API_CC freeObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass, void * instance) = 0;
  virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param) = 0;
};
```
```c++

virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) = 0;
```
Get the data class type name.

metaClass may be NULL if the meta data is not available. For example, STL string doesn't have meta data. If metaClass is not NULL, metaClass->getQualifiedName() should be called and returned. If metaClass is NULL, a unique name must be returned.

The returned name is used to track object pointer.

This function is called by archive writer.
```c++

virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) = 0;
```

Write an object. All required information is stored in "param".

This function is called by archive writer.
```c++

virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) = 0;
```
Construct an instance.

This function is called by archive reader.
```c++

virtual void G_API_CC freeObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass, void * instance) = 0;
```
Destroy an instance.

This function is called by archive reader.
```c++

virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param) = 0;
```
Read an object. All required information is stored in "param".

This function is called by archive reader.

## GMetaArchiveReaderParam
```c++

struct GMetaArchiveReaderParam
{
  const char * name;
  const GMetaTypeData * metaType;
  IMetaClass * metaClass;
  void * instance;
  IMetaSerializer * serializer;
  uint32_t archiveVersion;
  GMetaArchiveConfigData config;
};
```
  * name: the data name.
  * metaType: the meta type of the data.
  * metaClass and instance: the meta class and instance pointer for current processed class.
  * serializer: The serializer for current instance.
  * config: The config data.
  * archiveVersion: The archive version.

## GMetaArchiveWriterParam
```c++

struct GMetaArchiveWriterParam
{
  const char * name;
  uint32_t archiveID;
  const GMetaTypeData * metaType;
  IMetaClass * metaClass;
  const void * instance;
  IMetaSerializer * serializer;
  uint32_t classTypeID;
  uint32_t pointers;
  GMetaArchiveConfigData config;
};
```
  * name: the data name.
  * archiveID: the object archive ID.
  * metaType: the meta type of the data.
  * metaClass and instance: the meta class and instance pointer for current processed class.
  * serializer: the serializer for current instance.
  * classTypeID: the class type ID.
  * pointers: the object pointer dimension. If pointers is 0, the object is by value or reference. If pointers is 1, the object is by pointer. pointers will not be greater than 1.
  * config: The config data.
