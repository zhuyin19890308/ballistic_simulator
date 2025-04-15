



#include "ballistic_cannon.h"


ballistic_cannon::~ballistic_cannon()
{

}

ballistic_cannon::ballistic_cannon(BSL::BSL_Bullet_Name ammoName, BSL_TYPE theta0, BSL::BSL_Initialize_Para para)
    : ballistic(ammoName, theta0, para)
{
    bullet = bulletFactory.createAmmo(BulletName);
    this->setCb(theta0, BSL::CB_USE_THETA_LINEAR);
    this->setV0();

    this->setTheta(Theta0);
    BSL_Result_Init();
    setRotateMatrix();
}


ballistic_cannon::ballistic_cannon(BSL_Bullet_Name bulletName, BSL_Initialize_Para para)
    :ballistic(bulletName,0,para)
{
    bullet = bulletFactory.createAmmo(BulletName);
    std::cout << Initialize_Para.x_target << " " << Initialize_Para.y_target << " " << Initialize_Para.z_target << std::endl;
    BSL_TYPE  distance = calDistance(Initialize_Para);
    cout << "distance：" << distance << endl;
    std::vector<BSL_TYPE> distanceList = bullet.get()->getDistanceList();
    std::vector<BSL_TYPE> angleList = bullet.get()->getAngleList();
    BSL_TYPE distanceMax = bullet.get()->getMax(distanceList).second;

    if(distanceList.empty())
    {
        throw range_error("Failed to fetch distance list, check ammo library!");
    }

    if(distance > distanceMax)
    {
        throw logic_error("Exceed Maximum Distance ");
    }

    std::vector<int>index = bullet.get()->detectFireArea(distanceList, distance);

    if(index.size() == 1) 
    {
        BSL_TYPE angleStart{0};
        BSL_TYPE angleEnd{0};
        cout << index.front() << endl;

        
        if(index.front() == 0)
        {
            angleEnd = angleList.at(0);
        }
        else if(index.front() == static_cast<int>(angleList.size()))
        {
            angleStart = angleList.back();
            angleEnd = 1500;
        }
        else
        {
            angleStart = angleList.at(index.front() - 1);
            angleEnd = angleList.at(index.front());
        }

        cout << __FUNCTION__ << "_angleStart:" << angleStart << endl;
        cout << __FUNCTION__ << "_angleEnd:" << angleEnd << endl;
        Theta0 = iteration(angleStart, angleEnd, distance, 0.5);
        cout << __FUNCTION__ << "_Theta0:" << Theta0 << endl;
    }
    else  
    {
        if(distanceList.back() > distance)
        {
            BSL_TYPE angleStart{0};
            BSL_TYPE angleEnd{0};

            if(index.front() == 0)
            {
                angleEnd = angleList.at(0);
            }
            else
            {
                angleStart = angleList.at(index.front() - 1);
                angleEnd = angleList.at(index.front());
            }

            Theta0 = iteration(angleStart, angleEnd, distance, 0.5);
        }
        else if(distanceList.back() == distance)
        {
            Initialize_Para.useHighArea == false ? Theta0 = angleList.at(index.front()) : Theta0 = angleList.at(index.back());
        }
        else if(distanceList.back() < distance)
        {
            BSL_TYPE angleStart{0};
            BSL_TYPE angleEnd{0};
            unsigned long index_temp{0};
            Initialize_Para.useHighArea == false ? index_temp = index.front() : index_temp = index.back();

            if(!Initialize_Para.useHighArea)
            {
                angleStart = angleList.at(index_temp - 1);
                angleEnd = angleList.at(index_temp);
            }
            else
            {
                angleStart = angleList.at(index_temp);
                angleEnd = angleList.at(index_temp - 1);
            }

            Theta0 = iteration(angleStart, angleEnd, distance, 0.5);
        }
    }
    this->setCb(Theta0, BSL::CB_USE_THETA_LINEAR);
    this->setV0();
    this->setEtheta();
    this->setTheta(Theta0);
    BSL_Result_Init();
    setRotateMatrix();
}

