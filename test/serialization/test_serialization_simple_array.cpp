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

template <typename READER, typename AR>
void doTestSimpleArray(IMetaService * service, IMetaWriter * writer, const READER & reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(GMetaArchiveConfig(), service, writer));

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

	metaArchiveWriteValue(archiveWriter.get(), "b", b);
	metaArchiveWriteValue(archiveWriter.get(), "c", c);
	metaArchiveWriteValue(archiveWriter.get(), "wc", wc);
	metaArchiveWriteValue(archiveWriter.get(), "sc", sc);
	metaArchiveWriteValue(archiveWriter.get(), "uc", uc);
	metaArchiveWriteValue(archiveWriter.get(), "si", si);
	metaArchiveWriteValue(archiveWriter.get(), "usi", usi);
	metaArchiveWriteValue(archiveWriter.get(), "i", i);
	metaArchiveWriteValue(archiveWriter.get(), "ui", ui);
	metaArchiveWriteValue(archiveWriter.get(), "l", l);
	metaArchiveWriteValue(archiveWriter.get(), "ul", ul);
	metaArchiveWriteValue(archiveWriter.get(), "ll", ll);
	metaArchiveWriteValue(archiveWriter.get(), "ull", ull);
	metaArchiveWriteValue(archiveWriter.get(), "f", f);
	metaArchiveWriteValue(archiveWriter.get(), "df", df);
	metaArchiveWriteValue(archiveWriter.get(), "ldf", ldf);
	metaArchiveWriteValue(archiveWriter.get(), "s", s);
	metaArchiveWriteValue(archiveWriter.get(), "ps", ps);
	metaArchiveWriteValue(archiveWriter.get(), "o", o);
	metaArchiveWriteValue(archiveWriter.get(), "po", po);
	metaArchiveWriteValue(archiveWriter.get(), "npo", npo);

	ar.rewind();

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(GMetaArchiveConfig(), service, reader.get()));

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

	serializeReadValue(archiveReader.get(), "b", rb);
	serializeReadValue(archiveReader.get(), "c", rc);
	serializeReadValue(archiveReader.get(), "wc", rwc);
	serializeReadValue(archiveReader.get(), "sc", rsc);
	serializeReadValue(archiveReader.get(), "uc", ruc);
	serializeReadValue(archiveReader.get(), "si", rsi);
	serializeReadValue(archiveReader.get(), "usi", rusi);
	serializeReadValue(archiveReader.get(), "i", ri);
	serializeReadValue(archiveReader.get(), "ui", rui);
	serializeReadValue(archiveReader.get(), "l", rl);
	serializeReadValue(archiveReader.get(), "ul", rul);
	serializeReadValue(archiveReader.get(), "ll", rll);
	serializeReadValue(archiveReader.get(), "ull", rull);
	serializeReadValue(archiveReader.get(), "f", rf);
	serializeReadValue(archiveReader.get(), "df", rdf);
	serializeReadValue(archiveReader.get(), "ldf", rldf);
	serializeReadValue(archiveReader.get(), "s", rs);
	serializeReadValue(archiveReader.get(), "ps", rps);
	serializeReadValue(archiveReader.get(), "o", ro);
	serializeReadValue(archiveReader.get(), "po", rpo);
	serializeReadValue(archiveReader.get(), "npo", rnpo);

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
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	GScopedInterface<IMetaReader> reader(createTextStreamMetaReader(service.get(), stream));
	
	doTestSimpleArray(service.get(), writer.get(), MetaReaderGetter(reader.get()), TestArchiveStream<stringstream>(stream));

//	cout << stream.str().c_str() << endl;
}

GTEST(testSimpleArray_Xml)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestSimpleArray(service.get(), writer.get(), MetaReaderGetterXml(service.get(), outputArchive), TestArchiveStreamNone());

//	cout << stream.str().c_str() << endl;
}


GTEST(testSimpleArray_Json)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	GMetaJsonArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(outputArchive));
	
	doTestSimpleArray(service.get(), writer.get(), MetaReaderGetterJson(service.get(), outputArchive), TestArchiveStreamNone());

//	outputArchive.saveToStream(cout);
}


} // unnamed namespace
