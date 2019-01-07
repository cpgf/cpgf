# Create archive reader/writer and storage reader/writer
<!--begintoc-->
* [Overview](#a2_1)
  * [Concept disambiguition](#a3_1)
  * [Archive reader/writer](#a3_2)
  * [Storage reader/writer](#a3_3)
* [Create archive reader and writer.](#a2_2)
* [Create storage reader and writer.](#a2_3)
  * [Create storage reader/writer for text stream storage](#a3_4)
  * [Create storage reader/writer for XML storage](#a3_5)
  * [Create storage reader/writer for JSON storage](#a3_6)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

<a id="a3_1"></a>
### Concept disambiguition

There are two core concepts in the serialization library, archive and storage.

Archive, or archiving, is a shorter alias for serialization.  
Archive is a kind of virtual persistent data.  
Objects can be written to and read from archive.  
Archive deals with object meta information, pointer tracking, and exchange data with storage.  
Archive doesn't know where the data writes to or reads from, also doesn't know the physical data format.

Storage, as it's named, is the place where the data is stored in.  
It can be a memory stream, a XML or JSON tree, a disk file, or socket on the internet.  
Objects can not be processed by storage.  
Only basic data types, such as primary types (int, long, double, etc) and several other data types (string, null pointer) can be processed by storage.  
Storage takes care of physical data format, such as chunk of stream memory, XML node, etc.

Archive and storage are accessed through reader and writer.  
A reader is used to get data from archive and storage.  
A writer is used to send data to archive and storage.

<a id="a3_2"></a>
### Archive reader/writer

The interfaces IMetaArchiveReader and IMetaArchiveWriter are the archive reader and writer.  
The archive reader and writer are the core interfaces in the serialization library.

The archive writer does:
  * Parse meta data information in the object.
  * Write the basic field values (fundamental, string, null pointer) to storage writer.
  * Write the class information to storage writer.
  * Track pointers to serialize one object that's pointed by multiple pointers only once.
  * Create and use customized serializers for customized data type, such as std::string, C++ array. 

The archive reader does:
  * Parse meta data information in the object.
  * Allocate memory for object field, if the field is pointer with value NULL.
  * Read the basic field values (fundamental, string, null pointer) from storage reader.
  * Read the class information from storage reader.
  * Track pointers to deserialize one object that's pointed by multiple pointers only once.
  * Create and use customized serializers for customized data type, such as std::string, C++ array. 

<a id="a3_3"></a>
### Storage reader/writer

The interfaces IMetaStorageReader and IMetaStorageWriter is the meta reader and writer.  
The storage reader and writer is the bridge between archive reader/writer and permanent storage.  
The storage reader and writer takes care of storage format.  
So to support any new storage format, just implement new storage reader and writer and feed them to archive reader/writer.

The cpgf library has three built in storage formats:
  * Text stream -- use plain text stream as storage. See gmetastorage_textstream.h
  * XML -- use XML as storage. cpgf uses RapidXML to manipulate XML. See gmetastorage_xml.h
  * JSON -- use JSON as storage. cpgf uses Jsoncpp to manipulate JSON. See gmetastorage_json.h

<a id="a2_2"></a>
## Create archive reader and writer.

To create the interface for archive reader and writer, call below factory functions.
```c++
IMetaArchiveReader * createMetaArchiveReader(IMetaService * service, IMetaStorageReader * reader);
IMetaArchiveWriter * createMetaArchiveWriter(IMetaService * service, IMetaStorageWriter * writer);
```

How to get the storage reader and writer will be explained later.  
The meta service, IMetaService * service, is the meta data center that the archivers use to find meta data information.  
To get the service for global meta data, just call createDefaultMetaService(). It will return a IMetaService interface.

<a id="a2_3"></a>
## Create storage reader and writer.

<a id="a3_4"></a>
### Create storage reader/writer for text stream storage
```c++
IMetaStorageWriter * createTextStreamStorageWriter(std::ostream & outputStream);
IMetaStorageReader * createTextStreamStorageReader(std::istream & inputStream);
```

Text stream reader and writer works on C++ stream.  
Usually you can use a stringstream as the input and output stream.

<a id="a3_5"></a>
### Create storage reader/writer for XML storage
```c++
IMetaStorageWriter * createXmlStorageWriter(const GMetaXmlStorage & xmlStorage);
IMetaStorageReader * createXmlStorageReader(const GMetaXmlStorage & xmlStorage);
```

XML reader and writer works on GMetaXmlStorage.  
GMetaXmlStorage is a simple wrapper for RapidXML.  
RapidXML is included in cpgf library.

Functions in GMetaXmlStorage
```c++
void load(const char * xmlContent) const;
```
Load XML content.  
xmlContent is a string with the XML content.  
GMetaXmlStorage will copy xmlContent to internal buffer.  
So xmlContent can be freed after this function.
```c++
void saveToStream(std::ostream & outputStream) const;
```
Save the XML content to a stream.
```c++
void loadIntrusive(char * xmlContent) const;
```
Load XML content.  
xmlContent must be valid during the object life.  
And the content in xmlContent will be destroyed by GMetaXmlStorage.  
This function is an optimization version of load. It doesn't copy the xmlContent, and use xmlContent in place.
```c++
GScopedInterface<IMetaService> service(createDefaultMetaService());

GMetaXmlStorage storage;
GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));

GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer.get()));

// here we can write object to archiveWriter.

GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));

GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader.get()));

// here we can read object from archiveReader.
```

<a id="a3_6"></a>
### Create storage reader/writer for JSON storage
```c++
IMetaStorageWriter * createJsonStorageWriter(const GMetaJsonStorage & jsonStorage);
IMetaStorageReader * createJsonStorageReader(const GMetaJsonStorage & jsonStorage);
```

JSON reader and writer works on GMetaJsonStorage.  
GMetaJsonStorage is a simple wrapper for JsonCPP library.  
JsonCPP is included in cpgf library.

Functions in GMetaJsonStorage
```c++
void load(const char * jsonContent) const;
```
Load JSON content.  
jsonContent is a string with the JSON content.  
GMetaJsonStorage will copy jsonContent to internal buffer.  
So jsonContent can be freed after this function.
```c++
void saveToStream(std::ostream & outputStream) const;
```
Save the JSON content to a stream.
