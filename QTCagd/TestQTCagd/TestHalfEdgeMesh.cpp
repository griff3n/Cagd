#include "stdafx.h"
#include "CppUnitTest.h"

#include "HalfEdgeMesh.h"
#include "HalfEdgeMesh.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestQTCagd
{
	TEST_CLASS(TestHalfEdgeMesh) {
		
		TEST_CLASS_INITIALIZE(init) {
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}

		TEST_METHOD(testKonstruktor) {
			HalfEdgeMesh * testHEM = new HalfEdgeMesh();
			Assert::IsNotNull(testHEM);
			Assert::AreEqual(0uLL, 0 + testHEM->vertices.size());
			Assert::AreEqual(0uLL, 0 + testHEM->halfEdges.size());
			Assert::AreEqual(0uLL, 0 + testHEM->faces.size());
			Assert::AreEqual(1.0f, testHEM->model.column(0)[0]);
			Assert::AreEqual(0.0f, testHEM->model.column(0)[1]);
			Assert::AreEqual(0.0f, testHEM->model.column(0)[2]);
			Assert::AreEqual(0.0f, testHEM->model.column(0)[3]);
			Assert::AreEqual(0.0f, testHEM->model.column(1)[0]);
			Assert::AreEqual(1.0f, testHEM->model.column(1)[1]);
			Assert::AreEqual(0.0f, testHEM->model.column(1)[2]);
			Assert::AreEqual(0.0f, testHEM->model.column(1)[3]);
			Assert::AreEqual(0.0f, testHEM->model.column(2)[0]);
			Assert::AreEqual(0.0f, testHEM->model.column(2)[1]);
			Assert::AreEqual(1.0f, testHEM->model.column(2)[2]);
			Assert::AreEqual(0.0f, testHEM->model.column(2)[3]);
			Assert::AreEqual(0.0f, testHEM->model.column(3)[0]);
			Assert::AreEqual(0.0f, testHEM->model.column(3)[1]);
			Assert::AreEqual(0.0f, testHEM->model.column(3)[2]);
			Assert::AreEqual(1.0f, testHEM->model.column(3)[3]);
			delete testHEM;
		}
				
		TEST_METHOD(testDestruktor) {
			QVector4D posA = QVector4D(2, 2, 1, 1);
			QVector4D posB = QVector4D(8, 2, 2, 1);
			QVector4D posC = QVector4D(8, 8, -1, 1);
			QVector4D posD = QVector4D(2, 8, -2, 1);
			graphicVertex * a = new graphicVertex(posA);
			graphicVertex * b = new graphicVertex(posB);
			graphicVertex * c = new graphicVertex(posC);
			graphicVertex * d = new graphicVertex(posD);
			halfEdge * ab = new halfEdge();
			halfEdge * bc = new halfEdge();
			halfEdge * cd = new halfEdge();
			halfEdge * da = new halfEdge();
			halfEdge * ad = new halfEdge();
			halfEdge * dc = new halfEdge();
			halfEdge * cb = new halfEdge();
			halfEdge * ba = new halfEdge();
			graphicFace * abcda = new graphicFace();
			graphicFace * dcbad = new graphicFace();

			/*/
			a->id = 1;
			b->id = 2;
			c->id = 3;
			d->id = 4;
			ab->id = 5;
			bc->id = 6;
			cd->id = 7;
			da->id = 8;
			ad->id = 9;
			dc->id = 10;
			cb->id = 11;
			ba->id = 12;
			abcda->id = 13;
			dcbad->id = 14;
			//*/

			a->edge = ab;
			b->edge = bc;
			c->edge = cd;
			d->edge = da;
			ab->vert = a;
			ab->pair = ba;
			ab->face = abcda;
			ab->next = bc;
			bc->vert = b;
			bc->pair = cb;
			bc->face = abcda;
			bc->next = cd;
			cd->vert = c;
			cd->pair = dc;
			cd->face = abcda;
			cd->next = da;
			da->vert = d;
			da->pair = ad;
			da->face = abcda;
			da->next = ab;
			ad->vert = a;
			ad->pair = da;
			ad->face = dcbad;
			ad->next = dc;
			dc->vert = d;
			dc->pair = cd;
			dc->face = dcbad;
			dc->next = cb;
			cb->vert = c;
			cb->pair = bc;
			cb->face = dcbad;
			cb->next = ba;
			ba->vert = b;
			ba->pair = ab;
			ba->face = dcbad;
			ba->next = ad;
			abcda->edge = ab;
			dcbad->edge = ba;
									
			HalfEdgeMesh * testHEM = new HalfEdgeMesh();
			testHEM->vertices.push_back(a);
			testHEM->vertices.push_back(b);
			testHEM->vertices.push_back(c);
			testHEM->vertices.push_back(d);
			testHEM->halfEdges.push_back(ab);
			testHEM->halfEdges.push_back(bc);
			testHEM->halfEdges.push_back(cd);
			testHEM->halfEdges.push_back(da);
			testHEM->halfEdges.push_back(ad);
			testHEM->halfEdges.push_back(dc);
			testHEM->halfEdges.push_back(cb);
			testHEM->halfEdges.push_back(ba);
			testHEM->faces.push_back(abcda);
			testHEM->faces.push_back(dcbad);
			delete testHEM;
		}
	};
}