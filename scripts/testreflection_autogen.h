class TestObject0 {
public:
	signed int TestFunction0(signed int * p0, signed int ** p1, signed int * const p2)
	{
		TS_ASSERT_EQUALS(*p0, 3471872);
		TS_ASSERT_EQUALS(p1[0], (signed int *)(7120384));
		TS_ASSERT_EQUALS(p1[1], (signed int *)(-811007));
		TS_ASSERT_EQUALS(p1[2], (signed int *)(2758656));
		TS_ASSERT_EQUALS(*p2, -5612543);
		return (signed int)(-1526271);
	}

	signed int ** const TestFunction1(signed int * volatile p0, signed int ** volatile p1, signed int * const volatile p2)
	{
		TS_ASSERT_EQUALS(*p0, -1735167);
		TS_ASSERT_EQUALS(p1[0], (signed int *)(-8161791));
		TS_ASSERT_EQUALS(p1[1], (signed int *)(-7385599));
		TS_ASSERT_EQUALS(p1[2], (signed int *)(5640192));
		TS_ASSERT_EQUALS(*p2, -3451391);
		return (signed int **)(-6973951);
	}

	signed int ** const volatile TestFunction2(const signed int * p0, const signed int ** p1, const signed int * const p2)
	{
		TS_ASSERT_EQUALS(*p0, -4145663);
		TS_ASSERT_EQUALS(p1[0], (const signed int *)(1352704));
		TS_ASSERT_EQUALS(p1[1], (const signed int *)(8287744));
		TS_ASSERT_EQUALS(p1[2], (const signed int *)(-6002175));
		TS_ASSERT_EQUALS(*p2, 78848);
		return (signed int **)(-1883135);
	}

	const signed int ** const TestFunction3(const signed int * volatile p0, const signed int ** volatile p1, const signed int * const volatile p2)
	{
		TS_ASSERT_EQUALS(*p0, -3941375);
		TS_ASSERT_EQUALS(p1[0], (const signed int *)(-364543));
		TS_ASSERT_EQUALS(p1[1], (const signed int *)(2025472));
		TS_ASSERT_EQUALS(p1[2], (const signed int *)(8120832));
		TS_ASSERT_EQUALS(*p2, -3864575);
		return (const signed int **)(-3168767);
	}

	const signed int ** const volatile TestFunction4(volatile signed int * p0, volatile signed int ** p1, volatile signed int * const p2)
	{
		TS_ASSERT_EQUALS(*p0, -1597951);
		TS_ASSERT_EQUALS(p1[0], (volatile signed int *)(-2242559));
		TS_ASSERT_EQUALS(p1[1], (volatile signed int *)(6086656));
		TS_ASSERT_EQUALS(p1[2], (volatile signed int *)(-5181951));
		TS_ASSERT_EQUALS(*p2, -1794047);
		return (const signed int **)(-5557247);
	}

	volatile signed int ** const TestFunction5(volatile signed int * volatile p0, volatile signed int ** volatile p1, volatile signed int * const volatile p2)
	{
		TS_ASSERT_EQUALS(*p0, 899072);
		TS_ASSERT_EQUALS(p1[0], (volatile signed int *)(2254336));
		TS_ASSERT_EQUALS(p1[1], (volatile signed int *)(-4761087));
		TS_ASSERT_EQUALS(p1[2], (volatile signed int *)(8248832));
		TS_ASSERT_EQUALS(*p2, 6852608);
		return (volatile signed int **)(-1597951);
	}

	volatile signed int ** const volatile TestFunction6(const volatile signed int * p0, const volatile signed int ** p1, const volatile signed int * const p2)
	{
		TS_ASSERT_EQUALS(*p0, 7605760);
		TS_ASSERT_EQUALS(p1[0], (const volatile signed int *)(3271168));
		TS_ASSERT_EQUALS(p1[1], (const volatile signed int *)(-5446143));
		TS_ASSERT_EQUALS(p1[2], (const volatile signed int *)(3331072));
		TS_ASSERT_EQUALS(*p2, 212992);
		return (volatile signed int **)(6465024);
	}

	const volatile signed int ** const TestFunction7(const volatile signed int * volatile p0, const volatile signed int ** volatile p1, const volatile signed int * const volatile p2)
	{
		TS_ASSERT_EQUALS(*p0, -70143);
		TS_ASSERT_EQUALS(p1[0], (const volatile signed int *)(-5045759));
		TS_ASSERT_EQUALS(p1[1], (const volatile signed int *)(-3495935));
		TS_ASSERT_EQUALS(p1[2], (const volatile signed int *)(4902912));
		TS_ASSERT_EQUALS(*p2, -1416703);
		return (const volatile signed int **)(6339072);
	}

	const volatile signed int ** const volatile TestFunction8(signed int p0, signed int p1, signed int p2)
	{
		TS_ASSERT_EQUALS(p0, -1727487);
		TS_ASSERT_EQUALS(p1, 5693440);
		TS_ASSERT_EQUALS(p2, 6915584);
		return (const volatile signed int **)(3359744);
	}

	signed int TestFunction9(signed int p0, signed int * p1, signed int ** p2)
	{
		TS_ASSERT_EQUALS(p0, 3417088);
		TS_ASSERT_EQUALS(*p1, 6699520);
		TS_ASSERT_EQUALS(p2[0], (signed int *)(-3234815));
		TS_ASSERT_EQUALS(p2[1], (signed int *)(-2626047));
		TS_ASSERT_EQUALS(p2[2], (signed int *)(-961023));
		return (signed int)(-4193791);
	}

	signed int * const TestFunction10(signed int ** const p0, signed int * volatile p1, signed int ** volatile p2)
	{
		TS_ASSERT_EQUALS(p0[0], (signed int *)(3128320));
		TS_ASSERT_EQUALS(p0[1], (signed int *)(-7903743));
		TS_ASSERT_EQUALS(p0[2], (signed int *)(-7055871));
		TS_ASSERT_EQUALS(*p1, 4529152);
		TS_ASSERT_EQUALS(p2[0], (signed int *)(4054016));
		TS_ASSERT_EQUALS(p2[1], (signed int *)(5694976));
		TS_ASSERT_EQUALS(p2[2], (signed int *)(-923647));
		return (signed int *)(5884928);
	}

	signed int * const volatile TestFunction11(signed int ** const volatile p0, const signed int * p1, const signed int ** p2)
	{
		TS_ASSERT_EQUALS(p0[0], (signed int *)(2585088));
		TS_ASSERT_EQUALS(p0[1], (signed int *)(6326272));
		TS_ASSERT_EQUALS(p0[2], (signed int *)(-2949631));
		TS_ASSERT_EQUALS(*p1, -7711743);
		TS_ASSERT_EQUALS(p2[0], (const signed int *)(3564544));
		TS_ASSERT_EQUALS(p2[1], (const signed int *)(-8214015));
		TS_ASSERT_EQUALS(p2[2], (const signed int *)(2515968));
		return (signed int *)(2935296);
	}

	gDefineMetaClass(TestObject0, GMetaRoot) {
		reflectMethod("TestFunction0", &TestObject0::TestFunction0);
		reflectMethod("TestFunction1", &TestObject0::TestFunction1);
		reflectMethod("TestFunction2", &TestObject0::TestFunction2);
		reflectMethod("TestFunction3", &TestObject0::TestFunction3);
		reflectMethod("TestFunction4", &TestObject0::TestFunction4);
		reflectMethod("TestFunction5", &TestObject0::TestFunction5);
		reflectMethod("TestFunction6", &TestObject0::TestFunction6);
		reflectMethod("TestFunction7", &TestObject0::TestFunction7);
		reflectMethod("TestFunction8", &TestObject0::TestFunction8);
		reflectMethod("TestFunction9", &TestObject0::TestFunction9);
		reflectMethod("TestFunction10", &TestObject0::TestFunction10);
		reflectMethod("TestFunction11", &TestObject0::TestFunction11);
	}
};
signed int TestFunction0(signed int * p0, signed int ** p1, signed int * const p2)
{
	TS_ASSERT_EQUALS(*p0, 3789312);
	TS_ASSERT_EQUALS(p1[0], (signed int *)(4807680));
	TS_ASSERT_EQUALS(p1[1], (signed int *)(-193023));
	TS_ASSERT_EQUALS(p1[2], (signed int *)(3184640));
	TS_ASSERT_EQUALS(*p2, 2752000);
	return (signed int)(1445376);
}

