// the test is from SliderCrank.h in Box2D testbed

world = 0
ground = 0

function render()
{
	gl.glClear(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT + gl.GL_STENCIL_BUFFER_BIT)
	world.Step(0.016, 8, 3)
	world.DrawDebugData()
	gl.glutSwapBuffers()
}

function reshape(w, h)
{
	gl.glViewport(0, 0, w, h)
	gl.glMatrixMode(gl.GL_PROJECTION)
	gl.glLoadIdentity()
	gl.gluOrtho2D(-25, 25, -5, 45)
}

function specialKey(key, x, y)
{
}

function keyboard(key, x, y)
{
	if(key == 27) exitDemo();
}

function timer(value)
{
	gl.glutPostRedisplay()
	gl.glutTimerFunc(value, timer, value)
}

function setupBox2d()
{
	gravity = new box2d.b2Vec2(0.0, -10.0)
	world = new box2d.b2World(gravity, 1)
	bd = new box2d.b2BodyDef()
	ground = world.CreateBody(bd)
	shape = new box2d.b2PolygonShape()
	shape.SetAsEdge(new box2d.b2Vec2(-40.0, 0.0), new box2d.b2Vec2(40.0, 0.0))
	ground.CreateFixture(shape, 0.0)
	prevBody = ground

	// Define crank.
	shape = new box2d.b2PolygonShape()
	shape.SetAsBox(0.5, 2.0)
	bd = new box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 7.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)
	rjd = new box2d.b2RevoluteJointDef()
	rjd.Initialize(prevBody, body, new box2d.b2Vec2(0.0, 5.0))
	rjd.motorSpeed = 1.0 * box2d.b2_pi
	rjd.maxMotorTorque = 10000.0
	rjd.enableMotor = true
	m_joint1 = world.CreateJoint(rjd)
	prevBody = body

	// Define follower.
	shape = new box2d.b2PolygonShape()
	shape.SetAsBox(0.5, 4.0)
	bd = new box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 13.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)
	rjd = new box2d.b2RevoluteJointDef()
	rjd.Initialize(prevBody, body, new box2d.b2Vec2(0.0, 9.0))
	rjd.enableMotor = false
	world.CreateJoint(rjd)
	prevBody = body

	// Define piston
	shape = new box2d.b2PolygonShape()
	shape.SetAsBox(1.5, 1.5)
	bd = new box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 17.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)
	rjd = new box2d.b2RevoluteJointDef()
	rjd.Initialize(prevBody, body, new box2d.b2Vec2(0.0, 17.0))
	world.CreateJoint(rjd)
	pjd = new box2d.b2PrismaticJointDef()
	pjd.Initialize(ground, body, new box2d.b2Vec2(0.0, 17.0), new box2d.b2Vec2(0.0, 1.0))
	pjd.maxMotorForce = 1000.0
	pjd.enableMotor = true
	m_joint2 = world.CreateJoint(pjd)

	// Define payload.
	shape = new box2d.b2PolygonShape()
	shape.SetAsBox(1.5, 1.5)
	bd = new box2d.b2BodyDef()
	bd.type = box2d.b2_dynamicBody
	bd.position.Set(0.0, 23.0)
	body = world.CreateBody(bd)
	body.CreateFixture(shape, 2.0)
}

function getWorld()
{
	return world
}

function start()
{
	gl.glutInit()
	gl.glutInitDisplayMode(gl.GLUT_DEPTH + gl.GLUT_DOUBLE + gl.GLUT_RGB + gl.GLUT_STENCIL)
	gl.glutInitWindowPosition(100, 100)
	gl.glutInitWindowSize(320, 320)
	gl.glutCreateWindow("cpgf Box2D binding demo -- V8 JavaScript version")
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
}
