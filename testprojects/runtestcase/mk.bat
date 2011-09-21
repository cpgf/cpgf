pushd %cd%

cd %~p0

mingw32-make.exe -f makefile %*

popd
