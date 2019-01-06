

# cpgf metagen -- C++ meta data generator for reflection and script binding

## Get start

Metagen is a tool to generate meta data from Doxygen XML automatically and generate the corresponding C++ code.  
The tool itself is writter in Java.

To use this tool, you need very basic Javascript knowledge to understand the config file format which is a piece of Javascript script.

The tool is in folder cpgf/tools/metagen.  
The Box2D meta data generator is in folder cpgf/tools/metagen/metagen_box2d, there are Doxygen project, metagen config file, and shell script to execute the tool. The are good sample for start.  
Also there is SFML meta data generator.  
## Prepare Doxygen XML files

The tool uses Doxygen XML files for C++ code information.  
The tool only needs the code information, so your C++ code is not necessary documented.  
If you are going to generate meta data for a library, all header files with public API should be included in the Doxygen project, otherwise, some classes information may be missed.

Below listed Doxygen tags and values which required by metagen and has non-default value.
```c++
EXTRACT_ALL = YES
EXTRACT_STATIC = YES
SHOW_INCLUDE_FILES = NO
GENERATE_TODOLIST = NO
GENERATE_TESTLIST = NO
GENERATE_BUGLIST = NO
GENERATE_DEPRECATEDLIST = NO
SHOW_USED_FILES = NO
SHOW_FILES = NO
REFERENCES_LINK_SOURCE = NO
VERBATIM_HEADERS = NO
GENERATE_HTML = NO
GENERATE_LATEX = NO
GENERATE_XML = YES
XML_PROGRAMLISTING = NO
```

## Write config file

Metagen needs a config file to obtain necessary information.  
The config file itself is a Javascript script with a variable named "config", which is an Javascript object.  
Variable config contains several key-value pairs.

All properties in the Javascript config file are same name and same meaning with com.cpgf.metagen.Config  
Config.java is well documented.

Beside config, any valid Javascript code can be put into the config file, such as temporary variable, functions, etc.

## Tool command line options

To execute the tool, run,  
java -cp js.jar -jar .metagen.jar --config ConfigFileName --xml DoxygenXML.xml

## Metagen takes two must-have options.
  * --xml DoxygenXMLFileName
  * --config ConfigFileName

DoxygenXMLFileName can be any valid Doxygen XML file name. But usually it should be the index.xml.  
The tool will parse the XML file, and all XML files it references to. So a index.xml is enough to parse all library.

ConfigFileName is the config Javascript.

## The status and capability of the tool

Currently metagen has some useful features. It succeeds on generating meta data for Box2D physical engine, SFML multimedia library and Irrlicht 3D engine. 

Now the tool can not only generate meta data, but also it can automatically deal with special case for scripting usage, such as wrap operator to functions.

The advanced features supported by metagen

  * Wrap operators to functions. So C++ operator overloading can be used in JavaScript which doesn't support operator overloading.
  * Generate wrapper class to be inherited from script.
  * Wrap bitfields with properties.

If you are going to generate meta data to use in script, here are some limitations:
  * The tool can't handle script callback as we did in Glut binding manually.

The future goal of the tool is to be able to generate script usable meta data for wxWidgets or equivalent complex libraries.
