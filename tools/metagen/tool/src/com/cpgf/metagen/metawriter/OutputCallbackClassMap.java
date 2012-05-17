package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.Item;
import com.cpgf.metagen.metawriter.callback.OutputCallbackData;

public class OutputCallbackClassMap {
	private Config config;
	private OutputCallbackData callbackData;
	private Map<Item, OutputCallbackData> map;
	
	public OutputCallbackClassMap(Config config) {
		this.config = config;
		this.callbackData = new OutputCallbackData();
		this.map = new HashMap<Item, OutputCallbackData>();
	}
	
	public void build(List<CppClass> classList) {
		if(this.config.metaOutputCallback == null) {
			return;
		}

		List<Item> itemList = new ArrayList<Item>();

		itemList.addAll(classList);
		for(CppClass cppClass : classList) {
			cppClass.getAllItems(itemList);
		}
		
		for(Item item : itemList) {
			OutputCallbackData data = new OutputCallbackData();
			this.config.metaOutputCallback.outputCallback(item, data);
			this.map.put(item, data);
		}
	}

	public OutputCallbackData getData(Item item) {
		if(this.config.metaOutputCallback == null) {
			return this.callbackData;
		}
		else {
			return this.map.get(item);
		}
	}
}
