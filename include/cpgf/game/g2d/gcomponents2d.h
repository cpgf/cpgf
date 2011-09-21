#ifndef __GCOMPONENTS2D_H
#define __GCOMPONENTS2D_H

#include <limits>

#include "cpgf/game/gcomponent.h"
#include "cpgf/game/gentity.h"
#include "cpgf/game/gmath.h"

using namespace std;

namespace cpgf {


const char * const componentName_Render = "render";
const char * const componentName_Orientation = "orientation";
const char * const componentName_LocalTransform = "localtranform";
const char * const componentName_GlobalTransform = "globaltransform";
const char * const componentName_Motion = "motion";
const char * const componentName_TouchInput = "touch";
const char * const componentName_KeyboardInput = "keyboard";


class GOrientationComponent : public GComponent
{
private:
	typedef GComponent super;

private:
	static int thisComponentType;

public:
	static void componentInit() {
		super::componentInit();

		thisComponentType = getComponentRegister()->getComponentTypeFromName(componentName_Orientation);
	}

public:
	GOrientationComponent(int type = 0)
		: super(thisComponentType),
		position(GVector2(0)),
		scale(GVector2(1)),
		rotate(0),
		zOrder(0)
	{
	}

	~GOrientationComponent() {
	}

public:
	void setX(gcoord x) {
		this->position.setX(x);
	}

	gcoord getX() const {
		return this->position.getX();
	}

	void setY(gcoord y) {
		this->position.setY(y);
	}

	gcoord getY() const {
		return this->position.getY();
	}

	void setZOrder(int zOrder) {
		this->zOrder = zOrder;
	}

	int getZOrder() const {
		return this->zOrder;
	}

	void setPosition(gcoord x, gcoord y) {
		this->setX(x);
		this->setY(y);
	}

	void setScaleX(gcoord scaleX) {
		this->scale.setX(scaleX);
	}

	gcoord getScaleX() const {
		return this->scale.getX();
	}

	void setScaleY(gcoord scaleY) {
		this->scale.setY(scaleY);
	}

	gcoord getScaleY() const {
		return this->scale.getY();
	}

	void setRotate(gradian rotate) {
		this->rotate = rotate;
	}

	gradian getRotate() const {
		return this->rotate;
	}

	void applyToMatrix(GMatrix4 * matrix);

private:
	GVector2 position;
	GVector2 scale;
	gradian rotate;
	int zOrder;
};


class GLocalTransformComponent : public GComponent
{
private:
	typedef GComponent super;

private:
	static int thisComponentType;
	static int orientationComponentType;

public:
	static void componentInit() {
		super::componentInit();

		thisComponentType = getComponentRegister()->getComponentTypeFromName(componentName_LocalTransform);
		orientationComponentType = getComponentRegister()->getComponentTypeFromName(componentName_Orientation);
	}

public:
	GLocalTransformComponent(int type = 0)
		: super(thisComponentType),
		matrix(new GMatrix4)
	{
	}

	~GLocalTransformComponent() {
	}

public:
	GMatrix4 * getMatrix() const {
		return this->matrix.get();
	}

protected:
	virtual void doUpdate(GUpdateContext * context) override;

private:
	GScopedPointer<GMatrix4> matrix;
};


class GGlobalTransformComponent : public GComponent
{
private:
	typedef GComponent super;

private:
	static int thisComponentType;
	static int localTransformComponentType;

public:
	static void componentInit() {
		super::componentInit();

		thisComponentType = getComponentRegister()->getComponentTypeFromName(componentName_GlobalTransform);
		localTransformComponentType = getComponentRegister()->getComponentTypeFromName(componentName_LocalTransform);
	}

public:
	GGlobalTransformComponent(int type = 0)
	  : super(thisComponentType),
		matrix(NULL)
	{
	}

	~GGlobalTransformComponent() {
	}

	GMatrix4 * getMatrix() const {
		return this->matrix.get();
	}

	virtual int loadUpdateOrder() override;

