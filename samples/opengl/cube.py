#    v6-#-- v5
#   /|      /|
#  v1-#---v0|
#  | |     | |
#  | |v7-#|-|v4
#  |/      |/
#  v2-#---v3



rotate = 0
translateX = 0
translateY = 0
vertexList = 0
indexList = 0
colorList = 0
normalList = 0

def initData() :
	global rotate
	global translateX
	global translateY
	global vertexList
	global indexList
	global colorList
	global normalList

	vertexList = cpgf.createByteArray()
	indexList = cpgf.createByteArray()
	colorList = cpgf.createByteArray()
	normalList = cpgf.createByteArray()

	vertexList.writeFloat32(0.4)
	vertexList.writeFloat32(0.4)
	vertexList.writeFloat32(0.4)
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(0.4)  
	vertexList.writeFloat32(0.4)
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(0.4)
	vertexList.writeFloat32(0.4)  
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(0.4)
	vertexList.writeFloat32(0.4)  
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(-0.4)
	vertexList.writeFloat32(0.4)  
	vertexList.writeFloat32(0.4)  
	vertexList.writeFloat32(-0.4)
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(0.4)  
	vertexList.writeFloat32(-0.4)
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(-0.4) 
	vertexList.writeFloat32(-0.4)

	indexList.writeInt32(0) 
	indexList.writeInt32(1) 
	indexList.writeInt32(2) 
	indexList.writeInt32(3)
	indexList.writeInt32(0) 
	indexList.writeInt32(3) 
	indexList.writeInt32(4) 
	indexList.writeInt32(5)
	indexList.writeInt32(4) 
	indexList.writeInt32(5) 
	indexList.writeInt32(6) 
	indexList.writeInt32(7)
	indexList.writeInt32(1) 
	indexList.writeInt32(2) 
	indexList.writeInt32(7) 
	indexList.writeInt32(6)
	indexList.writeInt32(0) 
	indexList.writeInt32(1) 
	indexList.writeInt32(6) 
	indexList.writeInt32(5)
	indexList.writeInt32(2) 
	indexList.writeInt32(3) 
	indexList.writeInt32(4) 
	indexList.writeInt32(7)

	colorList.writeFloat32(1)  
	colorList.writeFloat32(0)  
	colorList.writeFloat32(0)
	colorList.writeFloat32(0) 
	colorList.writeFloat32(1)  
	colorList.writeFloat32(0)
	colorList.writeFloat32(0) 
	colorList.writeFloat32(0) 
	colorList.writeFloat32(1)
	colorList.writeFloat32(1)  
	colorList.writeFloat32(1) 
	colorList.writeFloat32(0)
	colorList.writeFloat32(1)  
	colorList.writeFloat32(0) 
	colorList.writeFloat32(1)
	colorList.writeFloat32(0)  
	colorList.writeFloat32(1)  
	colorList.writeFloat32(1)
	colorList.writeFloat32(1) 
	colorList.writeFloat32(1)  
	colorList.writeFloat32(1)
	colorList.writeFloat32(0) 
	colorList.writeFloat32(0) 
	colorList.writeFloat32(0)

	normalList.writeFloat32(1)  
	normalList.writeFloat32(0)  
	normalList.writeFloat32(0)
	normalList.writeFloat32(0) 
	normalList.writeFloat32(1)  
	normalList.writeFloat32(0)
	normalList.writeFloat32(0) 
	normalList.writeFloat32(0) 
	normalList.writeFloat32(1)
	normalList.writeFloat32(0)  
	normalList.writeFloat32(0) 
	normalList.writeFloat32(-1)
	normalList.writeFloat32(0)  
	normalList.writeFloat32(-1) 
	normalList.writeFloat32(0)
	normalList.writeFloat32(-1)  
	normalList.writeFloat32(0)  
	normalList.writeFloat32(0)
	normalList.writeFloat32(-1) 
	normalList.writeFloat32(1)  
	normalList.writeFloat32(0)
	normalList.writeFloat32(0) 
	normalList.writeFloat32(-1) 
	normalList.writeFloat32(-1)

def drawString(s) :
	global rotate
	global translateX
	global translateY
	global vertexList
	global indexList
	global colorList
	global normalList

	gl.glPushAttrib(gl.GL_LIGHTING_BIT + gl.GL_CURRENT_BIT)
	gl.glDisable(gl.GL_LIGHTING)
	positionList = cpgf.createByteArray()
	positionList.writeFloat32(-0.6)
	positionList.writeFloat32(-0.5)
	positionList.writeFloat32(0.5)
	gl.glRasterPos3fv(positionList.getPointer())
	for i in range(0, len(s)) :
		gl.glutBitmapCharacter(gl.GLUT_BITMAP_TIMES_ROMAN_24, ord(s[i]))
	gl.glEnable(gl.GL_LIGHTING)
	gl.glPopAttrib()

