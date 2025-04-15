#include "NAME_AK47.h"


NAMESPACE_AK47::NAME_AK47::NAME_AK47()
{
    _distanceList = {100,200,300,400,500,600,700,800
                    };
    _angleList = {3.1,4.2,5.8,7.8,10,13,16,21
                 };
    BSL_TYPE cbList[AK47_LEN_MAX] =
    {
      100,32,19.2,14.6,12,11,10,10.25
    };
    _cbList = matrix::Matrix<BSL_TYPE, AK47_CB_ROW, AK47_CB_COL>(cbList);


     BSL_TYPE _EthetaxList[AK47_LEN_MAX]={2e-06, 2e-06, 2e-06, 0.78125, 0.5625, 0.5, 0.375, 0.375};

     _ExList = matrix::Matrix<float, AK47_CB_ROW, AK47_CB_COL>(_EthetaxList);



      BSL_TYPE _EthetazList[AK47_LEN_MAX]={2e-06, 2e-06, 0.90625, 0.75, 0.625, 0.46875, 0.375, 0.375};

     _EzList = matrix::Matrix<float, AK47_CB_ROW, AK47_CB_COL>(_EthetazList);
}











BSL_TYPE NAMESPACE_AK47::NAME_AK47::getCb(BSL_TYPE theta)
{
    int index = searchInsert(_angleList, theta);
    BSL_TYPE retCb;

    switch (index)
    {
        case 0 :
            retCb = _cbList(AK47_CB_ROW - 1, index);
            break;

        case AK47_LEN_MAX:
            retCb = _cbList(AK47_CB_ROW - 1, AK47_LEN_MAX - 1);
            break;

        default:
            BSL_TYPE ratio = (theta - _angleList[index - 1]) / (_angleList[index] - _angleList[index - 1]);
            retCb = _cbList(AK47_CB_ROW - 1, index - 1) +
                    (_cbList(AK47_CB_ROW - 1, index) - _cbList(AK47_CB_ROW - 1, index - 1)) *
                    ratio;
            break;
    }

    return retCb;
}

int NAMESPACE_AK47::NAME_AK47::searchInsert(const vector<BSL_TYPE> &nums, BSL_TYPE Target)
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

    return left;
}

BSL_TYPE NAMESPACE_AK47::NAME_AK47::getSpeed()
{
    return AK47_SPEED;
}

string NAMESPACE_AK47::NAME_AK47::getBulletName()
{
    return "125mm破甲弹";
}



BSL_TYPE NAMESPACE_AK47::NAME_AK47::get_ETheta_x( BSL_TYPE theta)
{
    int index = searchInsert(_angleList, theta);
    BSL_TYPE retEx;

    switch (index)
    {
        case 0 :
            retEx = _ExList(AK47_CB_ROW - 1, index);
            break;

        case AK47_LEN_MAX:
            retEx = _ExList(AK47_CB_ROW - 1, AK47_LEN_MAX - 1);
            break;

        default:
            BSL_TYPE ratio = (theta - _angleList[index - 1]) / (_angleList[index] - _angleList[index - 1]);
            retEx = _ExList(AK47_CB_ROW - 1, index - 1) +
                    (_ExList(AK47_CB_ROW - 1, index) - _ExList(AK47_CB_ROW - 1, index - 1)) *
                    ratio;
            break;
    }

    return retEx;
}



BSL_TYPE NAMESPACE_AK47::NAME_AK47::get_ETheta_z( BSL_TYPE theta)
{
    int index = searchInsert(_angleList, theta);
    BSL_TYPE retEx;

    switch (index)
    {
        case 0 :
            retEx = _EzList(AK47_CB_ROW - 1, index);
            break;

        case AK47_LEN_MAX:
            retEx = _EzList(AK47_CB_ROW - 1, AK47_LEN_MAX - 1);
            break;

        default:
            BSL_TYPE ratio = (theta - _angleList[index - 1]) / (_angleList[index] - _angleList[index - 1]);
            retEx = _EzList(AK47_CB_ROW - 1, index - 1) +
                    (_EzList(AK47_CB_ROW - 1, index) - _EzList(AK47_CB_ROW - 1, index - 1)) *
                    ratio;
            break;
    }

    return retEx;
}

vector<BSL_TYPE> NAMESPACE_AK47::NAME_AK47::getDistanceList() const
{
    return _distanceList;
}

vector<BSL_TYPE> NAMESPACE_AK47::NAME_AK47::getAngleList() const
{
    return _angleList;
}


