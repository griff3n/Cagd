#include "stdafx.h"
#include "CppUnitTest.h"

#include "graphicFace.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestQTCagd
{
	TEST_CLASS(TestGraphicFace) {
		TEST_METHOD(testKonstruktor) {
			graphicFace * testGF = new graphicFace();
			Assert::IsNotNull(testGF);
			Assert::IsFalse(testGF->getIsSelected());
			Assert::IsFalse(testGF->isHole);
			Assert::AreEqual(0, testGF->valence);
			Assert::IsNull(testGF->edge);
			Assert::IsNull(testGF->nextLOD);
			delete testGF;
		}

		TEST_METHOD(testDestruktor) {
			graphicFace * testGF = new graphicFace();
			delete testGF;
		}

		TEST_METHOD(testSetVals) {
			halfEdge * edge = new halfEdge();
			QVector4D pos0 = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
			graphicVertex * nextLOD = new graphicVertex(pos0);
			graphicFace * testGF = new graphicFace();
			testGF->isHole = true;
			testGF->valence = 1;
			testGF->edge = edge;
			testGF->nextLOD = nextLOD;
			Assert::IsTrue(testGF->isHole);
			Assert::AreEqual(1, testGF->valence);
			Assert::IsNotNull(testGF->edge);
			Assert::IsTrue(edge == testGF->edge);
			Assert::IsNotNull(testGF->nextLOD);
			Assert::IsTrue(nextLOD == testGF->nextLOD);
			delete edge;
			delete testGF;
		}

		TEST_CLASS_CLEANUP(clean) {
			_CrtDumpMemoryLeaks();
		}
	};
}