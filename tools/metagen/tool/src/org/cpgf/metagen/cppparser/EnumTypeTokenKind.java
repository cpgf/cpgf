package org.cpgf.metagen.cppparser;

public enum EnumTypeTokenKind {
	Pointer, // *
	RValueReference, // &&
	Reference, // &
	LeftAngle, // <
	RightAngle, // >
	LeftSquare, // [
	RightSquare, // ]
	LeftParenthesis, // (
	RightParenthesis, // )
	Comma, // ,
	Const, // const
	Volatile, // volatile
	Symbol, // int, char, MyClass
}
