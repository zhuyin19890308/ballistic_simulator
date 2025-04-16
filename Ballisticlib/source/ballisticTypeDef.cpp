
#include "ballisticTypeDef.h"

BSL::BSL_Result::BSL_Result(int length)
{
    len = length;
    pData = std::unique_ptr<BSL_Point[]>(new BSL_Point[length], std::default_delete<BSL_Point[]>());
}

BSL::BSL_Initialize_Para::BSL_Initialize_Para()
{
    T = BSL_VirtualTemp_Ground;                         
    Wz = 0.0;                                           
    Wy = 0.0;                                           
    Wx = 0.0;                                           
    i = 0.0;                                            
    m = 0.0;                                            
    d = 0.0;                                            
    P0 = BSL_Pon;                                       
    xw = 0.0;                                           
    yw = 0.0;                                           
    zw = 0.0;                                           
    latitude = 0.0;                                     
    longtitude = 0.0;                                   
    x_target = 0.0;                                     
    y_target = 0.0;                                     
    z_target = 0.0;                                     
    useHighArea = false;
    Rotate_x = 0.0;                                     
    Rotate_y = 0.0;                                     
    Rotate_z = 0.0;                                     
    isSeparate = false;                                 
    E_theta_x = 0.0;                                    
    E_theta_z = 0.0;                                    
    E_v0 = 0.0;                                         
    E_cb = 0.0;                                         
    verticalCorrection = 0.0;                           
    horizontalCorrection = 0.0;                         
    gun_height = 0.0;                                   
}

BSL::BSL_Initialize_Para &BSL::BSL_Initialize_Para::BSL_Defualt_Para()
{
    static BSL_Initialize_Para para;
    return para;
}

void BSL::BSL_RK4_Para::clear()
{
    memset(this, 0, sizeof(BSL_RK4_Para));
}

BSL::BSL_RK4_Para::BSL_RK4_Para()
{
    this->clear();
}

void
BSL::BSL_RK4_Para::setValue(unsigned int i, const BSL_TYPE &multi_para, const BSL_TYPE &Kvx_value, const BSL_TYPE &Kvy_value,
                            const BSL_TYPE &Kvz_value, const BSL_TYPE &Kx_value, const BSL_TYPE &Ky_value,
                            const BSL_TYPE &Kz_value)
{
    if (i > 3 || i < 0)
    {
        return;
    }

    this->Kvx[i] = multi_para * Kvx_value;
    this->Kvy[i] = multi_para * Kvy_value;
    this->Kvz[i] = multi_para * Kvz_value;
    this->Kx[i] = multi_para * Kx_value;
    this->Ky[i] = multi_para * Ky_value;
    this->Kz[i] = multi_para * Kz_value;
}

std::shared_ptr<BSL_TYPE> BSL::BSL_RK4_Para::Sum()
{
    std::shared_ptr<BSL_TYPE> pSum(new BSL_TYPE[6], [](BSL_TYPE * ptr)
    {
        delete[] ptr;
    });
    pSum.get()[0] = (Kvx[0] + 2 * Kvx[1] + 2 * Kvx[2] + Kvx[3]) / 6;
    pSum.get()[1] = (Kvy[0] + 2 * Kvy[1] + 2 * Kvy[2] + Kvy[3]) / 6;
    pSum.get()[2] = (Kvz[0] + 2 * Kvz[1] + 2 * Kvz[2] + Kvz[3]) / 6;
    pSum.get()[3] = (Kx[0] + 2 * Kx[1] + 2 * Kx[2] + Kx[3]) / 6;
    pSum.get()[4] = (Ky[0] + 2 * Ky[1] + 2 * Ky[2] + Ky[3]) / 6;
    pSum.get()[5] = (Kz[0] + 2 * Kz[1] + 2 * Kz[2] + Kz[3]) / 6;
    return pSum;
}

BSL::BSL_Point::BSL_Point()
{
    memset(this, 0, sizeof(BSL_Point));
}

BSL::singleWeatherMsg::singleWeatherMsg()
{
    this->clear();
}

void BSL::singleWeatherMsg::clear()
{
    this->windSpeed = 0;
    this->windAngle = 0;
    this->temperature = 0;
    this->height = 0;
}

unsigned long BSL::Bullet_type::Bullet_Type_Decoder() const
{
    unsigned long coder;
    coder = (mainType << 16) + (subType << 8) + speedFlag;
    return coder;
}

bool BSL::weatherMsgStruct::isEmpty() const
{
    return Msg.empty();
}

BSL::singleWeatherMsg BSL::weatherMsgStruct::getNearestWeather(BSL_TYPE yw) const
{
    BSL_TYPE relativeHeight = yw - this->weather_station_height;
    int nearestIndex;

    if (!this->isEmpty())
    {
        unsigned char lenTmp = this->Msg.size();
        int heightList[lenTmp];

        for (int index = 0; index < lenTmp; ++index)
        {
            heightList[index] = this->Msg.at(index).height;

            if (static_cast<BSL_TYPE>(heightList[index]) >= relativeHeight)
            {
                
                if (index == 0)
                {
                    return Msg.front();
                }

                abs(static_cast<BSL_TYPE>(heightList[index]) - relativeHeight) >
                abs(static_cast<BSL_TYPE>(heightList[index - 1]) - relativeHeight) ?
                nearestIndex = index - 1 : nearestIndex = index;
                return Msg.at(nearestIndex);
            }
        }

        
        if (static_cast<BSL_TYPE>(heightList[lenTmp - 1]) < relativeHeight)
        {
            return Msg.back();
        }
    }
    else
    {
        throw std::length_error("empty weather message!");
    }

    return {};
}
void BSL::BSL_queue::resize(unsigned long newSize)
{
    this->len = newSize;
    unsigned long cnt = this->buff.size();

    if (cnt <= newSize)
    {
        return;
    }
    else
    {
        for (unsigned long i = 0; i < cnt - newSize; ++i)
        {
            this->buff.pop();
        }
    }
}

void BSL::BSL_queue::operator<<(BSL_TYPE &singleData)
{
    if (this->buff.size() < len)
    {
        this->buff.push(singleData);
    }

    if (this->buff.size() >= len)
    {
        this->buff.pop();                           
        this->buff.push(singleData);                  
    }

    if(this->buff.size() == 0)
    {
        miniValue = singleData; 
    }

    if(this->miniValue > singleData)
    {
        miniValue = singleData; 
    }
}

BSL_TYPE BSL::BSL_queue::operator[](int index) const
{
    if (index < 0 || index > static_cast<int>(this->len))
    {
        throw std::range_error("invalid index, must >0 and <maxLen!");
    }
    else if (index > this->buff.size())
    {
        throw std::range_error("invalid index, out of range!");
    }
    else
    {
        BSL_TYPE ret;
        const BSL_TYPE *pBuff = &buff.front();
        ret = *(pBuff + index);
        return ret;
    }
}

int BSL::BSL_queue::count() const
{
    return buff.size();
}

double BSL::BSL_queue::mean() const
{
    double sum = 0;

    for(unsigned long i = 0; i < buff.size(); i++)
    {
        sum += *(&(buff.front()) + i);
    }

    return sum / this->count();
}

void BSL::BSL_queue::print() const
{
    for(unsigned long i = 0; i < buff.size(); i++)
    {
        std::cout << *(&(buff.front()) + i) << ",";
    }

    std::cout << std::endl;
}

float BSL::calTime(BSL::BSL_Bullet_Name unkownAmmo, float distance)
{
    switch (unkownAmmo) {
    default:
        return 0;
    }


}
