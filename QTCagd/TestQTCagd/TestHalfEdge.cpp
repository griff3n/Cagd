#include "stdafx.h"
#include "CppUnitTest.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "graphicFace.h"
#include "graphicFace.cpp"
#include "GraphicObject.h"
#include "GraphicObject.cpp"
#include "graphicVertex.h"
#include "graphicVertex.cpp"
#include "halfEdge.h"
#include "halfEdge.cpp"
#include "ObjectMemory.h"
#include "ObjectMemory.cpp"
#include "skin.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestQTCagd
{		
	TEST_CLASS(TestHalfEdge)
	{
	public:
		
		TEST_METHOD(testKonstruktor) {
			halfEdge * testHE = new halfEdge();
			Assert::IsNotNull(testHE);
			Assert::IsFalse(testHE->getIsSelected());
			Assert::IsNull(testHE->vert);
			Assert::IsNull(testHE->pair);
			Assert::IsNull(testHE->face);
			Assert::IsNull(testHE->next);
			delete testHE;
		}
		
		TEST_METHOD(testDestruktor) {
			halfEdge * testHE = new halfEdge();
			graphicVertex * vert = new graphicVertex(QVector4D(0.0f, 0.0f, 0.0f, 0.0f));
			halfEdge * pair = new halfEdge();
			graphicFace * face = new graphicFace();
			halfEdge * next = new halfEdge();
			testHE->vert = vert;
			testHE->pair = pair;
			testHE->face = face;
			testHE->next = next;
			delete next;
			delete face;
			delete pair;
			delete vert;
			delete testHE;
		}

		TEST_METHOD(testSetVals) {
			halfEdge * testHE = new halfEdge();
			graphicVertex * vert = new graphicVertex(QVector4D(0.0f, 0.0f, 0.0f, 0.0f));
			halfEdge * pair = new halfEdge();
			graphicFace * face = new graphicFace();
			halfEdge * next = new halfEdge();
			testHE->vert = vert;
			testHE->pair = pair;
			testHE->face = face;
			testHE->next = next;
			Assert::IsNotNull(testHE->vert);
			Assert::IsTrue(vert == testHE->vert);
			Assert::IsNotNull(testHE->pair);
			Assert::IsTrue(pair == testHE->pair);
			Assert::IsNotNull(testHE->face);
			Assert::IsTrue(face == testHE->face);
			Assert::IsNotNull(testHE->next);
			Assert::IsTrue(next == testHE->next);
			delete next;
			delete face;
			delete pair;
			delete vert;
			delete testHE;
		}

		TEST_CLASS_CLEANUP(clean) {
			_CrtDumpMemoryLeaks();
		}
	};
}