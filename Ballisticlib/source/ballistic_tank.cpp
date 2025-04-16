#define END -1

#include "ballistic_tank.h"

ballistic_tank::ballistic_tank(BSL::BSL_Bullet_Name ammoName, BSL_TYPE theta0, BSL::BSL_Initialize_Para para) :
    ballistic(ammoName, theta0, para)
{
    bullet = bulletFactory.createAmmo(BulletName);
    this->setCb(theta0, BSL::CB_USE_THETA_LINEAR);
    this->setV0();
    this->setTheta(Theta0);
    BSL_Result_Init();
    setRotateMatrix();
}

ballistic_tank::~ballistic_tank()
{

}

void ballistic_tank::setVerticalCorrection(BSL_TYPE Target_distance, BSL_TYPE distanceOptical)
{
    ballistic::setVerticalCorrection(Target_distance, distanceOptical);
}

void ballistic_tank::setHorizantalCorrection(BSL_TYPE Target_distance, BSL_TYPE distanceOptical)
{
    ballistic::setHorizantalCorrection(Target_distance, distanceOptical);
}


