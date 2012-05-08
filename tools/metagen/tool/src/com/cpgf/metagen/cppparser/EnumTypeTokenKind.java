package com.cpgf.metagen.cppparser;

public enum EnumTypeTokenKind {
	Pointer, // *
	RValueReference, // &&
	Reference, // &
	LeftAngle, // <
	RightAngle, // >
	Comma, // ,
	Const, // const
	Volatile, // volatile
	Symbol, // int, char, MyClass
}