def render() :
	global rotate
	global translateX
	global translateY
	global vertexList
	global indexList
	global colorList
	global normalList

	gl.glClear(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT + gl.GL_STENCIL_BUFFER_BIT)

	gl.glEnableClientState(gl.GL_VERTEX_ARRAY)
	gl.glEnableClientState(gl.GL_COLOR_ARRAY)
	gl.glEnableClientState(gl.GL_NORMAL_ARRAY)
	gl.glPushMatrix()
	gl.glTranslatef(translateX, translateY, 0)
	gl.glRotatef(rotate, 1, 0, 1)
	gl.glVertexPointer(3, gl.GL_FLOAT, 0, vertexList.getPointer())
	gl.glColorPointer(3, gl.GL_FLOAT, 0, colorList.getPointer())
	gl.glNormalPointer(gl.GL_FLOAT, 0, normalList.getPointer())
	gl.glDrawElements(gl.GL_QUADS, 24, gl.GL_UNSIGNED_INT, indexList.getPointer())
	drawString("Python+OpenGL=Whoa!")
	gl.glPopMatrix()
	gl.glDisableClientState(gl.GL_VERTEX_ARRAY)
	gl.glDisableClientState(gl.GL_COLOR_ARRAY)
	gl.glDisableClientState(gl.GL_NORMAL_ARRAY)
	gl.glutSwapBuffers()

def reshape(w, h) :
	gl.glViewport(0, 0, w, h)
	gl.glMatrixMode(gl.GL_MODELVIEW)

def specialKey(key, x, y) :
	global rotate
	global translateX
	global translateY
	global vertexList
	global indexList
	global colorList
	global normalList

	delta = 0.02
	if key == gl.GLUT_KEY_LEFT :
		translateX = translateX - delta
	elif key == gl.GLUT_KEY_RIGHT :
		translateX = translateX + delta
	elif key == gl.GLUT_KEY_UP :
		translateY = translateY + delta
	elif key == gl.GLUT_KEY_DOWN :
		translateY = translateY - delta

def keyboard(key, x, y) :
	if(key == 27) : exitDemo()

def timer(value) :
	global rotate
	global translateX
	global translateY
	global vertexList
	global indexList
	global colorList
	global normalList
	
	rotate = rotate + 1
	rotate = rotate % 360
	gl.glutPostRedisplay()
	gl.glutTimerFunc(value, timer, value)

def initLights() :
	global rotate
	global translateX
	global translateY
	global vertexList
	global indexList
	global colorList
	global normalList

	lightAmbient = cpgf.createByteArray()
	lightAmbient.writeFloat32(0.6) 
	lightAmbient.writeFloat32(0.6) 
	lightAmbient.writeFloat32(0.6) 
	lightAmbient.writeFloat32(1.0)

	lightDiffuse = cpgf.createByteArray()
	lightDiffuse.writeFloat32(0.7) 
	lightDiffuse.writeFloat32(0.7) 
	lightDiffuse.writeFloat32(0.7) 
	lightDiffuse.writeFloat32(1.0)

	lightSpecular = cpgf.createByteArray()
	lightSpecular.writeFloat32(1) 
	lightSpecular.writeFloat32(1) 
	lightSpecular.writeFloat32(1) 
	lightSpecular.writeFloat32(1)

	lightPosition = cpgf.createByteArray()
	lightPosition.writeFloat32(0) 
	lightPosition.writeFloat32(1) 
	lightPosition.writeFloat32(-1) 
	lightPosition.writeFloat32(1)
	
	gl.glLightfv(gl.GL_LIGHT0, gl.GL_AMBIENT, lightAmbient.getPointer())
	gl.glLightfv(gl.GL_LIGHT0, gl.GL_DIFFUSE, lightDiffuse.getPointer())
	gl.glLightfv(gl.GL_LIGHT0, gl.GL_SPECULAR, lightSpecular.getPointer())
	gl.glLightfv(gl.GL_LIGHT0, gl.GL_POSITION, lightPosition.getPointer())
	gl.glEnable(gl.GL_LIGHT0)

def start() :
	global rotate
	global translateX
	global translateY
	global vertexList
	global indexList
	global colorList
	global normalList

	cpgf._import(None, "builtin.collections.bytearray")

	initData()
	gl.glutInit()
	gl.glutInitDisplayMode(gl.GLUT_DEPTH + gl.GLUT_DOUBLE + gl.GLUT_RGB + gl.GLUT_STENCIL)
	gl.glutInitWindowPosition(100, 100)
	gl.glutInitWindowSize(320, 320)
	gl.glutCreateWindow("cpgf OpenGL binding demo")
	gl.glutDisplayFunc(render)
	gl.glutReshapeFunc(reshape)
	gl.glutSpecialFunc(specialKey)
	gl.glutKeyboardFunc(keyboard)
	period = 33
	gl.glutTimerFunc(period, timer, period)

	gl.glShadeModel(gl.GL_SMOOTH)
	gl.glHint(gl.GL_PERSPECTIVE_CORRECTION_HINT, gl.GL_NICEST)
	gl.glHint(gl.GL_LINE_SMOOTH_HINT, gl.GL_NICEST)
	gl.glHint(gl.GL_POLYGON_SMOOTH_HINT, gl.GL_NICEST)

	gl.glEnable(gl.GL_DEPTH_TEST)
	gl.glEnable(gl.GL_LIGHTING)
	gl.glEnable(gl.GL_TEXTURE_2D)
	gl.glEnable(gl.GL_CULL_FACE)

	gl.glColorMaterial(gl.GL_FRONT_AND_BACK, gl.GL_AMBIENT_AND_DIFFUSE)
	gl.glEnable(gl.GL_COLOR_MATERIAL)
	gl.glClearColor(0, 0, 0, 0)
	gl.glClearStencil(0)
	gl.glClearDepth(1.0)
	gl.glDepthFunc(gl.GL_LEQUAL)

	initLights()
	gl.glutMainLoop()
