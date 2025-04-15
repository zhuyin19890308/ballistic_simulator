



#ifndef BSL_BULLET_DATA_LIBRARY_AMMOBASE_H
#define BSL_BULLET_DATA_LIBRARY_AMMOBASE_H


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

#define PI                      3.141592653589793f        

#include <iostream>
#include <vector>


#define BSL_TYPE float

using namespace std;


class BALLISTICLIBRARY ammoBase
{
public:



    
    virtual BSL_TYPE getCb(BSL_TYPE theta);

    
    virtual BSL_TYPE getCb(BSL_TYPE distance, BSL_TYPE theta);

    
    virtual BSL_TYPE getSpeed();

    
    virtual string getBulletName();

    
    typedef enum WindSpeed_Method
    {
        WS_SHEET,                           
        WS_CAL,                             
    } WindSpeed_Method;

    WindSpeed_Method method = WindSpeed_Method::WS_CAL;
    const static int windSpeedSheetRow = 16; 
    const static int windSpeedSheetCol = 20; 



    
    virtual vector<BSL_TYPE>
    read_Ballistic_WindSpeed_sheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle) const;

    
    virtual int searchInsert(const vector<BSL_TYPE> &nums, BSL_TYPE Target);


    

    virtual BSL_TYPE get_ETheta_x(BSL_TYPE theta);

    

    virtual BSL_TYPE get_ETheta_z( BSL_TYPE theta);

    
    virtual std::vector<int> detectFireArea(std::vector<BSL_TYPE> &distanceList
                                            , BSL_TYPE &distance);

    
    virtual bool detectFireArea(std::vector<BSL_TYPE> &distance);

    virtual vector<BSL_TYPE> getDistanceList() const;

    virtual vector<BSL_TYPE> getAngleList()const;
    

    virtual std::pair<int, BSL_TYPE> getMax(const std::vector<BSL_TYPE> &data) const;

    
    virtual int binarySearch(const vector<BSL_TYPE> &nums, BSL_TYPE Target);

    virtual BSL_TYPE getTime(BSL_TYPE distance);

};


#endif 
