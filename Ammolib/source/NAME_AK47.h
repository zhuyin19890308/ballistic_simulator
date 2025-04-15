
#ifndef BSL_BULLET_DATA_LIBRARY_NAME_AK47_H
#define BSL_BULLET_DATA_LIBRARY_NAME_AK47_H


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
#include <vector>

#include "../../utils/matrix/Matrix.hpp"
#include "ammoBase.h"


using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
namespace NAMESPACE_AK47
{

const int AK47_LEN_MAX = 8;                               
const int AK47_CB_ROW = 1;                                  
const int AK47_CB_COL = AK47_LEN_MAX;           
const BSL_TYPE AK47_SPEED = 710;                              

class BALLISTICLIBRARY  NAME_AK47 final: public ammoBase
{

public:
    
    NAME_AK47();


    
    BSL_TYPE getCb(BSL_TYPE theta) override;

    
    BSL_TYPE getSpeed() override;

    
    string getBulletName() override;

    

    BSL_TYPE get_ETheta_x(BSL_TYPE theta) override;


    

    BSL_TYPE get_ETheta_z( BSL_TYPE theta) override;


    vector<BSL_TYPE> getDistanceList() const override;

    vector<BSL_TYPE> getAngleList() const override;


private:

    vector<BSL_TYPE> _distanceList;
    vector<BSL_TYPE> _angleList;
    matrix::Matrix<BSL_TYPE, AK47_CB_ROW, AK47_CB_COL> _cbList;
    matrix::Matrix<BSL_TYPE,AK47_CB_ROW, AK47_CB_COL> _ExList;
    matrix::Matrix<BSL_TYPE,AK47_CB_ROW, AK47_CB_COL> _EzList;



    
    int searchInsert(const vector<BSL_TYPE> &nums, BSL_TYPE Target) override;

};
}
#ifdef __cplusplus
}
#endif
#endif 
