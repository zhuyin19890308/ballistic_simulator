
#ifndef BALLISTICLIBRARY_BASECLASS_H
#define BALLISTICLIBRARY_BASECLASS_H


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
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cstdarg>

#include "ballisticTypeDef.h"

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include <random>
#include <ctime>
#include "ammoData.h"
#include "matrix/math.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include "ballisticWeatherMsg.h"

using namespace std;
using namespace boost::algorithm;

class BALLISTICLIBRARY ballistic
{

public:
    explicit ballistic(BSL::BSL_Bullet_Name bulletName, BSL_TYPE theta0,
                       BSL::BSL_Initialize_Para para = BSL::BSL_Initialize_Para::BSL_Defualt_Para());
    explicit ballistic(BSL::BSL_Bullet_Name bulletName,
                       BSL::BSL_Initialize_Para para = BSL::BSL_Initialize_Para::BSL_Defualt_Para());
    virtual ~ballistic();
    static std::string version();
    BSL_TYPE getV0() const;
    void setV0();
    BSL_TYPE getTheta0() const;
    BSL_TYPE getTheta() const;
    void setTheta(BSL_TYPE theta);
    void setCb(BSL_TYPE value, BSL::BSL_Cb_Method method = BSL::CB_USE_THETA_LINEAR);
    virtual std::shared_ptr<BSL::weatherMsgStruct> decodeWethearMessage(const string &message);
    void showRandomNumDistribution();
    static BSL_TYPE angleConvert(BSL_TYPE srcAngle, BSL::BSL_Convert_Angle method);
    unsigned long calculate();
    void getResult(BSL::BSL_Result &dst);
    virtual void writeData(const std::string &path, BSL::BSL_Export_Method method);
    string outString(const basic_string<char> &str, unsigned char setLen = 40);
    void writeHeader(ofstream &file, const string &writeTime);
    void writeContent(ofstream &file, BSL_Export_Method method) const;
    std::vector<BSL_TYPE> getGlobalCoordinatePOS();
    virtual matrix::Matrix<BSL_TYPE, 4, 4> calRotateMat();
    virtual void setHorizantalCorrection(BSL_TYPE targetDistance, BSL_TYPE opticalDistance);
    virtual void setVerticalCorrection(BSL_TYPE targetDistance, BSL_TYPE opticalDistance);
    const matrix::Matrix<BSL_TYPE, 4, 4> &getRotateMatrix() const;
    void setRotateMatrix();
    void setInitializePara(const BSL::BSL_Initialize_Para &initializePara);
    virtual BSL_TYPE _dvx_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod);
    virtual BSL_TYPE _dvy_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod);
    virtual BSL_TYPE _dvz_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod);
    virtual std::vector<BSL_TYPE> _faster_dv_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod);
    virtual unsigned long _fun_RK4();
    static vector<BSL_TYPE> randomData;
    ammoFactory bulletFactory;
    std::shared_ptr<ammoBase> bullet = nullptr;
    std::shared_ptr<weatherMsgDecoderFactory> weatherFactory = nullptr;
    std::shared_ptr<weatherMsgBase> weatherDecoder = nullptr;
    std::shared_ptr<BSL::weatherMsgStruct> decodedWeatherMsg;
    BSL::BSL_Bullet_Name BulletName{};                      
    BSL_TYPE Cb{};                                             
    BSL_TYPE Theta0{};                                         
    BSL_TYPE Theta{};                                          
    BSL_TYPE V0{};                                             
    bool isFinishedRK4 = false;                             
    
    typedef struct Result
    {
        vector<BSL_TYPE> timeStamp {};
        vector<BSL_TYPE> v_x{};                                
        vector<BSL_TYPE> v_y{};                                
        vector<BSL_TYPE> v_z{};                                
        vector<BSL_TYPE> v_remain{};                           
        vector<BSL_TYPE> Pos_x{};                              
        vector<BSL_TYPE> Pos_y{};                              
        vector<BSL_TYPE> Pos_z{};                              
        vector<BSL_TYPE> Pos_xw{};                             
        vector<BSL_TYPE> Pos_yw{};                             
        vector<BSL_TYPE> Pos_zw{};
        Result();
        ~Result();
        void BSL_Result_Clear();
        void get_BSL_Result(BSL::BSL_Result &res) const;
        BSL::BSL_Point operator[](unsigned int i) const;
        void operator<<(BSL::BSL_Point &point);
        bool isEmpty(BSL::BSL_Result_Is_Empty_Flag flag = BSL::DEFAULT) const;
        unsigned long size() const;
    } Result;

    
    Result Result_Rk4;
    BSL::BSL_Point Result_Rk4_Buffer;
    int Result_Rk4_Save_Interval = 1;
    int getResultRk4SaveInterval() const;
    void setResultRk4SaveInterval(int resultRk4SaveInterval);
    matrix::Matrix<BSL_TYPE, 4, 4> rotateMatrix;
    BSL::BSL_Initialize_Para Initialize_Para{};
    void BSL_Result_Init();
    vector<BSL_TYPE>
    generateRandomNum(BSL_TYPE inputParaA, BSL_TYPE inputParaB = 0, int dataLen = BSL_Day_Second,
                      BSL::BSL_Random_Method method = BSL::RANDOM_NORMAL);
    static BSL_TYPE getOneRandomNum();
    BSL_TYPE getCb(bool byCalculate = true) const;
    BSL_TYPE getOneRandomNum(BSL_TYPE inputParaA, BSL_TYPE inputParaB = 0, BSL::BSL_Random_Method method = BSL::RANDOM_NORMAL);
    static int getCurrentSecond();
    BSL_TYPE _fun_II(BSL_TYPE y, bool isStandard = false) const;
    BSL_TYPE _soundSpeed(const BSL_TYPE &yw) const;
    BSL_TYPE _fun_Fv(const BSL_TYPE &vt);
    BSL_TYPE _fun_Gv(const BSL_TYPE &vr, const BSL_TYPE &y);
    BSL_TYPE _fun_Vr(const BSL_TYPE &vx, const BSL_TYPE &vy, const BSL_TYPE &vz) const;
    void updateInitPara(BSL_TYPE yw);
    string getCurrentTime();
    void setEtheta();
    BSL_TYPE calDistance(BSL::BSL_Initialize_Para& para);
    BSL_TYPE calDistance(BSL_TYPE &x, BSL_TYPE &y, BSL_TYPE &z, BSL_TYPE &x_target, BSL_TYPE &y_target, BSL_TYPE &z_target);
    BSL_TYPE calOnce(BSL_TYPE theta);
    BSL_TYPE iteration(BSL_TYPE startAngle, BSL_TYPE endAngle, BSL_TYPE target, BSL_TYPE delta = 0.1);
    BSL_TYPE getTime(BSL_TYPE distance);
};

#endif 