signed int ** const TestFunction1(signed int * volatile p0, signed int ** volatile p1, signed int * const volatile p2)
{
	TS_ASSERT_EQUALS(*p0, 7856640);
	TS_ASSERT_EQUALS(p1[0], (signed int *)(394752));
	TS_ASSERT_EQUALS(p1[1], (signed int *)(-1550335));
	TS_ASSERT_EQUALS(p1[2], (signed int *)(-1294335));
	TS_ASSERT_EQUALS(*p2, 5920256);
	return (signed int **)(-624639);
}

signed int ** const volatile TestFunction2(const signed int * p0, const signed int ** p1, const signed int * const p2)
{
	TS_ASSERT_EQUALS(*p0, -3565055);
	TS_ASSERT_EQUALS(p1[0], (const signed int *)(7847424));
	TS_ASSERT_EQUALS(p1[1], (const signed int *)(1704448));
	TS_ASSERT_EQUALS(p1[2], (const signed int *)(6338560));
	TS_ASSERT_EQUALS(*p2, -7934975);
	return (signed int **)(-5896191);
}

const signed int ** const TestFunction3(const signed int * volatile p0, const signed int ** volatile p1, const signed int * const volatile p2)
{
	TS_ASSERT_EQUALS(*p0, -7950335);
	TS_ASSERT_EQUALS(p1[0], (const signed int *)(-1384447));
	TS_ASSERT_EQUALS(p1[1], (const signed int *)(-2371583));
	TS_ASSERT_EQUALS(p1[2], (const signed int *)(-7728639));
	TS_ASSERT_EQUALS(*p2, -165375);
	return (const signed int **)(-3010047);
}

