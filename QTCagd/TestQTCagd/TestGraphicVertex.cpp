#include "stdafx.h"
#include "CppUnitTest.h"

#include "graphicVertex.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestQTCagd
{
	TEST_CLASS(TestGraphicVertex) {
		TEST_METHOD(testKonstruktor) {
			QVector4D pos = QVector4D(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * testGV = new graphicVertex(pos);
			Assert::IsNotNull(testGV);
			Assert::IsFalse(testGV->getIsSelected());
			Assert::AreEqual(0.0f, testGV->weight);
			Assert::IsTrue(pos == testGV->location);
			Assert::IsFalse(testGV->sharp);
			Assert::IsFalse(testGV->selected);
			Assert::AreEqual(0, testGV->valence);
			Assert::IsNull(testGV->edge);
			Assert::IsNull(testGV->lastLOD);
			Assert::IsNull(testGV->nextLOD);
			delete testGV;
		}

		TEST_METHOD(testKonstruktor2) {
			QVector4D pos0 = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
			QVector4D pos = QVector4D(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * lastLOD = new graphicVertex(pos0);
			graphicVertex * testGV = new graphicVertex(pos, lastLOD);
			Assert::IsNotNull(testGV);
			Assert::IsFalse(testGV->getIsSelected());
			Assert::AreEqual(0.0f, testGV->weight);
			Assert::IsTrue(pos == testGV->location);
			Assert::IsFalse(pos0 == testGV->location);
			Assert::IsFalse(testGV->sharp);
			Assert::IsFalse(testGV->selected);
			Assert::AreEqual(0, testGV->valence);
			Assert::IsNull(testGV->edge);
			Assert::IsNotNull(testGV->lastLOD);
			Assert::IsTrue(lastLOD == testGV->lastLOD);
			Assert::IsNull(testGV->nextLOD);
			delete testGV;
		}

		TEST_METHOD(testDestruktor) {
			QVector4D pos0 = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
			QVector4D pos = QVector4D(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * lastLOD = new graphicVertex(pos0);
			graphicVertex * testGV = new graphicVertex(pos, lastLOD);
			delete testGV;
		}

		TEST_METHOD(testSetVals) {
			QVector4D pos0 = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
			QVector4D pos1 = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
			QVector4D pos = QVector4D(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * lastLOD = new graphicVertex(pos0);
			graphicVertex * nextLOD = new graphicVertex(pos1);
			halfEdge * edge = new halfEdge();
			graphicVertex * testGV = new graphicVertex(pos);
			testGV->weight = 1.0f;
			testGV->sharp = true;
			testGV->selected = true;
			testGV->valence = 1;
			testGV->edge = edge;
			testGV->nextLOD = nextLOD;
			testGV->lastLOD = lastLOD;
			Assert::AreEqual(1.0f, testGV->weight);
			Assert::IsTrue(testGV->sharp);
			Assert::IsTrue(testGV->selected);
			Assert::AreEqual(1, testGV->valence);
			Assert::IsNotNull(testGV->edge);
			Assert::IsTrue(edge == testGV->edge);
			Assert::IsNotNull(testGV->nextLOD);
			Assert::IsTrue(nextLOD == testGV->nextLOD);
			Assert::IsNotNull(testGV->lastLOD);
			Assert::IsTrue(lastLOD == testGV->lastLOD);
			delete edge;
			delete testGV;
		}

		TEST_CLASS_CLEANUP(clean) {
			_CrtDumpMemoryLeaks();
		}
	};
}