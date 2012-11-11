package org.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.List;

public class CppEnum extends Item {
	private List<EnumValue> valueList;

	public CppEnum(String name) {
		super(EnumCategory.Enum, name);
		
		this.valueList = new ArrayList<EnumValue>();
	}

	public List<EnumValue> getValueList() {
		return valueList;
	}

	public void addValue(String name, String value) {
		valueList.add(new EnumValue(this, name, value));
	}

}