const signed int ** const volatile TestFunction4(volatile signed int * p0, volatile signed int ** p1, volatile signed int * const p2)
{
	TS_ASSERT_EQUALS(*p0, 4019200);
	TS_ASSERT_EQUALS(p1[0], (volatile signed int *)(-2162175));
	TS_ASSERT_EQUALS(p1[1], (volatile signed int *)(3975168));
	TS_ASSERT_EQUALS(p1[2], (volatile signed int *)(-8344575));
	TS_ASSERT_EQUALS(*p2, -2384383);
	return (const signed int **)(5213696);
}

volatile signed int ** const TestFunction5(volatile signed int * volatile p0, volatile signed int ** volatile p1, volatile signed int * const volatile p2)
{
	TS_ASSERT_EQUALS(*p0, 7435264);
	TS_ASSERT_EQUALS(p1[0], (volatile signed int *)(-7311359));
	TS_ASSERT_EQUALS(p1[1], (volatile signed int *)(8218624));
	TS_ASSERT_EQUALS(p1[2], (volatile signed int *)(7781376));
	TS_ASSERT_EQUALS(*p2, 2779136);
	return (volatile signed int **)(6595072);
}

volatile signed int ** const volatile TestFunction6(const volatile signed int * p0, const volatile signed int ** p1, const volatile signed int * const p2)
{
	TS_ASSERT_EQUALS(*p0, 6262272);
	TS_ASSERT_EQUALS(p1[0], (const volatile signed int *)(-7907327));
	TS_ASSERT_EQUALS(p1[1], (const volatile signed int *)(509952));
	TS_ASSERT_EQUALS(p1[2], (const volatile signed int *)(-3792895));
	TS_ASSERT_EQUALS(*p2, 3998720);
	return (volatile signed int **)(-5486079);
}

const volatile signed int ** const TestFunction7(const volatile signed int * volatile p0, const volatile signed int ** volatile p1, const volatile signed int * const volatile p2)
{
	TS_ASSERT_EQUALS(*p0, 4273152);
	TS_ASSERT_EQUALS(p1[0], (const volatile signed int *)(3493376));
	TS_ASSERT_EQUALS(p1[1], (const volatile signed int *)(8329216));
	TS_ASSERT_EQUALS(p1[2], (const volatile signed int *)(6489088));
	TS_ASSERT_EQUALS(*p2, 4896256);
	return (const volatile signed int **)(3540992);
}

const volatile signed int ** const volatile TestFunction8(signed int p0, signed int p1, signed int p2)
{
	TS_ASSERT_EQUALS(p0, 4436992);
	TS_ASSERT_EQUALS(p1, -4688383);
	TS_ASSERT_EQUALS(p2, 4215296);
	return (const volatile signed int **)(1069056);
}

signed int TestFunction9(signed int p0, signed int * p1, signed int ** p2)
{
	TS_ASSERT_EQUALS(p0, 6983680);
	TS_ASSERT_EQUALS(*p1, -1471487);
	TS_ASSERT_EQUALS(p2[0], (signed int *)(-4712959));
	TS_ASSERT_EQUALS(p2[1], (signed int *)(-553983));
	TS_ASSERT_EQUALS(p2[2], (signed int *)(-7333887));
	return (signed int)(1196544);
}

