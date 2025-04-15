#ifndef BALLISTICLIBRARY_BALLISTIC_TANK_H
#define BALLISTICLIBRARY_BALLISTIC_TANK_H




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





class BALLISTICLIBRARY ballistic_tank : public ballistic {
public:
    ballistic_tank(BSL::BSL_Bullet_Name ammoName, BSL_TYPE theta0, BSL::BSL_Initialize_Para para);

    ~ballistic_tank();

    void setVerticalCorrection(BSL_TYPE Target_distance, BSL_TYPE distanceOptical = 0.3) override;

    void setHorizantalCorrection(BSL_TYPE Target_distance, BSL_TYPE distanceOptical = 0.1) override;
};







#endif 
