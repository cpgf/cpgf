msbuild

if errorlevel 1 goto error

debug\bindv8

goto end

:error
echo ERROR!!!
:end
