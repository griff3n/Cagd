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
			Assert::IsNull(testGF->surrounding);
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
			Assert::IsNull(testGF->surrounding);
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
			halfEdge * surrounding = new halfEdge();
			glm::vec4 pos0 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			graphicVertex * nextLOD = new graphicVertex(pos0);
			graphicFace * testGF = new graphicFace();
			testGF->isHole = true;
			testGF->surrounding = surrounding;
			testGF->nextLOD = nextLOD;
			Assert::IsTrue(testGF->isHole);
			Assert::IsNotNull(testGF->surrounding);
			Assert::IsTrue(surrounding == testGF->surrounding);
			Assert::IsNotNull(testGF->nextLOD);
			Assert::IsTrue(nextLOD == testGF->nextLOD);
		}
	};
}