cpgf._import(None, "builtin")

world = 0
ground = 0

def render() :
	global world
	gl.glClear(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT + gl.GL_STENCIL_BUFFER_BIT)
	world.Step(0.016, 8, 3)
	world.DrawDebugData()
	gl.glutSwapBuffers()

def reshape(w, h) :
	gl.glViewport(0, 0, w, h)
	gl.glMatrixMode(gl.GL_PROJECTION)
	gl.glLoadIdentity()
	gl.gluOrtho2D(-25, 25, -5, 45)
	a = cpgf.GByteArray()
	#a = None

def specialKey(key, x, y) :
	return

def keyboard(key, x, y) :
	if(key == 27) : exitDemo()

def timer(value) :
	gl.glutPostRedisplay()
	gl.glutTimerFunc(value, timer, value)

def setupBox2d()  :
	global world
	global ground

	gravity = box2d.b2Vec2(0.0, -10.0)
	world = box2d.b2World(gravity, 1);
	bd = box2d.b2BodyDef()
	ground = world.CreateBody(bd)
	shape = box2d.b2PolygonShape()
	shape.SetAsEdge(box2d.b2Vec2(-40.0, 0.0), box2d.b2Vec2(40.0, 0.0))
	ground.CreateFixture(shape, 0.0)
	prevBody = ground

	# Define crank.
	shape = box2d.b2PolygonShape()
	shape.SetAsBox(0.5, 2.0)
	bd = box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 7.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)
	rjd = box2d.b2RevoluteJointDef()
	rjd.Initialize(prevBody, body, box2d.b2Vec2(0.0, 5.0))
	rjd.motorSpeed = 1.0 * box2d.b2_pi
	rjd.maxMotorTorque = 10000.0
	rjd.enableMotor = True
	m_joint1 = world.CreateJoint(rjd)
	prevBody = body

	# Define follower.
	shape = box2d.b2PolygonShape()
	shape.SetAsBox(0.5, 4.0)
	bd = box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 13.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)
	rjd = box2d.b2RevoluteJointDef()
	rjd.Initialize(prevBody, body, box2d.b2Vec2(0.0, 9.0))
	rjd.enableMotor = False
	world.CreateJoint(rjd)
	prevBody = body

	# Define piston
	shape = box2d.b2PolygonShape()
	shape.SetAsBox(1.5, 1.5)
	bd = box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 17.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)
	rjd = box2d.b2RevoluteJointDef()
	rjd.Initialize(prevBody, body, box2d.b2Vec2(0.0, 17.0))
	world.CreateJoint(rjd)
	pjd = box2d.b2PrismaticJointDef()
	pjd.Initialize(ground, body, box2d.b2Vec2(0.0, 17.0), box2d.b2Vec2(0.0, 1.0))
	pjd.maxMotorForce = 1000.0
	pjd.enableMotor = True
	m_joint2 = world.CreateJoint(pjd)

	# Define payload.
	shape = box2d.b2PolygonShape()
	shape.SetAsBox(1.5, 1.5)
	bd = box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 23.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)

def getWorld() :
	global world
	return world

def start() :
	global world
	gl.glutInit()
	gl.glutInitDisplayMode(gl.GLUT_DEPTH + gl.GLUT_DOUBLE + gl.GLUT_RGB + gl.GLUT_STENCIL)
	gl.glutInitWindowPosition(100, 100)
	gl.glutInitWindowSize(320, 320)
	gl.glutCreateWindow("cpgf Box2D binding demo -- Python version")
	gl.glutDisplayFunc(render)
	gl.glutReshapeFunc(reshape)
	gl.glutSpecialFunc(specialKey)
	gl.glutKeyboardFunc(keyboard)
	period = 16
	gl.glutTimerFunc(period, timer, period)

	gl.glShadeModel(gl.GL_SMOOTH)
	gl.glHint(gl.GL_PERSPECTIVE_CORRECTION_HINT, gl.GL_NICEST)
	gl.glHint(gl.GL_LINE_SMOOTH_HINT, gl.GL_NICEST)
	gl.glHint(gl.GL_POLYGON_SMOOTH_HINT, gl.GL_NICEST)

	gl.glutMainLoop()
