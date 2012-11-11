package org.cpgf.metagen.metawriter.callback;

import org.cpgf.metagen.metadata.Item;

public interface IOutputCallback {
	void outputCallback(Item item, OutputCallbackData data);
}