	virtual void attachToEntity(GEntity *entity) override;
	virtual void detachFromEntity(GEntity *entity) override;

protected:
	virtual void doUpdate(GUpdateContext * context) override;

private:
	GScopedPointer<GMatrix4> matrix;
};


class GRenderComponent : public GComponent
{
private:
	typedef GComponent super;

private:
	static int thisComponentType;
	static int globalTransformComponentType;
	static int orientationComponentType;

public:
	static void componentInit() {
		super::componentInit();

		thisComponentType = getComponentRegister()->getComponentTypeFromName(componentName_Render);
		globalTransformComponentType = getComponentRegister()->getComponentTypeFromName(componentName_GlobalTransform);
		orientationComponentType = getComponentRegister()->getComponentTypeFromName(componentName_Orientation);
	}

public:
	GRenderComponent(int type = 0)
	  : super(thisComponentType)
	{
	}

	~GRenderComponent() {
	}

	virtual int loadUpdateOrder() override;

protected:
	virtual void doUpdate(GUpdateContext * context) override;

public:

	gcolor color;

private:
};


template <typename T>
class GMotionVelocity
{
public:
	GMotionVelocity() : velocity(T(0)), accelerate(T(0)), limitVelocity(T(0)) {
	}

	inline void setVelocity(T v) {
		this->velocity = v;
	}

	inline T getVelocity() const {
		return this->velocity;
	}

	inline void setAccelerate(T a) {
		this->accelerate = a;
	}

	inline T getAccelerate() const {
		return this->accelerate;
	}

	inline void setLimitVelocity(T limit) {
		this->limitVelocity = limit;
	}

	inline T getLimitVelocity() const {
		return this->limitVelocity;
	}

	inline void unlimit() {
		this->limitVelocity = std::numeric_limits<T>::quiet_NaN();
	}

	inline bool isLimited() {
		return !(this->limitVelocity != this->limitVelocity);
	}

	inline void update() {
		this->velocity += this->accelerate;

		if(this->isLimited()) {
			if(this->accelerate < 0 && this->velocity < this->limitVelocity
				|| this->accelerate > 0 && this->velocity > this->limitVelocity) {
				this->velocity = this->limitVelocity;
			}
		}
	}

private:
	T velocity;
	T accelerate;
	T limitVelocity;
};

typedef GMotionVelocity<gcoord> GMotionVelocityCoord;
typedef GMotionVelocity<gradian> GMotionVelocityRadian;


class GMotionComponent : public GComponent
{
private:
	typedef GComponent super;

private:
	static int thisComponentType;
	static int orientationComponentType;

public:
	static void componentInit() {
		super::componentInit();

		thisComponentType = getComponentRegister()->getComponentTypeFromName(componentName_Motion);
		orientationComponentType = getComponentRegister()->getComponentTypeFromName(componentName_Orientation);
	}

public:
	GMotionComponent(int type = 0)
		: super(thisComponentType)
	{
	}

	~GMotionComponent() {
	}

	inline GMotionVelocityCoord *getVelocityX() {
		return &this->velocityX;
	}

	inline GMotionVelocityCoord *getVelocityY() {
		return &this->velocityY;
	}

	inline GMotionVelocityRadian *getVelocityRotate() {
		return &this->velocityRotate;
	}

protected:
	virtual void doUpdate(GUpdateContext * context) override;

private:
	GMotionVelocityCoord velocityX;
	GMotionVelocityCoord velocityY;
	GMotionVelocityRadian velocityRotate;
};



class GTouchInputComponent : public GComponent
{
private:
	typedef GComponent super;

private:
	static int thisComponentType;
	static int orientationComponentType;

public:
	static void componentInit() {
		super::componentInit();

		thisComponentType = getComponentRegister()->getComponentTypeFromName(componentName_TouchInput);
		orientationComponentType = getComponentRegister()->getComponentTypeFromName(componentName_Orientation);
	}

public:
	GTouchInputComponent(int type = 0)
		: super(thisComponentType)
	{
	}

	virtual int loadUpdateOrder() override;

	virtual bool acceptLocation(gcoord screenX, gcoord screenY);
	virtual bool processTouchInput(int message, int key, gcoord x, gcoord y);
};



} // namespace cpgf


#endif

