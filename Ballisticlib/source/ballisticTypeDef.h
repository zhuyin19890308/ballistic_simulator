#ifndef BALLISTICLIBRARY_BALLISTICTYPEDEF_H
#define BALLISTICLIBRARY_BALLISTICTYPEDEF_H


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


#define BSL_TYPE float

#include <exception>
#include <memory>
#include <cstring>
#include <vector>
#include <iostream>
#include <exception>
#include <queue>

namespace BSL
{

#define BSL_VERSION_MAJOR    2025
#define BSL_VERSION_MINOR    4
#define BSL_VERSION_REVISE   15
#define BSL_VERSION_BUILD    1                              


#define BSL_PI                      3.141592653589793f        
#define BSL_g                       9.80665                   
#define BSL_VirtualTemp_Ground      288.900000f               
#define BSL_Pon                     100.000000                
#define BSL_K                       1.4000000                 
#define BSL_R                       287.000000                
#define BSL_G1                      (-0.006328f)              

#define BSL_R_earth                 6371393.0f                   
#define BSL_Day_Second              86400
                      
#define BSL_CHUAN                   0x1A                      


#define sub_1 1
#define sub_2 2
#define sub_3 3
#define sub_4 4
#define sub_5 5
#define sub_6 6
#define sub_7 7
#define sub_8 8
#define sub_11 11
#define sub_12 12
#define sub_13 13
#define sub_21 21
#define sub_22 22
#define sub_23 23
#define sub_24 24
#define sub_25 25
#define sub_31 31
#define sub_32 32
#define sub_33 33
#define sub_51 51
#define sub_52 52
#define sub_53 53


#define ZHUANG_0    0
#define ZHUANG_1    1
#define ZHUANG_2    2
#define ZHUANG_3    3
#define ZHUANG_4    4
#define ZHUANG_5    5
#define ZHUANG_6    6
#define ZHUANG_7    7
#define ZHUANG_8    8
#define  ZHUANG_ALL 100


#define  HEIGHT_0            1
#define  HEIGHT_500          2
#define  HEIGHT_1000         3
#define  HEIGHT_1500         4
#define  HEIGHT_2000         5
#define  HEIGHT_2500         6
#define  HEIGHT_3000         7
#define  HEIGHT_3500         8
#define  HEIGHT_4000         9
#define  HEIGHT_4500         10
#define  HEIGHT_5000         11
#define  HEIGHT_5500         12


typedef struct Bullet_type
{
    unsigned char mainType{};                             
    unsigned char subType{};                              
    unsigned char speedFlag{};                            
    unsigned long Bullet_Type_Decoder() const;
} Bullet_type;


typedef enum BSL_Bullet_Name
{
     NAME_AK47 =  (BSL_CHUAN << 24) + (sub_4 << 16) + (HEIGHT_0 << 8) + ZHUANG_ALL,
} BSL_Bullet_Name;


BSL_TYPE BALLISTICLIBRARY calTime(BSL_Bullet_Name unkownAmmo,BSL_TYPE distance);

typedef struct BALLISTICLIBRARY BSL_Initialize_Para
{
    BSL_TYPE T;                                            
    BSL_TYPE Wz;                                           
    BSL_TYPE Wy;                                           
    BSL_TYPE Wx;                                           
    BSL_TYPE i;                                            
    BSL_TYPE m;                                            
    BSL_TYPE d;                                            
    BSL_TYPE P0;                                           
    BSL_TYPE xw;                                           
    BSL_TYPE yw;                                           
    BSL_TYPE zw;                                           
    BSL_TYPE latitude;                                     
    BSL_TYPE longtitude;                                   
    BSL_TYPE x_target;                                     
    BSL_TYPE y_target;                                     
    BSL_TYPE z_target;                                     
    bool useHighArea;                                      
    BSL_TYPE Rotate_x;                                     
    BSL_TYPE Rotate_y;                                     
    BSL_TYPE Rotate_z;                                     
    bool isSeparate;                                       
    BSL_TYPE E_theta_x;                                    
    BSL_TYPE E_theta_z;                                    
    BSL_TYPE E_v0;                                         
    BSL_TYPE E_cb;                                         
    BSL_TYPE verticalCorrection;                           
    BSL_TYPE horizontalCorrection;                         
    BSL_TYPE gun_height;                                   

    BSL_Initialize_Para();

    static BSL_Initialize_Para &BSL_Defualt_Para();
} BSL_Initialize_Para;


typedef struct BSL_Point
{
    BSL_TYPE timeStamp{};                        
    BSL_TYPE v_x{};                              
    BSL_TYPE v_y{};                              
    BSL_TYPE v_z{};                              
    BSL_TYPE v_remain{};                         
    BSL_TYPE Pos_x{};                            
    BSL_TYPE Pos_y{};                            
    BSL_TYPE Pos_z{};                            
    BSL_TYPE Pos_xw{};                           
    BSL_TYPE Pos_yw{};                           
    BSL_TYPE Pos_zw{};                           
    BSL_Point();
} BSL_Point;


typedef struct BALLISTICLIBRARY BSL_Result
{
    int len{};                                          
    std::unique_ptr<BSL_Point[]> pData = nullptr;       
    explicit BSL_Result(int length);                    
} BSL_Result;


typedef enum BSL_Convert_Angle
{
    CVT_ANGLE2MIL = 0,                                  
    CVT_MIL2ANGLE = 1,                                  
    CVT_ANGLE2RAD = 2,                                  
    CVT_RAD2ANGLE = 3,                                  
    CVT_MIL2RAD = 4,                                    
    CVT_RAD2MIL = 5,                                    
} BSL_Convert_Angle;


typedef enum BSL_Export_Method
{
    EXP_RELATIVE_POS = 0,                               
    EXP_ABSOLUTE_POS = 1,                               
    EXP_ALL = 2,                                        
} BSL_Export_Method;


typedef enum BSL_Cb_Method
{
    CB_USE_VALUE,                                       
    CB_USE_THETA_LINEAR,                                
    CB_USE_THETA_BILINEAR,                              
} BSL_Cb_Method;


typedef enum BSL_Random_Method
{
    RANDOM_UNIFORM,                                      
    RANDOM_NORMAL,                                       
    RANDOM_BINORMAL,                                     
} BSL_Random_Method;


typedef enum BSL_Result_Is_Empty_Flag
{
    DEFAULT,                                            
    IGNORE_INIT_VALUE,                                  
} BSL_Result_Is_Empty_Flag;


typedef struct BSL_RK4_Para
{
    BSL_TYPE Kvx[4] {};
    BSL_TYPE Kvy[4] {};
    BSL_TYPE Kvz[4] {};
    BSL_TYPE Kx[4] {};
    BSL_TYPE Ky[4] {};
    BSL_TYPE Kz[4] {};

    void clear();

    BSL_RK4_Para();

    void setValue(unsigned int i, const BSL_TYPE &multi_para, const BSL_TYPE &Kvx_value, const BSL_TYPE &Kvy_value,
                  const BSL_TYPE &Kvz_value, const BSL_TYPE &Kx_value, const BSL_TYPE &Ky_value,
                  const BSL_TYPE &Kz_value);


    std::shared_ptr<BSL_TYPE> Sum();
} BSL_RK4_Para;


typedef enum BSL_Weather_Msg_Type
{
    WEATHER_TYPE_0000 = 0000,                              
    WEATHER_TYPE_1111 = 1111,                              
    WEATHER_TYPE_2222 = 2222,                              
    WEATHER_TYPE_3333 = 3333,                              
    WEATHER_TYPE_4444 = 4444,                              
    WEATHER_TYPE_5555 = 5555,                              
} BSL_Weather_Msg_Type;

typedef struct singleWeatherMsg
{
    int height{};                                           
    BSL_TYPE temperature{};                                    
    int windAngle{};                                        
    BSL_TYPE windSpeed{};                                      
    singleWeatherMsg();                                     
    void clear();
} singleWeatherMsg;

typedef struct weatherMsgStruct
{
    unsigned short len{};                                   
    BSL::BSL_Weather_Msg_Type messageType;                  
    std::string time;                                       
    int weather_station_height{};                           
    BSL_TYPE weather_station_pressure{};                       
    BSL_TYPE weather_station_temperature{};                    
    int measureHeight{};                                    
    std::vector<singleWeatherMsg> Msg;                      
    bool isEmpty() const;                                   

    
    singleWeatherMsg getNearestWeather(BSL_TYPE yw) const;
} weatherMsgStruct;

typedef enum BSL_Earth_Part
{
    NORTH_HEMISPHERE,                                      
    SOUTH_HEMISPHERE,                                      
} BSL_Earth_Part;

typedef struct BSL_queue
{
    unsigned long len{20};                                
    std::queue<BSL_TYPE> buff;                  
    BSL_TYPE miniValue{100};                         
    
    void resize(unsigned long newSize);                   
    
    void operator<<(BSL_TYPE &singleData);      
    
    BSL_TYPE operator[](int index) const;       
    
    int count() const;

    double mean() const;
    void print() const;
} BSL_queue;
}




#endif 
