CPGF=/projects/cpgf
METAGEN=$CPGF/tools/metagen/
java -cp "$METAGEN/tool/bin;$METAGEN/js.jar" metagen --config config.tween.js --xml xml/index.xml