BSL_TYPE ballistic_cannon::iteration(BSL_TYPE startAngle, BSL_TYPE endAngle, BSL_TYPE target, BSL_TYPE delta)
{
    BSL_TYPE dis {};
    double left{startAngle};
    double right{endAngle};
    double midAngle{};
    static int cnt = 0;
    BSL_TYPE delta_tmp{};
    std::cout.precision(10);
    BSL_queue previousData;

    do
    {
        cnt++;
        midAngle = (left + right) / 2.0;
        std::cout << "midAngle:" << midAngle << std::endl;
        dis = calOnce(midAngle);
        dis > target ? right = midAngle : left = midAngle;
        delta_tmp = fabs(dis - target);
        previousData << delta_tmp;
        previousData.print();
        std::cout << "Iterated：" << cnt << "times" << std::endl;
        std::cout << "Iterated value：" << delta_tmp << std::endl;
        std::cout << "previousData.mean()" << previousData.mean() << std::endl;

        if(previousData.count() == (int)previousData.len)
        {
            if(fabs(previousData.mean() - delta_tmp) < 0.1)
            {
                break;
            }
            else
            {
                continue;
            }
        }
    }
    while((previousData.miniValue > delta) );

    cnt = 0;
    return midAngle;
}


BSL_TYPE ballistic_cannon::calOnce(BSL_TYPE theta)
{

    BSL_Initialize_Para para=this->Initialize_Para;
    para.isSeparate=false;
    std::shared_ptr<ballistic> iter(new ballistic_cannon(this->BulletName, theta, para));
    int len = iter->calculate();
    BSL::BSL_Result result(len);
    iter->getResult(result);
    return result.pData[len - 1].Pos_x;
}


std::vector<BSL_TYPE> ballistic_cannon::_faster_dv_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod)
{
    
    BSL_TYPE cb = getCb(cbMethod);
    
    auto hy = _fun_II(y, IIMethod) * (BSL_VirtualTemp_Ground / (this->Initialize_Para.T - BSL_G1 * y));
    
    auto gv = _fun_Gv(vr, y);
    
    BSL_TYPE lat_updated = updateLatitude(Result_Rk4_Buffer.Pos_x, Initialize_Para.latitude);
    BSL_TYPE lat = angleConvert(lat_updated, BSL::CVT_ANGLE2RAD);
    BSL_TYPE beta = angleConvert(Initialize_Para.Rotate_y, BSL::CVT_MIL2RAD);
    auto ret_x = -1 * cb * hy * gv * (Result_Rk4_Buffer.v_x - Initialize_Para.Wx)
                 - 2 * BSL_Omiga * (Result_Rk4_Buffer.v_z * sin(lat) + Result_Rk4_Buffer.v_y * cos(lat) * sin(beta))
                 + (Result_Rk4_Buffer.v_x * Result_Rk4_Buffer.v_y) / (BSL_R_earth + y);
    auto ret_y = -1 * cb * hy * gv * (Result_Rk4_Buffer.v_y - Initialize_Para.Wy) - BSL_g
                 + 2 * BSL_Omiga *
                 (Result_Rk4_Buffer.v_x * cos(lat) * sin(beta) + Result_Rk4_Buffer.v_z * cos(lat) * cos(beta))
                 - (Result_Rk4_Buffer.v_x * Result_Rk4_Buffer.v_x) / (BSL_R_earth + y);
    auto ret_z = -1 * cb * hy * gv * (Result_Rk4_Buffer.v_z - Initialize_Para.Wz)
                 - 2 * BSL_Omiga * (Result_Rk4_Buffer.v_y * cos(lat) * cos(beta) - Result_Rk4_Buffer.v_x * sin(lat));
    std::vector<BSL_TYPE> ret;
    ret.push_back(static_cast<BSL_TYPE>(ret_x));
    ret.push_back(static_cast<BSL_TYPE>(ret_y));
    ret.push_back(static_cast<BSL_TYPE>(ret_z));
    return ret;
}


BSL_TYPE ballistic_cannon::updateLatitude(BSL_TYPE currentDistance, BSL_TYPE initLatitude, BSL::BSL_Earth_Part shootPos)
{
    BSL_TYPE shotAngle = angleConvert(this->Initialize_Para.Rotate_y, BSL::CVT_MIL2RAD);
    BSL_TYPE currentLatitude{};

    
    if (shootPos == NORTH_HEMISPHERE)
    {
        currentLatitude = initLatitude + currentDistance * cos(shotAngle) / 111000;
    }
    else if (shootPos == SOUTH_HEMISPHERE)
    {
        currentLatitude = initLatitude - currentDistance * cos(shotAngle) / 111000;
    }

    return currentLatitude;
}

