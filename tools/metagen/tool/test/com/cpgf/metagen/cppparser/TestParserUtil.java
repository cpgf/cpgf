package com.cpgf.metagen.cppparser;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.List;

import org.cpgf.metagen.Util;
import org.cpgf.metagen.cppparser.EnumTypeTokenKind;
import org.cpgf.metagen.cppparser.ParserUtil;
import org.cpgf.metagen.cppparser.TypeToken;
import org.junit.Test;


public class TestParserUtil {

	@Test
	public void testSplitTypeTokenLiterals() {
		List<String> tokenList;

		tokenList = ParserUtil.splitTypeTokenLiterals("");
		assertEquals(tokenList.size(), 0);

		tokenList = ParserUtil.splitTypeTokenLiterals("int");
		assertEquals(tokenList.size(), 1);
		assertEquals(tokenList.get(0), "int");

		tokenList = ParserUtil.splitTypeTokenLiterals("  &  ");
		assertEquals(tokenList.size(), 1);
		assertEquals(tokenList.get(0), "&");

		tokenList = ParserUtil.splitTypeTokenLiterals("  int  &  ");
		assertEquals(tokenList.size(), 2);
		assertEquals(tokenList.get(0), "int");
		assertEquals(tokenList.get(1), "&");
	}

	@Test
	public void testSplitTypeTokens() {
		List<TypeToken> tokenList;
		
		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals(""));
		assertEquals(tokenList.size(), 0);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int"));
		assertEquals(tokenList.size(), 1);
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Symbol);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int const*volatile& <>"));
		assertEquals(tokenList.size(), 7);
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Symbol);
		assertEquals(tokenList.get(1).getKind(), EnumTypeTokenKind.Const);
		assertEquals(tokenList.get(2).getKind(), EnumTypeTokenKind.Pointer);
		assertEquals(tokenList.get(3).getKind(), EnumTypeTokenKind.Volatile);
		assertEquals(tokenList.get(4).getKind(), EnumTypeTokenKind.Reference);
		assertEquals(tokenList.get(5).getKind(), EnumTypeTokenKind.LeftAngle);
		assertEquals(tokenList.get(6).getKind(), EnumTypeTokenKind.RightAngle);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int & && & &"));
		assertEquals(tokenList.size(), 5);
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Symbol);
		assertEquals(tokenList.get(1).getKind(), EnumTypeTokenKind.Reference);
		assertEquals(tokenList.get(2).getKind(), EnumTypeTokenKind.RValueReference);
		assertEquals(tokenList.get(3).getKind(), EnumTypeTokenKind.Reference);
		assertEquals(tokenList.get(4).getKind(), EnumTypeTokenKind.Reference);
	}

	@Test
	public void testSwapListItems() {
		List<String> list = new ArrayList<String>();
		
		list.add("a");
		list.add("b");
		list.add("c");
		list.add("d");
		list.add("e");
		
		assertEquals(list.get(0), "a");
		assertEquals(list.get(1), "b");
		assertEquals(list.get(2), "c");
		assertEquals(list.get(3), "d");
		assertEquals(list.get(4), "e");

		Util.swapListItems(list, 0, 1);
		assertEquals(list.get(0), "b");
		assertEquals(list.get(1), "a");
		assertEquals(list.get(2), "c");
		assertEquals(list.get(3), "d");
		assertEquals(list.get(4), "e");

		Util.swapListItems(list, 0, 4);
		assertEquals(list.get(0), "e");
		assertEquals(list.get(1), "a");
		assertEquals(list.get(2), "c");
		assertEquals(list.get(3), "d");
		assertEquals(list.get(4), "b");
	}

	@Test
	public void testNormalizeConstVolatile() {
		List<TypeToken> tokenList;

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals(""));
		assertFalse(ParserUtil.normalizeConstVolatile(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("const int"));
		assertFalse(ParserUtil.normalizeConstVolatile(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int * const"));
		assertFalse(ParserUtil.normalizeConstVolatile(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int const *"));
		assertTrue(ParserUtil.normalizeConstVolatile(tokenList));
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Const);
		assertEquals(tokenList.get(1).getKind(), EnumTypeTokenKind.Symbol);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("const const int"));
		assertTrue(ParserUtil.normalizeConstVolatile(tokenList));
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Const);
		assertEquals(tokenList.get(1).getKind(), EnumTypeTokenKind.Const);
		assertEquals(tokenList.get(2).getKind(), EnumTypeTokenKind.Symbol);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int <<>> const *"));
		assertTrue(ParserUtil.normalizeConstVolatile(tokenList));
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Const);
		assertEquals(tokenList.get(1).getKind(), EnumTypeTokenKind.Symbol);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int <long * &> const *"));
		assertTrue(ParserUtil.normalizeConstVolatile(tokenList));
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Const);
		assertEquals(tokenList.get(1).getKind(), EnumTypeTokenKind.Symbol);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int volatile <long * &> const *"));
		assertTrue(ParserUtil.normalizeConstVolatile(tokenList));
		assertEquals(tokenList.get(0).getKind(), EnumTypeTokenKind.Const);
		assertEquals(tokenList.get(1).getKind(), EnumTypeTokenKind.Volatile);
		assertEquals(tokenList.get(2).getKind(), EnumTypeTokenKind.Symbol);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int <const long>"));
		assertFalse(ParserUtil.normalizeConstVolatile(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int <long const * &>*"));
		assertTrue(ParserUtil.normalizeConstVolatile(tokenList));
		assertEquals(tokenList.get(2).getKind(), EnumTypeTokenKind.Const);

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("int <long * const>"));
		assertFalse(ParserUtil.normalizeConstVolatile(tokenList));

	}
	
	@Test
	public void testNormalizeSignedAndUnsigned() {
		List<TypeToken> tokenList;

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals(""));
		assertFalse(ParserUtil.normalizeSignedAndUnsigned(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("signed  int"));
		assertFalse(ParserUtil.normalizeSignedAndUnsigned(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("unsigned  int"));
		assertFalse(ParserUtil.normalizeSignedAndUnsigned(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("signed"));
		assertTrue(ParserUtil.normalizeSignedAndUnsigned(tokenList));
		assertEquals(tokenList.size(), 2);
		assertEquals(tokenList.get(1).getToken(), "int");

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("unsigned *"));
		assertTrue(ParserUtil.normalizeSignedAndUnsigned(tokenList));
		assertEquals(tokenList.size(), 3);
		assertEquals(tokenList.get(1).getToken(), "int");
	}

	@Test
	public void testNormalizeShortInt() {
		List<TypeToken> tokenList;

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals(""));
		assertFalse(ParserUtil.normalizeShortInt(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("short  int"));
		assertFalse(ParserUtil.normalizeShortInt(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("short"));
		assertTrue(ParserUtil.normalizeShortInt(tokenList));
		assertEquals(tokenList.size(), 2);
		assertEquals(tokenList.get(1).getToken(), "int");

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("short *"));
		assertTrue(ParserUtil.normalizeShortInt(tokenList));
		assertEquals(tokenList.size(), 3);
		assertEquals(tokenList.get(1).getToken(), "int");
	}

	@Test
	public void testNormalizeUnusedKeyword() {
		List<TypeToken> tokenList;

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals(""));
		assertFalse(ParserUtil.normalizeUnusedKeyword(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("_class"));
		assertFalse(ParserUtil.normalizeUnusedKeyword(tokenList));

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("struct abc"));
		assertTrue(ParserUtil.normalizeUnusedKeyword(tokenList));
		assertEquals(tokenList.size(), 1);
		assertEquals(tokenList.get(0).getToken(), "abc");

		tokenList = ParserUtil.splitTypeTokens(ParserUtil.splitTypeTokenLiterals("my enum def"));
		assertTrue(ParserUtil.normalizeUnusedKeyword(tokenList));
		assertEquals(tokenList.size(), 2);
		assertEquals(tokenList.get(1).getToken(), "def");
	}

	@Test
	public void testSplitDelimitedString() {
		char delimiter = ',';
		List<String> tokenList;
		int endIndex;
		String s;
		
		tokenList = new ArrayList<String>();
		s = "";
		endIndex = ParserUtil.splitDelimitedString(tokenList, s, delimiter, 0);
		assertEquals(tokenList.size(), 0);
		assertEquals(endIndex, s.length());
		
		tokenList = new ArrayList<String>();
		s = "abc  , def";
		endIndex = ParserUtil.splitDelimitedString(tokenList, s, delimiter, 0);
		assertEquals(tokenList.size(), 2);
		assertEquals(endIndex, s.length());
		assertEquals(tokenList.get(0), "abc");
		assertEquals(tokenList.get(1), "def");
		
		tokenList = new ArrayList<String>();
		s = "abc <x, y, z>  , def";
		endIndex = ParserUtil.splitDelimitedString(tokenList, s, delimiter, 0);
		assertEquals(tokenList.size(), 2);
		assertEquals(endIndex, s.length());
		assertEquals(tokenList.get(0), "abc <x, y, z>");
		assertEquals(tokenList.get(1), "def");
		
		tokenList = new ArrayList<String>();
		s = "abc <x, y, z>  , def }";
		endIndex = ParserUtil.splitDelimitedString(tokenList, s, delimiter, 0);
		assertEquals(tokenList.size(), 2);
		assertEquals(endIndex, s.length() - 1);
		assertEquals(tokenList.get(0), "abc <x, y, z>");
		assertEquals(tokenList.get(1), "def");
	}
	
	@Test
	public void testGetLastWord() {
		assertEquals(ParserUtil.getLastWord(""), "");
		assertEquals(ParserUtil.getLastWord(" abc "), "abc");
		assertEquals(ParserUtil.getLastWord("int def "), "def");
	}
	
}
