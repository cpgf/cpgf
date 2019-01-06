# cpgf tween library

cpgf tween and timeline library is a powerful tweening engine written in pure C++. A tween interpolates property values overtime in-between a certain duration. A timeline manages a sequence of tweens or nested timelines.

We can do lots of stuff with cpgf tween library, such as, but not limit to,
  * Create tween animation in game or media software, similar as we did in Adobe Flash with ActionScript.
  * Create visual user interface effect in application software, similar as the drag and drop effect in IOS and Android devices.
  * Delay call a function after a certain duration.

## Main features

  * Support many features that exist in some other widely used Flash tweening engines such as TweenMax. cpgf tween library supports repeat, yoyo, delay, repeat delay, backward, use frames, time scale, dynamical destination value (follow), and a lot of callbacks on initialization or complete.
  * Support many easing functions. Now supports back, bounce, circle, cubic, elastic, exponent, linear, quad, quart, quint (strong), and sine. Adding new easing functions is a piece of cake!
  * Support unlimited property types to tween on. With the powerful generic template programming, any types can be tweened as long as the type supports operators T + T, T - T, and T * float.
  * Support tweening getter and setter functions. With the powerful cpgf accessor library, we can tween a property via either direct address access, or getter and setter functions.
  * Flexibility and extensibility. Not only can we add any new easing functions and tween different property types, but also we can tween a bunch of properties in CPU data cache friendly manner.
  * Support both tween and timeline. A timeline is a "super" tween which can contains other tweens and timelines.

<WRAP tip>
To learn from real projects using or samples, check my 2D game engine that uses cpgf tweening library a lot.

[]()
</WRAP>


The code snippet below shows how a tween looks like. A tween moves a sprite to new position x=100, y=200, with elastic ease, in 2 time units.
```c++

// Define a sprite somewhere.
FooSprite mySprite;

// Setup the tween
GTween & tween = GTweenList::getInstance()->createTween()
  .duration(2.0f)
  .ease(ElasticEase::easeIn())
  
  // We can use getter/setter functions
  .target(createAccessor(&mySprite, &FooSprite::getX, &FooSprite::setX), 100)
  
  // We can also use property address directly, as long as the property is public.
  .target(createAccessor(&mySprite, &FooSprite::y, &FooSprite::y), 200)
;

// The main tick function
void gameLoop(float frameDuration)
{
  GTweenList::getInstance()->tick(frameDuration);
}
```

Below animation shows how cpgf tween library works. A teapot moves with bounce-out ease, repeatedly, in 2 seconds.
<img src="images/cpgf-tween-animation.gif">

Below animation shows how timeline works. Three teapots moves in different ease animation, one by one.
<img src="images/cpgf-timeline-animation.gif">

## Time unit in cpgf tween library

cpgf tween library supports two kinds of timing mode
  * One mode is frame based (useFrames(true)). In each tick the time is forwarded by 1. So a duration 10 will be finished in 10 frames, no matter what the frame rate is.
  * The other mode is time based (useFrames(false)). In each tick the time is forwarded by frameDuration which passed in. Unlike a lot of other tweening engines, cpgf tween library doesn't define the time unit. It all depends on how we use it. If we use seconds as duration, we should tick with seconds. If we use milliseconds as duration, so does we call tick().

**Canveat**: if the frameDuration is 0, the timeline will start any pending tweens immediately and cause weird behavior. So only use positive number as frameDuration. If your game frame rate is over 1000 that cause the frame duration is under 1 millisecond, you should use microsecond or even nanosecond which is never goes to zero.


## Common types, methods and parameters in both GTween and GTimeline

Class GTween and GTimeline share a lot of types, methods and parameters in common. Below lists the methods and parameters. Note either tween or timeline is also called a tweenable.

GTween is in header "cpgf/tween/gtween.h", GTimeline is in header "cpgf/tween/gtimeline.h". And they are in namespace "cpgf".

Types
```c++

typedef float GTweenNumber;
```
The type is used to store and calculate timing duration.
```c++

typedef GCallback<void ()> GTweenCallback;
```
The callback type.

Parameter setting functions
```c++

GTween(Or GTimeline) & backward(bool value);
```
Default is false. If the value is true, the tweenable will start from end to beginning. The function returns a reference to current object. So we can write chain functions as we did in above sample code.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```
Default is false. If the value is true, the timing mode will be frame based. If the tweenable is in a master timeline, this value is always overwritten by the master timeline.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```
Default is 0. Set the delay time. The tweenable will start after the value of time is elapsed.
```c++

GTween(Or GTimeline) & timeScale(GTweenNumber value);
```
Default is 1. This value is multiplied to frame duration to change the tweenable speed. 1 is the normal speed, 0.5 is half speed, 2.0 is double speed, etc.
```c++

GTween(Or GTimeline) & repeat(int value);
```
Default is 0. Set the number of times that the tweenable should repeat. -1 or tweenRepeatInfinitely repeats infinitely, 0 or tweenNoRepeat doesn't repeat.
```c++

GTween(Or GTimeline) & repeatDelay(GTweenNumber value);
```
Default is 0. Set the delay time between repeats.
```c++

GTween(Or GTimeline) & yoyo(bool value);
```
Default is false. If the value is true, the tweenable will go back and forth, appearing to reverse every other cycle.
```c++

typedef float GTweenNumber;
```0
Set the callback which is called when the tweenable initializes. A tweenable will initialize after the delay time is elapsed and before the first tick.
```c++

typedef float GTweenNumber;
```1
Set the callback which is called when the tweenable completes.
```c++

typedef float GTweenNumber;
```2
Set the callback which is called when the tweenable object is going to be freed.
```c++

typedef float GTweenNumber;
```3
Set the callback which is called when the tweenable applied the new value in each tick.
```c++

typedef float GTweenNumber;
```4
Set the callback which is called when the tweenable is going to repeat.

