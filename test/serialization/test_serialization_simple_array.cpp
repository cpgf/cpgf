#include "testserializationcommonclasses.h"
#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


#define LOOP(l) for(int z = 0; z < l; ++z)

namespace {

template <typename AR>
void doTestSimpleArray(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	enum {
		A = 3, B = 5, C = 6, D = 7, E = 8, F = 10, G = 11,
		H = 12, I = 15, J = 16, K = 17, L = 18, M = 20, N = 21,
		O = 22, P = 25, Q = 28,
		R = 3, S = 5
	};

	bool b[A];
	char c[B];
	wchar_t wc[C];
	signed char sc[D];
	unsigned char uc[E];
	short si[F];
	unsigned short usi[G];
	int i[H];
	unsigned int ui[I];
	long l[J];
	unsigned long ul[K];
	long long ll[L];
	unsigned long long ull[M];
	float f[N];
	double df[O];
	long double ldf[P];
	string s[Q];
	string * ps[Q];
	TestSerializeClass o[R];
	TestSerializeClass * po[R];
	TestSerializeClass * npo[S];

#define INIT(v, l) LOOP(l) initTestValue(v[z], getTestSeed(z + 1));
	INIT(b, A)
	INIT(c, B)
	INIT(wc, C)
	INIT(sc, D)
	INIT(uc, E)
	INIT(si, F)
	INIT(usi, G)
	INIT(i, H)
	INIT(ui, I)
	INIT(l, J)
	INIT(ul, K)
	INIT(ll, L)
	INIT(ull, M)
	INIT(f, N)
	INIT(df, O)
	INIT(ldf, P)
	INIT(s, Q)
	INIT(o, R)
#undef INIT

#define INIT(v, u, l) LOOP(l) v[z] = &u[z];
	INIT(ps, s, Q)
	INIT(po, o, R)
#undef INIT

#define INIT(v, l) LOOP(l) { v[z] = new TestSerializeClass(); initTestValue(*v[z], getTestSeed(z + 1)); }
	INIT(npo, S)
#undef INIT

	serializeWriteData(archiveWriter.get(), "b", b);
	serializeWriteData(archiveWriter.get(), "c", c);
	serializeWriteData(archiveWriter.get(), "wc", wc);
	serializeWriteData(archiveWriter.get(), "sc", sc);
	serializeWriteData(archiveWriter.get(), "uc", uc);
	serializeWriteData(archiveWriter.get(), "si", si);
	serializeWriteData(archiveWriter.get(), "usi", usi);
	serializeWriteData(archiveWriter.get(), "i", i);
	serializeWriteData(archiveWriter.get(), "ui", ui);
	serializeWriteData(archiveWriter.get(), "l", l);
	serializeWriteData(archiveWriter.get(), "ul", ul);
	serializeWriteData(archiveWriter.get(), "ll", ll);
	serializeWriteData(archiveWriter.get(), "ull", ull);
	serializeWriteData(archiveWriter.get(), "f", f);
	serializeWriteData(archiveWriter.get(), "df", df);
	serializeWriteData(archiveWriter.get(), "ldf", ldf);
	serializeWriteData(archiveWriter.get(), "s", s);
	serializeWriteData(archiveWriter.get(), "ps", ps);
	serializeWriteData(archiveWriter.get(), "o", o, &module);
	serializeWriteData(archiveWriter.get(), "po", po, &module);
	serializeWriteData(archiveWriter.get(), "npo", npo, &module);

	TestArchiveTraits<AR>::rewind(ar);

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader));

	bool rb[A];
	char rc[B];
	wchar_t rwc[C];
	signed char rsc[D];
	unsigned char ruc[E];
	short rsi[F];
	unsigned short rusi[G];
	int ri[H];
	unsigned int rui[I];
	long rl[J];
	unsigned long rul[K];
	long long rll[L];
	unsigned long long rull[M];
	float rf[N];
	double rdf[O];
	long double rldf[P];
	string rs[Q];
	string * rps[Q];
	TestSerializeClass ro[R];
	TestSerializeClass * rpo[R];
	TestSerializeClass * rnpo[S];

