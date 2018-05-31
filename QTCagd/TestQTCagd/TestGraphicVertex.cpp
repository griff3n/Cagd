#include "stdafx.h"
#include "CppUnitTest.h"

#include "graphicVertex.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestQTCagd
{
	TEST_CLASS(TestGraphicVertex) {
		TEST_METHOD(testKonstruktor) {
			glm::vec4 pos = glm::vec4(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * testGV = new graphicVertex(pos);
			Assert::IsNotNull(testGV);
			Assert::IsFalse(testGV->getIsSelected());
			Assert::IsNull(testGV->getDesign());
			Assert::IsNull(testGV->getSMem());
			Assert::AreEqual(0.0f, testGV->weight);
			Assert::IsTrue(pos == testGV->location);
			Assert::IsFalse(testGV->hasFlag);
			Assert::AreEqual(0, testGV->valence);
			Assert::IsNull(testGV->edge);
			Assert::IsNull(testGV->lastLOD);
			Assert::IsNull(testGV->nextLOD);
			delete testGV;
		}

		TEST_METHOD(testKonstruktor2) {
			glm::vec4 pos0 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			glm::vec4 pos = glm::vec4(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * lastLOD = new graphicVertex(pos0);
			Skin * testSkin = new Skin();
			Design * design = new Design(testSkin, testSkin);
			ObjectMemory * sMem = new ObjectMemory();
			graphicVertex * testGV = new graphicVertex(pos, lastLOD, design, sMem);
			Assert::IsNotNull(testGV);
			Assert::IsFalse(testGV->getIsSelected());
			Assert::IsNotNull(testGV->getDesign());
			Assert::IsTrue(design == testGV->getDesign());
			Assert::IsNotNull(testGV->getSMem());
			Assert::IsTrue(sMem == testGV->getSMem());
			Assert::AreEqual(0.0f, testGV->weight);
			Assert::IsTrue(pos == testGV->location);
			Assert::IsFalse(pos0 == testGV->location);
			Assert::IsFalse(testGV->hasFlag);
			Assert::AreEqual(0, testGV->valence);
			Assert::IsNull(testGV->edge);
			Assert::IsNotNull(testGV->lastLOD);
			Assert::IsTrue(lastLOD == testGV->lastLOD);
			Assert::IsNull(testGV->nextLOD);
			delete testGV;
		}

		TEST_METHOD(testDestruktor) {
			glm::vec4 pos0 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			glm::vec4 pos = glm::vec4(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * lastLOD = new graphicVertex(pos0);
			Skin * testSkin = new Skin();
			Design * design = new Design(testSkin, testSkin);
			ObjectMemory * sMem = new ObjectMemory();
			graphicVertex * testGV = new graphicVertex(pos, lastLOD, design, sMem);
			delete testGV;
		}

		TEST_METHOD(testSetVals) {
			glm::vec4 pos0 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			glm::vec4 pos1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 pos = glm::vec4(4.0f, 3.0f, 2.0f, 1.0f);
			graphicVertex * lastLOD = new graphicVertex(pos0);
			graphicVertex * nextLOD = new graphicVertex(pos0);
			halfEdge * edge = new halfEdge();
			graphicVertex * testGV = new graphicVertex(pos);
			testGV->weight = 1.0f;
			testGV->hasFlag = true;
			testGV->valence = 1;
			testGV->edge = edge;
			testGV->nextLOD = nextLOD;
			testGV->lastLOD = lastLOD;
			Assert::AreEqual(1.0f, testGV->weight);
			Assert::IsTrue(testGV->hasFlag);
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