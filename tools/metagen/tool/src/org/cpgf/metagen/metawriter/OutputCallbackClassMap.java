package org.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.doxyxmlparser.FileInfo;
import org.cpgf.metagen.doxyxmlparser.FileMap;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.Item;
import org.cpgf.metagen.metawriter.callback.OutputCallbackData;


public class OutputCallbackClassMap {
	private Config config;
	private OutputCallbackData callbackData;
	private Map<Item, OutputCallbackData> map;
	
	public OutputCallbackClassMap(Config config) {
		this.config = config;
		this.callbackData = new OutputCallbackData();
		this.map = new HashMap<Item, OutputCallbackData>();
	}
	
	public void build(List<CppClass> classList, FileMap fileMap) {
		if(this.config.metaItemCallback == null) {
			return;
		}

		List<Item> itemList = new ArrayList<Item>();

		itemList.addAll(classList);
		for(CppClass cppClass : classList) {
			cppClass.getAllItems(itemList);
		}
		
		for(Item item : itemList) {
			String location = item.getLocation();

			OutputCallbackData data = new OutputCallbackData();
			this.config.metaItemCallback.outputCallback(item, data);
			this.map.put(item, data);

			String newLocation = item.getLocation();

			// If the location is changed, we must add all namespaces to the new location. 
			if(! location.equals(newLocation)) {
				FileInfo fileInfo = fileMap.getFileMap().get(location);
				FileInfo newFileInfo = fileMap.getFileMap().get(newLocation);
				if(newFileInfo != null && fileInfo != null) {
					newFileInfo.appendNamespaces(fileInfo.getNamespaceList());
				}
			}
		}
	}

	public OutputCallbackData getData(Item item) {
		if(this.config.metaItemCallback == null) {
			return this.callbackData;
		}
		else {
			return this.map.get(item);
		}
	}
}
