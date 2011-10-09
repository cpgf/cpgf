@echo off

set GEN=perl ..\..\scripts\gendoc.pl
set SRC_PATH=source
set OUT_PATH=output

rem %GEN% -o=%OUT_PATH% %SRC_PATH%/reflex-main.txt
%GEN% -o=%OUT_PATH% %SRC_PATH%/reflex-build-meta-data.txt
%GEN% -o=%OUT_PATH% %SRC_PATH%/reflex-use-meta-data-lib.txt
%GEN% -o=%OUT_PATH% %SRC_PATH%/library-vs-api.txt
%GEN% -o=%OUT_PATH% %SRC_PATH%/docs-gvariant.txt
%GEN% -o=%OUT_PATH% %SRC_PATH%/docs-lua.txt
%GEN% -o=%OUT_PATH% %SRC_PATH%/reflection-index.txt
%GEN% -o=%OUT_PATH% %SRC_PATH%/lua-index.txt

