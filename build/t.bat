@mingw32-make mingw TARGET=test
@rem @nmake mingw TARGET=test

@if errorlevel 1 goto end

@bin\test.exe
:end
