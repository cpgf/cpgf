A sample for using SFML -- Simple and Fast Multimedia Library -- in Lua and Google V8 Javascript

The build script is included in the main project in folder "build".

The sample requires SFML 1.6 and at least Lua or Google V8 library.

The samples are Pong and PostFX in SFML sample applications.

All the SFML related code is written in Lua, Javascript, or Python.

To run the sample, after compiled the application, copy folder datas from SFML samples/bin to build/bin,
then go to build/bin, run
samplesfml ScriptFileName

ScriptFileName is full path of Javascript or Lua file.
If ScriptFileName is omitted, samplesfml will try to use sfml.js in current directory.
