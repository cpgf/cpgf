package com.cpgf.metagen.cppparser;

public enum EnumTypeTokenKind {
	Pointer, // *
	RValueRerference, // &&
	Reference, // &
	LeftAngle, // <
	RightAngle, // >
	Const, // const
	Volatile, // volatile
	Symbol, // int, char, MyClass
}
