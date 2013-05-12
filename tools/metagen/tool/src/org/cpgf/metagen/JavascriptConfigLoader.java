package org.cpgf.metagen;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.util.Map;

import org.cpgf.metagen.metadata.ClassTraits;
import org.cpgf.metagen.metadata.Item;
import org.cpgf.metagen.metawriter.callback.IOutputCallback;
import org.cpgf.metagen.metawriter.callback.OutputCallbackData;
import org.mozilla.javascript.Context;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.NativeArray;
import org.mozilla.javascript.NativeObject;
import org.mozilla.javascript.Scriptable;
import org.mozilla.javascript.ScriptableObject;


public class JavascriptConfigLoader implements IOutputCallback {
    static class Shell extends ScriptableObject {

        @Override
        public String getClassName() {
            return "global";
        }

        public static void print(Context cx, Scriptable thisObj, Object[] args, Function funObj) {
            for (int i = 0; i < args.length; i++) {
                String s = Context.toString(args[i]);
                System.out.print(s);
            }
        }
    }

	private static Context context;
	private static Scriptable scope;
	private Config config;
	
	private Function jsOutputCallback;
	
	public JavascriptConfigLoader(Config config) {
		this.config = config;
	}

	@Override
	public void outputCallback(Item item, OutputCallbackData data) {
		if(this.jsOutputCallback != null) {
			Object[] args = { item, data };
			this.jsOutputCallback.call(context, scope, scope, args);
		}
	}

	public void load(String configFileName) throws Exception {
		String javascriptCode = Util.readTextFromFile(configFileName);
        
		if(context == null) {
			context = Context.enter();
		}
		if(scope == null) {
  			Shell shell = new Shell();
  			String[] names = {"print"};
  			shell.defineFunctionProperties(names, Shell.class, ScriptableObject.DONTENUM);
 			scope = context.initStandardObjects(shell);
		}

        context.evaluateString(scope, javascriptCode, configFileName, 1, null);
        
        Object configObject = scope.get("config", scope);
        if(configObject == null || configObject == Scriptable.NOT_FOUND) {
        	this.error("Can't find config object in " + configFileName);
        }
        if(! (configObject instanceof NativeObject)) {
        	this.error("config must be Javascript object in " + configFileName);
        }
        
        this.loadConfig((NativeObject)configObject);
	}
	
	public void free() {
		if(context != null) {
			Context.exit();
			context = null;
		}
	}
	
	private void error(String message) throws Exception {
		Util.error("Config error -- " + message);
	}
	
	private void loadConfig(NativeObject jsObject) throws Exception {
		for(Map.Entry<Object, Object> entry : jsObject.entrySet()) {
			this.loadProperty(entry.getKey(), entry.getValue());
		}
	}
	
	private void loadProperty(Object key, Object value) throws Exception {
		if(! (key instanceof String)) {
			this.error("Unknown config property type " + key);
		}
		
		String propertyName = (String)key;
		Field field = null;
		try {
			field = Config.class.getField(propertyName);
		}
		catch(NoSuchFieldException e) {
			this.error("Can't find config property for " + propertyName);
		}
		
		if(this.setField(field, String.class, propertyName, value)) {
			return;
		}

		if(this.setField(field, boolean.class, propertyName, value)) {
			return;
		}

		if(checkArrayField(field, propertyName, value)) {
			return;
		}

		if(checkCallbackField(field, propertyName, value)) {
			return;
		}

		if(checkSpecialField(field, propertyName, value)) {
			return;
		}

		this.error("Unknow value type for property " + propertyName);
	}

	private boolean checkSpecialField(Field field, String propertyName, Object value) throws Exception {
		if(propertyName.equals("classTraits")) {
			this.loadClassTraits(field, propertyName, value);
			return true;
		}
		
		return false;
	}
	
	private void loadClassTraits(Field field, String propertyName, Object value) throws Exception {
		if(! (value instanceof NativeArray)) {
			this.error("Property " + propertyName + " must be an array.");
		}
		
		NativeArray na = (NativeArray)(value);
		Object[] array = na.toArray();
		for(Object item : array) {
			if(! (item instanceof NativeObject)) {
				this.error("Elements of " + propertyName + " must be object.");
			}

			String pattern = null;
			ClassTraits traits = null;
			
			for(Map.Entry<Object, Object> entry : ((NativeObject)item).entrySet()) {
				String key = (String)(entry.getKey());
				Object propertyValue = entry.getValue();
				
				if(key.equals("pattern")) {
					pattern = (String)propertyValue;
				}
				else if(key.equals("traits")) {
					if(! (propertyValue instanceof NativeObject)) {
						this.error("Traits must be object.");
					}
					if(traits == null) {
						traits = new ClassTraits();
					}
					for(Map.Entry<Object, Object> traitsEntry : ((NativeObject)propertyValue).entrySet()) {
						String traitsKey = (String)(traitsEntry.getKey());
						Boolean traitsValue = (Boolean)(traitsEntry.getValue());
						ClassTraits.class.getMethod("set" + Util.upcaseFirst(traitsKey), boolean.class).invoke(traits, traitsValue);
					}
				}
			}
			
			if(pattern == null) {
				this.error("Traits must have pattern.");
			}
			
			this.config.classTraits.addTraits(pattern, traits);
		}
	}
	
	private boolean checkArrayField(Field field, String propertyName, Object value) throws Exception {
		if(field.getType().isArray()) {
			if(! (value instanceof NativeArray)) {
				this.error("Property " + propertyName + " must be an array.");
			}
			
			NativeArray na = (NativeArray)(value);
			int count = (int)na.getLength();
			Object[] array = na.toArray();
			Object obj = Array.newInstance(field.getType().getComponentType(), count);
			for(int i = 0; i < count; ++i) {
				Array.set(obj, i, array[i]);
			}
			field.set(this.config, obj);
			
			return true;
		}
		
		return false;
	}

	private boolean checkCallbackField(Field field, String propertyName, Object value) throws Exception {
		if(field.getType().equals(IOutputCallback.class)) {
			if(value != null) {
				if(! (value instanceof Function)) {
					this.error("Property " + propertyName + " must be a function.");
				}
				
				this.jsOutputCallback = (Function)value;
				this.config.metaItemCallback = this;
			}
			return true;
		}

		return false;
	}

	private static Class<?>[][] compatibleTypes = {
		{ boolean.class, Boolean.class }
	};
	
	private static boolean typeIsIn(Class<?> type, Class<?>[] typeList) {
		for(Class<?> t : typeList) {
			if(t.equals(type)) {
				return true;
			}
		}
		
		return false;
	}
	
	private boolean isConvertable(Class<?> javaType, Class<?> jsType) {
		if(javaType.equals(jsType)) {
			return true;
		}
		
		for(Class<?>[] typeList : compatibleTypes) {
			if(typeIsIn(javaType, typeList)) {
				return typeIsIn(jsType, typeList);
			}
		}
		
		return false;
	}
	
	private boolean setField(Field field, Class<?> type, String propertyName, Object value) throws Exception {
		if(field.getType().equals(type)) {
			if(value != null && ! isConvertable(value.getClass(), type)) {
				this.error("Value for " + propertyName + " is wrong.");
			}
			
			field.set(this.config, value);
			return true;
		}
		else {
			return false;
		}
	}

}