unsigned long ballistic_cannon::_fun_RK4()
{
    
    if (!Result_Rk4.isEmpty(BSL::IGNORE_INIT_VALUE))
    {
        throw logic_error("Please get the previous data first");
    }

    BSL_TYPE time = 0;
    BSL_TYPE time_interval = 0.001;
    bool isTouchGround = false;  

    int saveCnt = Result_Rk4_Save_Interval;

    while (!isTouchGround)
    {
        BSL::BSL_RK4_Para para;
        std::vector<BSL_TYPE> dvVector = _faster_dv_dt(Result_Rk4_Buffer.Pos_y, Result_Rk4_Buffer.v_remain, true, false);
        
        BSL_TYPE dxdt = Result_Rk4_Buffer.v_x * (BSL_R_earth / (BSL_R_earth + Result_Rk4_Buffer.Pos_y));
        para.setValue(0, time_interval,
                      dvVector.at(0), dvVector.at(1), dvVector.at(2),
                      dxdt, Result_Rk4_Buffer.v_y, Result_Rk4_Buffer.v_z);
        
        para.setValue(1, time_interval,
                      0.5f * para.Kvx[0] + dvVector.at(0),
                      0.5f * para.Kvy[0] + dvVector.at(1),
                      0.5f * para.Kvz[0] + dvVector.at(2),
                      0.5f * para.Kx[0] + dxdt,
                      0.5f * para.Ky[0] + Result_Rk4_Buffer.v_y,
                      0.5f * para.Kz[0] + Result_Rk4_Buffer.v_z);
        
        para.setValue(2, time_interval,
                      0.5f * para.Kvx[1] + dvVector.at(0),
                      0.5f * para.Kvy[1] + dvVector.at(1),
                      0.5f * para.Kvz[1] + dvVector.at(2),
                      0.5f * para.Kx[1] + dxdt,
                      0.5f * para.Ky[1] + Result_Rk4_Buffer.v_y,
                      0.5f * para.Kz[1] + Result_Rk4_Buffer.v_z);
        
        para.setValue(3, time_interval,
                      para.Kvx[1] + dvVector.at(0),
                      para.Kvy[1] + dvVector.at(1),
                      para.Kvz[1] + dvVector.at(2),
                      para.Kx[1] + dxdt,
                      para.Ky[1] + Result_Rk4_Buffer.v_y,
                      para.Kz[1] + Result_Rk4_Buffer.v_z);
        

        std::shared_ptr<BSL_TYPE> sumResult = para.Sum();
        time += time_interval;
        
        Result_Rk4_Buffer.timeStamp = time;
        Result_Rk4_Buffer.v_x += sumResult.get()[0];
        Result_Rk4_Buffer.v_y += sumResult.get()[1];
        Result_Rk4_Buffer.v_z += sumResult.get()[2];
        Result_Rk4_Buffer.v_remain = _fun_Vr(Result_Rk4_Buffer.v_x, Result_Rk4_Buffer.v_y, Result_Rk4_Buffer.v_z);
        Result_Rk4_Buffer.Pos_x += sumResult.get()[3];
        Result_Rk4_Buffer.Pos_y += sumResult.get()[4];
        Result_Rk4_Buffer.Pos_z += sumResult.get()[5];
        
        BSL_TYPE Pos_xyz_[4] = {Result_Rk4_Buffer.Pos_x, Result_Rk4_Buffer.Pos_y, Result_Rk4_Buffer.Pos_z, 1};
        matrix::Matrix<BSL_TYPE, 4, 1> Pos_xyz(Pos_xyz_);
        matrix::Matrix<BSL_TYPE, 4, 1> Posw_xyz = rotateMatrix * Pos_xyz;
        Result_Rk4_Buffer.Pos_xw = Posw_xyz(0, 0);
        Result_Rk4_Buffer.Pos_yw = Posw_xyz(1, 0);
        Result_Rk4_Buffer.Pos_zw = Posw_xyz(2, 0);
        saveCnt--;

        if (!saveCnt)
        {
            Result_Rk4 << Result_Rk4_Buffer;
            saveCnt = Result_Rk4_Save_Interval;
        }

        this->updateInitPara(Posw_xyz(1, 0));


        if (Result_Rk4_Buffer.Pos_y <= Initialize_Para.gun_height ||
                Result_Rk4_Buffer.v_x <= 0)
        {
            if (saveCnt)
            {
                Result_Rk4 << Result_Rk4_Buffer;
            }

            isTouchGround = true;
        }
    }

    return Result_Rk4.size();
}

