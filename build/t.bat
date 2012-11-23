@echo off

set TARGET=test_metagen
sh qtest.sh %TARGET%
if errorlevel 1 goto error

set TARGET=test_serialization
sh qtest.sh %TARGET%
if errorlevel 1 goto error

set TARGET=test_reflection
sh qtest.sh %TARGET%
if errorlevel 1 goto error

set TARGET=test_misc
sh qtest.sh %TARGET%
if errorlevel 1 goto error

set TARGET=test_scriptbind
sh qtest.sh %TARGET%
if errorlevel 1 goto error

set TARGET=test_variant
sh qtest.sh %TARGET%
if errorlevel 1 goto error

goto end

:error
echo Error occured -- %TARGET%
:end