#define INIT(v, l) LOOP(l) initTestValue(v[z], getTestSeed(0));
	INIT(rb, A)
	INIT(rc, B)
	INIT(rwc, C)
	INIT(rsc, D)
	INIT(ruc, E)
	INIT(rsi, F)
	INIT(rusi, G)
	INIT(ri, H)
	INIT(rui, I)
	INIT(rl, J)
	INIT(rul, K)
	INIT(rll, L)
	INIT(rull, M)
	INIT(rf, N)
	INIT(rdf, O)
	INIT(rldf, P)
	INIT(rs, Q)
	INIT(ro, R)
#undef INIT

#define INIT(v, l) LOOP(l) v[z] = NULL;
	INIT(rps, Q)
	INIT(rpo, R)
	INIT(rnpo, S)
#undef INIT

	serializeReadData(archiveReader.get(), "b", rb);
	serializeReadData(archiveReader.get(), "c", rc);
	serializeReadData(archiveReader.get(), "wc", rwc);
	serializeReadData(archiveReader.get(), "sc", rsc);
	serializeReadData(archiveReader.get(), "uc", ruc);
	serializeReadData(archiveReader.get(), "si", rsi);
	serializeReadData(archiveReader.get(), "usi", rusi);
	serializeReadData(archiveReader.get(), "i", ri);
	serializeReadData(archiveReader.get(), "ui", rui);
	serializeReadData(archiveReader.get(), "l", rl);
	serializeReadData(archiveReader.get(), "ul", rul);
	serializeReadData(archiveReader.get(), "ll", rll);
	serializeReadData(archiveReader.get(), "ull", rull);
	serializeReadData(archiveReader.get(), "f", rf);
	serializeReadData(archiveReader.get(), "df", rdf);
	serializeReadData(archiveReader.get(), "ldf", rldf);
	serializeReadData(archiveReader.get(), "s", rs);
	serializeReadData(archiveReader.get(), "ps", rps);
	serializeReadData(archiveReader.get(), "o", ro, &module);
	serializeReadData(archiveReader.get(), "po", rpo, &module);
	serializeReadData(archiveReader.get(), "npo", rnpo, &module);

#define EQ(v, u, l) LOOP(l) GEQUAL(v[z], u[z]);
	EQ(b, rb, A)
	EQ(c, rc, B)
	EQ(wc, rwc, C)
	EQ(sc, rsc, D)
	EQ(uc, ruc, E)
	EQ(si, rsi, F)
	EQ(usi, rusi, G)
	EQ(i, ri, H)
	EQ(ui, rui, I)
	EQ(l, rl, J)
	EQ(ul, rul, K)
	EQ(ll, rll, L)
	EQ(ull, rull, M)
	EQ(s, rs, Q)
	EQ(o, ro, R)
#undef EQ

#define EQ(v, u, l) LOOP(l) GCHECK(fabs(v[z] - u[z]) < 0.1);
	EQ(f, rf, N)
	EQ(df, rdf, O)
	EQ(ldf, rldf, P)
#undef EQ

#define EQ(v, u, l) LOOP(l) GEQUAL(&v[z], u[z]);
	EQ(rs, rps, Q)
	EQ(ro, rpo, R)
#undef EQ

	LOOP(S) GEQUAL(*npo[z], *rnpo[z]);

	LOOP(S) delete npo[z];
	LOOP(S) delete rnpo[z];
}

GTEST(testSimpleArray_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestSimpleArray(writer.get(), reader.get(), stream);

//	cout << stream.str().c_str() << endl;
}

GTEST(testSimpleArray_Xml)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestSimpleArray(writer.get(), reader.get(), storage);

//	storage.saveToStream(cout);
}


GTEST(testSimpleArray_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestSimpleArray(writer.get(), reader.get(), storage);

//	storage.saveToStream(cout);
}


} // unnamed namespace
