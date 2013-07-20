if not exist bin mkdir bin
javac -cp ../js.jar;./src -d ./bin ./src/metagen.java
cd bin
jar cmf ../metagen.mf metagen.jar metagen.class org
mv metagen.jar ../../
