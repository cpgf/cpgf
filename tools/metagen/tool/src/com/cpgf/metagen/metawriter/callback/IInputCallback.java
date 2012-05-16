package com.cpgf.metagen.metawriter.callback;

import com.cpgf.metagen.metadata.CppClass;

public interface IInputCallback {
	void inputCallback(CppClass cppClass, InputCallbackData data);
}
