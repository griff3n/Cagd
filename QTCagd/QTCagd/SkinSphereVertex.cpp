#include "SkinSphereVertex.h"



SkinSphereVertex::SkinSphereVertex()
{
}


SkinSphereVertex::~SkinSphereVertex()
{
}

Qt3DCore::QEntity* SkinSphereVertex::returnSkinObject()
{
	// Sphere shape data
	Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
	sphereMesh->setRings(20);
	sphereMesh->setSlices(20);
	sphereMesh->setRadius(2);

	Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
	sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

	Qt3DCore::QEntity* m_sphereEntity = new Qt3DCore::QEntity();

	m_sphereEntity->addComponent(sphereMesh);
	m_sphereEntity->addComponent(sphereMaterial);

	return m_sphereEntity;
}
