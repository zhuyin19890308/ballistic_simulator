#include "NAME_UNKNOWN_AMMO.h"




float NAMESPACE_UNKNOWN_AMMO_H::NAME_TANK_KILL_LIU_85::getTime(float distance)
{

    return  distance/TANK_KILL_LIU_85_SPEED;
}

float NAMESPACE_UNKNOWN_AMMO_H::NAME_TANK_KILL_LIU_120::getTime(float distance)
{
        return  distance/TANK_KILL_LIU_120_SPEED;
}

std::pair<int, float> NAMESPACE_UNKNOWN_AMMO_H::unknow::getMax(const std::vector<float> &data) const
{
    return std::pair<int, float>(0,10000000);
}
