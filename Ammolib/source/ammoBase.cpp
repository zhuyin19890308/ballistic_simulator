#include<algorithm>
#include "ammoBase.h"

BSL_TYPE ammoBase::getCb(BSL_TYPE theta)
{
    return 0;
}

BSL_TYPE ammoBase::getCb(BSL_TYPE distance, BSL_TYPE theta)
{
    return 0;
}

BSL_TYPE ammoBase::getSpeed()
{
    return 0;
}

string ammoBase::getBulletName()
{
    return std::string();
}

int ammoBase::searchInsert(const vector<BSL_TYPE> &nums, BSL_TYPE Target)
{
    return 0;
}

vector<BSL_TYPE> ammoBase::read_Ballistic_WindSpeed_sheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle) const
{
    return vector<BSL_TYPE>();
}


BSL_TYPE ammoBase::get_ETheta_x( BSL_TYPE theta)
{
    return  0;
}


BSL_TYPE ammoBase::get_ETheta_z( BSL_TYPE theta)
{
    return  0;
}

std::vector<int> ammoBase::detectFireArea(std::vector<BSL_TYPE> &_distanceList, BSL_TYPE &distance)
{
    std::pair<int, BSL_TYPE> max = getMax(_distanceList);
    std::vector<int> index;

    if(max.second == _distanceList.back())
    {
        std::cout << getBulletName() << ":" << "No boundary" << std::endl;
        int indexTmp = binarySearch(_distanceList, distance);
        index.push_back(indexTmp);
    }
    else
    {
        std::cout << getBulletName() << ":" << "Boundary exsits" << std::endl;
        std::cout << "Index：" << max.first << std::endl;
        std::vector<BSL_TYPE> front, back;
        int indexFront{0}, indexBack{0};

        for(unsigned long i = 0; i < _distanceList.size(); i++)
        {
            if(i <= max.first)
            {
                front.push_back(_distanceList.at(i));
                indexFront = binarySearch(front, distance);
            }
            else
            {
                back.push_back(_distanceList.at(_distanceList.size() - (i - max.first)));
                indexBack = _distanceList.size() - binarySearch(back, distance);
            }
        }

        index.push_back(indexFront);
        index.push_back(indexBack);
        std::cout << "indexFront:" << indexFront << std::endl;
        std::cout << "indexBack:" << indexBack << std::endl;
    }

    return index;
}

bool ammoBase::detectFireArea(std::vector<BSL_TYPE> &distance)
{
    BSL_TYPE max = getMax(distance).second;
    bool ret;
    max == distance.back() ? ret = false : ret = true;
    return ret;
}

vector<BSL_TYPE> ammoBase::getDistanceList() const
{
    return vector<BSL_TYPE> {};
}

vector<BSL_TYPE> ammoBase::getAngleList() const
{
    return vector<BSL_TYPE> {};
}


std::pair<int, BSL_TYPE> ammoBase::getMax(const std::vector<BSL_TYPE> &data) const
{
    if(data.empty())
    {
        throw std::invalid_argument("empty data");
    }

    BSL_TYPE max;
    unsigned long index = 0;
    index =max_element(data.begin(),data.end())-data.begin();
    max = data[index];
    return std::make_pair(index, max);
}



int ammoBase::binarySearch(const vector<BSL_TYPE> &nums, BSL_TYPE Target)
{
    int left{0};
    int right = static_cast<int> (nums.size());

    do
    {
        int mid = (left + right) / 2;

        if (nums[mid] == Target)
        {
            return mid;
        }
        else if (nums[mid] < Target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    while (left < right);
    return -1;
}

BSL_TYPE ammoBase::getTime(BSL_TYPE distance)
{
    return  0;
}

