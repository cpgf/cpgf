# Serialize and deserialize objects

## Overview

The cpgf serialization library uses archive writer and reader to serialize and deserialize objects.  
If you don't know how to create archive writer and reader yet, please read the document of Create archive reader/writer and meta reader writer.

## Save objects and data to permanent storage

IMetaArchiveWriter is used to save objects and data to permanent storage.  
However, we usually don't need to invoke IMetaArchiveWriter functions directly.  
There are several global auxiliary functions to ease the use.
```c++
void serializeWriteObject(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass);
```
Write an object to archive.  
Parameter name is the object name that represents in the archive. Text stream archive will ignore the name. XML and JSON archive will use the name as node name.  
Parameter instance is a pointer to the object.  
Parameter metaClass is the meta type for the object.
```c++
template <typename T>
void serializeWriteData(IMetaArchiveWriter * archiveWriter, const char * name, const T & data);
```
Write arbitrary data to archive.  
data can be any type.  
It can be fundamental, such as int, double.  
It can also be any object, which meta class can be retrieved from global scope.  
It can also be any data that has a customized serializer.  
If the library can't serialize the data, an exception will be thrown.
```c++
template <typename T>
void serializeWriteData(IMetaArchiveWriter * archiveWriter, const char * name, const T & data, const GMetaModule * module);
```
Write arbitrary data to archive.  
The meta information is retrieved from "module".  
Currently there is no any document for GMetaModule, if you have interesting, please check the source code for serialization unit tests.

## Load objects and data from permanent storage

IMetaArchiveReader is used to load objects and data from permanent storage.  
However, we usually don't need to invoke IMetaArchiveReader functions directly.  
There are several global auxiliary functions to ease the use.
```c++
void serializeReadObject(IMetaArchiveReader * archiveReader, const char * name, void * instance, IMetaClass * metaClass);
```
Read an object at the address instance.  
instance must be allocated.
```c++
template <typename T>
void serializeReadData(IMetaArchiveReader * archiveReader, const char * name, T & instance);
```
Read arbitrary data at address &instance.
```c++
template <typename T>
void serializeReadData(IMetaArchiveReader * archiveReader, const char * name, T & instance, const GMetaModule * module);
```
