@mingw32-make mingw TARGET=test
@rem @nmake mingw TARGET=test

@if errorlevel 1 goto error

@bin\test.exe
@goto end

:error
@echo error occurred in batch file.
:end
