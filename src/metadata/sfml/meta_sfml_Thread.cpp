// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/System/Thread.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Thread.h"
#include "cpgf/scriptbind/gscriptbindutil.h"

#include <iostream>


using namespace std;
using namespace cpgf;

namespace meta_sfml { 


static IScriptFunction * xxx = NULL;
void callback_Thread_Constructor_0_p0(void * param)
{
    invokeScriptFunction(xxx, param);
}

void * callbackWrapper_Thread_Constructor_0(cpgf::IScriptFunction * scriptFunction0, void * UserData)
{
	return new sf::Thread(NULL);
	cout << "AAAAAAAAAa   " << scriptFunction0 << endl;
	xxx = scriptFunction0; 
	return new sf::Thread(&callback_Thread_Constructor_0_p0);
}



GDefineMetaInfo createMetaClass_Thread()
{
    GDefineMetaClass<sf::Thread> _d = GDefineMetaClass<sf::Thread>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("Thread");
    buildMetaClass_Thread(0, _d, NULL);
    return _d.getMetaInfo();
}


} // namespace meta_sfml


