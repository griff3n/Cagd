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
			Assert::IsNull(testGF->getDesign());
			Assert::IsNull(testGF->getSMem());
			Assert::IsFalse(testGF->isHole);
			Assert::AreEqual(0, testGF->valence);
			Assert::IsNull(testGF->edge);
			Assert::IsNull(testGF->nextLOD);
		}

		TEST_METHOD(testKonstruktor2) {
			Skin * testSkin = new Skin();
			Design * design = new Design(testSkin, testSkin);
			ObjectMemory * sMem = new ObjectMemory();
			graphicFace * testGF = new graphicFace(design, sMem);
			
			Assert::IsNotNull(testGF);
			Assert::IsFalse(testGF->getIsSelected());
			Assert::IsNotNull(testGF->getDesign());
			Assert::IsTrue(design == testGF->getDesign());
			Assert::IsNotNull(testGF->getSMem());
			Assert::IsTrue(sMem == testGF->getSMem());
			Assert::IsFalse(testGF->isHole);
			Assert::AreEqual(0, testGF->valence);
			Assert::IsNull(testGF->edge);
			Assert::IsNull(testGF->nextLOD);
		}

		TEST_METHOD(testDestruktor) {
			Skin * testSkin = new Skin();
			Design * design = new Design(testSkin, testSkin);
			ObjectMemory * sMem = new ObjectMemory();
			graphicFace * testGF = new graphicFace(design, sMem);
			delete testGF;
		}

		TEST_METHOD(testSetVals) {
			halfEdge * edge = new halfEdge();
			glm::vec4 pos0 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
		}
	};
}