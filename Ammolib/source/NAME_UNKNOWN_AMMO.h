//
// Created by gn on 2021/9/16.
//

#ifndef BALLISTICLIBRARY_NAME_UNKNOWN_AMMO_H
#define BALLISTICLIBRARY_NAME_UNKNOWN_AMMO_H


//用户使用时去掉_use后缀
#define BALLISTICLIBRARY_LIBRARY_use

#ifdef _WIN32
// Windows 平台
    #define BALLISTICLIBRARY_EXPORT     __declspec(dllexport)
    #define BALLISTICLIBRARY_IMPORT     __declspec(dllimport)
#elif defined(__APPLE__) || defined(__linux__) || defined(unix)
// macOS 和 Linux 平台
#define BALLISTICLIBRARY_EXPORT     __attribute__((visibility("default")))
#define BALLISTICLIBRARY_IMPORT     __attribute__((visibility("default")))
#define BALLISTICLIBRARY_HIDDEN     __attribute__((visibility("hidden")))
#else
// 其他平台
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
//#include "Eigen"
#include "../../utils/matrix/Matrix.hpp"
#include "ammoBase.h"



using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
namespace NAMESPACE_UNKNOWN_AMMO_H
{

class unknow:public ammoBase{

   std::pair<int, BSL_TYPE> getMax(const std::vector<BSL_TYPE> &data) const override;


};

class BALLISTICLIBRARY NAME_TANK_KILL_LIU_85 : public ammoBase
{
public:
    //85坦克杀伤榴弹
    const BSL_TYPE TANK_KILL_LIU_85_SPEED = 124;               //弹丸初

    BSL_TYPE getTime(BSL_TYPE distance) override;

};


class BALLISTICLIBRARY NAME_TANK_KILL_LIU_120 : public ammoBase
{
public:
    //120坦克杀伤榴弹
    const BSL_TYPE TANK_KILL_LIU_120_SPEED = 124;               //弹丸初

    BSL_TYPE getTime(BSL_TYPE distance) override;

};





}




#ifdef __cplusplus
}
#endif
#endif //BSL_BULLET_DATA_LIBRARY_NAME_WHEELED_CANNON_LIU_120_1_H
