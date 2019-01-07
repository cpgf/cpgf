<!--notoc-->

# cpgf serialization -- a C++ open source library to serialization

cpgf serialization library is very easy to use serialization engine written in C++.  
The library uses reflection meta data extensively.

cpgf serialization is licensed under Apache License, Version 2.0

## Key features
  * As powerful as the serialization system in Java and C#.
  * Serialize complicated object graph.
  * Correctly resolve multiple pointers to same object.
  * Serialize polymorphic objects.
  * Serialize C++ array.
  * Very extensible. The library supports customized serializer to serialize special data type manually.
  * Storage format agnostic. Now support text stream, XML, and JSON. Adding more formats is easy!
  * Versioning.
  * Doesn't require extra intervention. Serialization only depends on relection meta data.

## Sample code
```c++
// the archive reader and writer
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"

// the text stream archive
#include "cpgf/serialization/gmetatextstreamarchive.h"

// meta traits for STL string and C++ array
#include "cpgf/metatraits/gmetaserializer_string.h"
#include "cpgf/metatraits/gmetaserializer_array.h"

#include <sstream>


// get the meta service
GScopedInterface<IMetaService> service(createDefaultMetaService());

// create the meta reader and writer on a string stream
stringstream stream;
GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
GScopedInterface<IMetaReader> reader(createTextStreamMetaReader(service.get(), stream));

// create the archive writer
GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer.get()));

// the object to serialize
TestSerializeClass instance;
serializeWriteObjectValue(archiveWriter.get(), "MyObject", &instance, metaClass.get());

// rewind the stream so we can read the content again
stream.seekg(0);

// create the archive reader
GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader.get()));

// the object to read on	
TestSerializeClass readInstance;

serializeReadObject(archiveReader.get(), "MyObject", &readInstance, metaClass.get());

// now readInstance should be identical to instance
```
