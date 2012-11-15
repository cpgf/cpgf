Know issues:

10.shaders.py doesn't render the shader correct.

In 09.meshviewer, the xml is not .drop because there is bug in metagen.

11.perpixellighting.js crash on exit. Seems
var renderer = this.Driver.getMaterialRenderer(irr.EMT_PARALLAX_MAP_SOLID);
renderer is dropped?

13.rendertotexture.js and 13.rendertotexture.py crash on device.drop().
16.quake3mapshader.js and 16.quake3mapshader.py crash on device.drop().