Status query functions
```c++

typedef float GTweenNumber;
```5

Functions
```c++

typedef float GTweenNumber;
```6
Step forward the time.
```c++

typedef float GTweenNumber;
```7
Pause the tweenable.
```c++

typedef float GTweenNumber;
```8
Resume the tweenable.
```c++

typedef float GTweenNumber;
```9
Tick the tweenable immediately. This is usually called after the tweenable is setup and before starting. Calling this function will set the properties to the start value.
```c++

typedef GCallback<void ()> GTweenCallback;
```0
Restart the tweenable. Any delay time is not included. So the tweenable will start without any delay time.
```c++

typedef GCallback<void ()> GTweenCallback;
```1
Restart the tweenable and include any delay time.
```c++

typedef GCallback<void ()> GTweenCallback;
```2
Get current elapsed time, not including delay time or any repeat.
```c++

typedef GCallback<void ()> GTweenCallback;
```3
Set current elapsed time, not including delay time or any repeat. In next tick, the tweenable will update from the new time.
```c++

typedef GCallback<void ()> GTweenCallback;
```4
Get overall elapsed time, not including delay time, but include repeat and repeat delay time.  
If the tweenable is repeating infinitely, the return value is meaningless.
```c++

typedef GCallback<void ()> GTweenCallback;
```5
Set overall elapsed time, not including delay time, but include repeat and repeat delay time.  
If the tweenable is repeating infinitely, setting the total time is undefined behavor.
```c++

typedef GCallback<void ()> GTweenCallback;
```6
Get current elapsed progress, not including delay time or any repeat. This is same as  *return getCurrentTime() / getDuration();*  
The return value is always between [0, 1].
```c++

typedef GCallback<void ()> GTweenCallback;
```7
Set current elapsed progress, not including delay time or any repeat. This is same as  *setCurrentTime(value * getDuration());*  
The value should be always between [0, 1]. If the value is smaller than 0, 0 will be used. If the value is larger than 1, 1 will be used.
```c++

typedef GCallback<void ()> GTweenCallback;
```8
Get overall elapsed progress, not including delay time, but include repeat and repeat delay time. This is same as  *return getTotalTime() / getTotalDuration();*  
The return value is always between [0, 1].  
If the tweenable is repeating infinitely, the return value is meaningless.
```c++

typedef GCallback<void ()> GTweenCallback;
```9
Set overall elapsed progress, not including delay time, but include repeat and repeat delay time. This is same as  *setTotalTime()(value * getTotalDuration());//  
The value should be always between [0, 1]. If the value is smaller than 0, 0 will be used. If the value is larger than 1, 1 will be used.  
If the tweenable is repeating infinitely, setting the total progress is undefined behavor.
```c++

GTween(Or GTimeline) & backward(bool value);
```0
Get the duration of the tweenable. The duration doesn't include any delay or repeat.
```c++

GTween(Or GTimeline) & backward(bool value);
```1
Get the total duration of the tweenable. The total duration doesn't include any delay, but include repeat and repeat delay time.  
If the tweenable is repeating infinitely, the return value is meaningless.
```c++

GTween(Or GTimeline) & backward(bool value);
```2
Remove all properties which object equals to the instance.

## GTween special methods and parameters

Parameter setting functions
```c++

GTween(Or GTimeline) & backward(bool value);
```3
Set the ease function. Default is LinearEase::ease().
```c++

GTween(Or GTimeline) & backward(bool value);
```4
Set the duration time. The duration doesn't include any delay time or repeat delay.

Functions
```c++

GTween(Or GTimeline) & backward(bool value);
```5
Add a property to tween on.  
accessor: An accessor that the tween uses to get and set value. We can call global function createAccessor to create the accessor. Please reference to the accessor documentation for details. In the second overload function, since we specified the "from" value, the accessor could only have setter and without getter.  
targetValue: The target value. The property value will be tweened to the target value.  
from: Specify the beginning value. Otherwise with the first function, GTween will use the property value at the time when the tween starts.
```c++

GTween(Or GTimeline) & backward(bool value);
```6
Similar as the "target" functions. The "relative" functions add a property to tween on. Instead of setting any target value, "relative" sets relative value, which will be added to the property value when the tween starts.
```c++

GTween(Or GTimeline) & backward(bool value);
```7
Similar as the "target" functions, the "follow" functions add a property to tween on. The target value will be retrieved from targetGetter dynamically. We can use createGetter to create the target getter.

