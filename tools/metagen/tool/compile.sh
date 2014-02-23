#!/bin/bash

mkdir -p bin
javac -cp ../js.jar:./src -d ./bin ./src/metagen.java
pushd bin && jar cmf ../metagen.mf metagen.jar metagen.class org ; popd
cp bin/metagen.jar ../metagen.jar