signed int * const TestFunction10(signed int ** const p0, signed int * volatile p1, signed int ** volatile p2)
{
	TS_ASSERT_EQUALS(p0[0], (signed int *)(2472448));
	TS_ASSERT_EQUALS(p0[1], (signed int *)(-2481663));
	TS_ASSERT_EQUALS(p0[2], (signed int *)(-167423));
	TS_ASSERT_EQUALS(*p1, -1618431);
	TS_ASSERT_EQUALS(p2[0], (signed int *)(1527296));
	TS_ASSERT_EQUALS(p2[1], (signed int *)(-3613183));
	TS_ASSERT_EQUALS(p2[2], (signed int *)(3446272));
	return (signed int *)(1846784);
}

signed int * const volatile TestFunction11(signed int ** const volatile p0, const signed int * p1, const signed int ** p2)
{
	TS_ASSERT_EQUALS(p0[0], (signed int *)(-7089151));
	TS_ASSERT_EQUALS(p0[1], (signed int *)(-3928575));
	TS_ASSERT_EQUALS(p0[2], (signed int *)(-6821375));
	TS_ASSERT_EQUALS(*p1, -5202431);
	TS_ASSERT_EQUALS(p2[0], (const signed int *)(7054848));
	TS_ASSERT_EQUALS(p2[1], (const signed int *)(5947392));
	TS_ASSERT_EQUALS(p2[2], (const signed int *)(2362368));
	return (signed int *)(-3293183);
}

