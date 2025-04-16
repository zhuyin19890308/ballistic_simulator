



#ifndef BALLISTICLIBRARY_BALLISTIC_CANNON_H
#define BALLISTICLIBRARY_BALLISTIC_CANNON_H



#define BALLISTICLIBRARY_LIBRARY_use

#ifdef _WIN32

    #define BALLISTICLIBRARY_EXPORT     __declspec(dllexport)
    #define BALLISTICLIBRARY_IMPORT     __declspec(dllimport)
#elif defined(__APPLE__) || defined(__linux__) || defined(unix)

#define BALLISTICLIBRARY_EXPORT     __attribute__((visibility("default")))
#define BALLISTICLIBRARY_IMPORT     __attribute__((visibility("default")))
#define BALLISTICLIBRARY_HIDDEN     __attribute__((visibility("hidden")))
#else

    #define BALLISTICLIBRARY_EXPORT
    #define BALLISTICLIBRARY_IMPORT
    #define BALLISTICLIBRARY_HIDDEN
#endif

#ifdef BALLISTICLIBRARY_LIBRARY_use
#define BALLISTICLIBRARY BALLISTICLIBRARY_EXPORT
#else
#define BALLISTICLIBRARY BALLISTICLIBRARY_IMPORT
#endif


#include <iostream>
#include <algorithm>
#include <map>
#include "baseClass.h"
#include "ballisticTypeDef.h"


class BALLISTICLIBRARY ballistic_cannon final : public ballistic
{

public:
    explicit ballistic_cannon(BSL::BSL_Bullet_Name ammoName, BSL_TYPE theta0,
                              BSL::BSL_Initialize_Para para = BSL::BSL_Initialize_Para::BSL_Defualt_Para());
    explicit ballistic_cannon(BSL::BSL_Bullet_Name bulletName,
                       BSL::BSL_Initialize_Para para = BSL::BSL_Initialize_Para::BSL_Defualt_Para());
    ~ballistic_cannon();

    float calOnce(float theta);
    float iteration(float startAngle, float endAngle, float target, float delta);
private:
    BSL_TYPE BSL_Omiga = 7.292115f * pow(10, -5);
    std::vector<BSL_TYPE> _faster_dv_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod) override;
    BSL_TYPE updateLatitude(BSL_TYPE currentDistance, BSL_TYPE initLatitude, BSL::BSL_Earth_Part shootPos = NORTH_HEMISPHERE);
    unsigned long _fun_RK4() override;
};


#endif 
