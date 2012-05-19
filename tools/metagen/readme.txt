How to compile metagen
1, Download Rhino Javascript for Java and put js.jar in this folder.
2, Go to folder ./tool, run compile.bat if you are on Windows. If you are on Linux system, take the command from compile.bat and run them.

How to run metagen

To execute metagen.jar
	java -cp js.jar -jar .metagen.jar --config YourConfig.js --xml DoxygenXML.xml

To execute from classes	
	java -cp "tool/bin;js.jar" metagen --config YourConfig.js --xml DoxygenXML.xml