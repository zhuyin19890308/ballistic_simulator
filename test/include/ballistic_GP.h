#ifndef BALLISTICLIBRARY_BALLISTIC_GP_H
#define BALLISTICLIBRARY_BALLISTIC_GP_H




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





class BALLISTICLIBRARY ballistic_GP final : public ballistic {
public:

    ballistic_GP(BSL::BSL_Bullet_Name ammoName, BSL_TYPE theta0, BSL_TYPE distance, BSL_TYPE alpha, BSL::BSL_Initialize_Para para);
    ~ballistic_GP();

    void setVerticalCorrection(BSL_TYPE Target_distance, BSL_TYPE distanceOptical = 0.3) override;

    void setHorizantalCorrection(BSL_TYPE Target_distance, BSL_TYPE distanceOptical = 0.1) override;
    void setCb(BSL_TYPE distance, BSL_TYPE alpha, BSL::BSL_Cb_Method method);
};







#endif 