void testReflectGlobal() {
	using namespace cpgf;

	reflectMethod("TestFunction0", &TestFunction0);
	reflectMethod("TestFunction1", &TestFunction1);
	reflectMethod("TestFunction2", &TestFunction2);
	reflectMethod("TestFunction3", &TestFunction3);
	reflectMethod("TestFunction4", &TestFunction4);
	reflectMethod("TestFunction5", &TestFunction5);
	reflectMethod("TestFunction6", &TestFunction6);
	reflectMethod("TestFunction7", &TestFunction7);
	reflectMethod("TestFunction8", &TestFunction8);
	reflectMethod("TestFunction9", &TestFunction9);
	reflectMethod("TestFunction10", &TestFunction10);
	reflectMethod("TestFunction11", &TestFunction11);
}
void testAutogen() {
	testReflectGlobal();

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction0");
		TestObject0 obj;
		signed int t0 = (signed int)(3471872);
		signed int * t1 [] = { (signed int *)(7120384), (signed int *)(-811007), (signed int *)(2758656) }; 
		signed int t2 = (signed int)(-5612543);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction1");
		TestObject0 obj;
		signed int t0 = (signed int)(-1735167);
		signed int * t1 [] = { (signed int *)(-8161791), (signed int *)(-7385599), (signed int *)(5640192) }; 
		signed int t2 = (signed int)(-3451391);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction2");
		TestObject0 obj;
		signed int t0 = (signed int)(-4145663);
		const signed int * t1 [] = { (const signed int *)(1352704), (const signed int *)(8287744), (const signed int *)(-6002175) }; 
		signed int t2 = (signed int)(78848);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction3");
		TestObject0 obj;
		signed int t0 = (signed int)(-3941375);
		const signed int * t1 [] = { (const signed int *)(-364543), (const signed int *)(2025472), (const signed int *)(8120832) }; 
		signed int t2 = (signed int)(-3864575);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction4");
		TestObject0 obj;
		signed int t0 = (signed int)(-1597951);
		volatile signed int * t1 [] = { (volatile signed int *)(-2242559), (volatile signed int *)(6086656), (volatile signed int *)(-5181951) }; 
		signed int t2 = (signed int)(-1794047);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction5");
		TestObject0 obj;
		signed int t0 = (signed int)(899072);
		volatile signed int * t1 [] = { (volatile signed int *)(2254336), (volatile signed int *)(-4761087), (volatile signed int *)(8248832) }; 
		signed int t2 = (signed int)(6852608);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction6");
		TestObject0 obj;
		signed int t0 = (signed int)(7605760);
		const volatile signed int * t1 [] = { (const volatile signed int *)(3271168), (const volatile signed int *)(-5446143), (const volatile signed int *)(3331072) }; 
		signed int t2 = (signed int)(212992);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction7");
		TestObject0 obj;
		signed int t0 = (signed int)(-70143);
		const volatile signed int * t1 [] = { (const volatile signed int *)(-5045759), (const volatile signed int *)(-3495935), (const volatile signed int *)(4902912) }; 
		signed int t2 = (signed int)(-1416703);
		method->invoke(&obj, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction8");
		TestObject0 obj;
		method->invoke(&obj, (signed int)(-1727487), (signed int)(5693440), (signed int)(6915584));
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction9");
		TestObject0 obj;
		signed int t1 = (signed int)(6699520);
		signed int * t2 [] = { (signed int *)(-3234815), (signed int *)(-2626047), (signed int *)(-961023) }; 
		method->invoke(&obj, (signed int)(3417088), &t1, t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction10");
		TestObject0 obj;
		signed int * t0 [] = { (signed int *)(3128320), (signed int *)(-7903743), (signed int *)(-7055871) }; 
		signed int t1 = (signed int)(4529152);
		signed int * t2 [] = { (signed int *)(4054016), (signed int *)(5694976), (signed int *)(-923647) }; 
		method->invoke(&obj, t0, &t1, t2);
	}

	{
		const GMetaClass * metaClass = findMetaClass<TestObject0>();
		const GMetaMethod * method = metaClass->getMethod("TestFunction11");
		TestObject0 obj;
		signed int * t0 [] = { (signed int *)(2585088), (signed int *)(6326272), (signed int *)(-2949631) }; 
		signed int t1 = (signed int)(-7711743);
		const signed int * t2 [] = { (const signed int *)(3564544), (const signed int *)(-8214015), (const signed int *)(2515968) }; 
		method->invoke(&obj, t0, &t1, t2);
	}


	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction0");
		signed int t0 = (signed int)(3789312);
		signed int * t1 [] = { (signed int *)(4807680), (signed int *)(-193023), (signed int *)(3184640) }; 
		signed int t2 = (signed int)(2752000);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction1");
		signed int t0 = (signed int)(7856640);
		signed int * t1 [] = { (signed int *)(394752), (signed int *)(-1550335), (signed int *)(-1294335) }; 
		signed int t2 = (signed int)(5920256);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction2");
		signed int t0 = (signed int)(-3565055);
		const signed int * t1 [] = { (const signed int *)(7847424), (const signed int *)(1704448), (const signed int *)(6338560) }; 
		signed int t2 = (signed int)(-7934975);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction3");
		signed int t0 = (signed int)(-7950335);
		const signed int * t1 [] = { (const signed int *)(-1384447), (const signed int *)(-2371583), (const signed int *)(-7728639) }; 
		signed int t2 = (signed int)(-165375);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction4");
		signed int t0 = (signed int)(4019200);
		volatile signed int * t1 [] = { (volatile signed int *)(-2162175), (volatile signed int *)(3975168), (volatile signed int *)(-8344575) }; 
		signed int t2 = (signed int)(-2384383);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction5");
		signed int t0 = (signed int)(7435264);
		volatile signed int * t1 [] = { (volatile signed int *)(-7311359), (volatile signed int *)(8218624), (volatile signed int *)(7781376) }; 
		signed int t2 = (signed int)(2779136);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction6");
		signed int t0 = (signed int)(6262272);
		const volatile signed int * t1 [] = { (const volatile signed int *)(-7907327), (const volatile signed int *)(509952), (const volatile signed int *)(-3792895) }; 
		signed int t2 = (signed int)(3998720);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction7");
		signed int t0 = (signed int)(4273152);
		const volatile signed int * t1 [] = { (const volatile signed int *)(3493376), (const volatile signed int *)(8329216), (const volatile signed int *)(6489088) }; 
		signed int t2 = (signed int)(4896256);
		method->invoke(NULL, &t0, t1, &t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction8");
		method->invoke(NULL, (signed int)(4436992), (signed int)(-4688383), (signed int)(4215296));
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction9");
		signed int t1 = (signed int)(-1471487);
		signed int * t2 [] = { (signed int *)(-4712959), (signed int *)(-553983), (signed int *)(-7333887) }; 
		method->invoke(NULL, (signed int)(6983680), &t1, t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction10");
		signed int * t0 [] = { (signed int *)(2472448), (signed int *)(-2481663), (signed int *)(-167423) }; 
		signed int t1 = (signed int)(-1618431);
		signed int * t2 [] = { (signed int *)(1527296), (signed int *)(-3613183), (signed int *)(3446272) }; 
		method->invoke(NULL, t0, &t1, t2);
	}

	{
		const GMetaClass * metaClass = getGlobalMetaClass();
		const GMetaMethod * method = metaClass->getMethod("TestFunction11");
		signed int * t0 [] = { (signed int *)(-7089151), (signed int *)(-3928575), (signed int *)(-6821375) }; 
		signed int t1 = (signed int)(-5202431);
		const signed int * t2 [] = { (const signed int *)(7054848), (const signed int *)(5947392), (const signed int *)(2362368) }; 
		method->invoke(NULL, t0, &t1, t2);
	}



}