## GTimeline special methods
```c++

GTween(Or GTimeline) & backward(bool value);
```8
Create a tween. The timeline will own the new created tween. The tween is not on the timeline until functions append, prepend, insert, or setAt are called.
```c++

GTween(Or GTimeline) & backward(bool value);
```9
Create a timeline. The timeline will own the new created timeline. The new created timeline is not on the timeline until functions append, prepend, insert, or setAt are called.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```0
Append the tweenable to the timeline. If there is already 8 time units in the timeline, tweenable will start from 8th time units.  
The function returns the start time of the tweenable.  
Note the tweenable must be owned by the timeline, or to say, must be created by tween() or timeline() of the timeline.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```1
Insert the tweenable to the beginning of the timeline. The tweenable will always start from 0th time unit. All other tweenables on the timeline will be pushed back.  
Note the tweenable must be owned by the timeline, or to say, must be created by tween() or timeline() of the timeline.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```2
Insert the tweenable at the "time" of the timeline. The tweenable will always start from "time" time unit. All other tweenables which start time is later than or equal to "time" on the timeline will be pushed back.  
Note the tweenable must be owned by the timeline, or to say, must be created by tween() or timeline() of the timeline.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```3
Put the tweenable at the "time" of the timeline. The tweenable will always start from "time" time unit. All other tweenables are not affected.  
Note the tweenable must be owned by the timeline, or to say, must be created by tween() or timeline() of the timeline.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```4
Get the start time of a tweenable.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```5
Get tweenable count.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```6
Clear all tweenables.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```7
Remove tweenable from the tween list.
```c++

GTween(Or GTimeline) & useFrames(bool value);
```8
Remove all properties which object equals to the instance.

## Use GTweenList

Though we can create objects of GTween and GTimeline and tick them, a better way is to use GTweenList. We can get several benefits by using GTweenList,
  * GTweenList allocates GTween and GTimeline on a memory pool, which is better performace and less memory fragements.
  * GTweenList frees GTween and GTimeline automatically after they are completed.

To use GTweenList, we can create new instance of GTweenList, also we can use the global GTweenList instance by calling GTweenList::getInstance().
```c++

GTween(Or GTimeline) & useFrames(bool value);
```9
Get the global singleton instance of GTweenList.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```0
Create a tween and return the reference. Note either GTween or GTimeline is not copyable.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```1
Create a timeline and return the reference.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```2
Step forward the time.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```3
Get tweenable count.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```4
Clear all tweenables.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```5
Remove tweenable from the tween list.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```6
Remove all properties which object equals to the instance.

## Drive the tween

To be most platform independent, cpgf tween library doesn't update each tweens or timelines. It's up to the user to call the function "tick" on GTween, GTimeline, or GTweenList.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```7

**Canveat**: if the frameDuration is 0, the timeline will start any pending tweens immediately and cause weird behavior. So only use positive number as frameDuration. If your game frame rate is over 1000 that cause the frame duration is under 1 millisecond, you should use microsecond or even nanosecond which is never goes to zero.


## Tweening large amount of properties at once in CPU data cache friendly manner

Nowadays CPU data cache friendly is one of the most important optimization skill in high performance software such as console game. It's not rare that we may want to tween thousands of object on the screen in a game. Creating thousands of tweens, and each tween moves only one object, sounds work. But that may cause a lot of data cache misses and cause very bad performance.

Fortunately, with the great extensibility, we can create only one tween and change all objects values in very CPU data cache friendly manner.

Now let's see how this magic works. Let's create one and only one tween first.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```8
The differences with other tweens are,
  * We set the getter as 0 to the accessor. That means the accessor can't get value from the property. That's fine because we passed the "from" value (0.0f).
  * The "from" value is always 0.0f and the "target" value is always 1.0f.

Now let's see how MyClass::setRatio works.
```c++

GTween(Or GTimeline) & delay(GTweenNumber value);
```9

Now we can see the function setRatio receives a parameter of ratio. The tween "thinks" setRatio is a normal setter function and passes the value to it, but since we set the "from" value to 0.0f and "target" value to 1.0f, the value passed from the tween is the ratio between 0 and 1.
Thus we only need to multiply "ratio" to our data cache friendly data structure. For example, if we have a vector that holds a list of positions, we simple multiply ratio with each elements in the vector.
```c++

GTween(Or GTimeline) & timeScale(GTweenNumber value);
```0

In the tween sample application, there is test case in testcaseparticles.cpp shows how this works.

## Use the sample application

There is a "tween" folder in cpgf sample directory, which is a comprehensive demonstration of how cpgf tween library works. The sample is a wxWidgets GUI application, and render in OpenGL. It also uses freeglut to draw the teapot.

Below image shows the charts of all ease function in the tween library, rendered by the sample application.
<img src="images/cpgf-tween-all-ease-chart.jpg">
