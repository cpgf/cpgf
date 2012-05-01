package com.cpgf.metagen.metawriter.callback;

import com.cpgf.metagen.metadata.Item;

public interface IOutputCallback {
	void outputCallback(Item item, OutputCallbackData data);
}
