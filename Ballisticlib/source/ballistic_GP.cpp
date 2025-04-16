#define END -1

#include "ballistic_GP.h"

ballistic_GP::ballistic_GP(BSL::BSL_Bullet_Name ammoName, BSL_TYPE theta0, BSL_TYPE distance, BSL_TYPE alpha,
                           BSL::BSL_Initialize_Para para) :
    ballistic(ammoName, theta0, para)
{
    bullet = bulletFactory.createAmmo(BulletName);
    this->setCb(distance, alpha, BSL::CB_USE_THETA_BILINEAR);
    this->setV0();
    this->setTheta(Theta0);
    BSL_Result_Init();
    setRotateMatrix();
}

ballistic_GP::~ballistic_GP()
{

}

void ballistic_GP::setCb(BSL_TYPE distance, BSL_TYPE alpha, BSL::BSL_Cb_Method method)  
{
    BSL_TYPE rd = getOneRandomNum(this->Initialize_Para.E_cb);

    switch (method)
    {
        case BSL::CB_USE_THETA_BILINEAR:

            try
            {
                ballistic::Cb = bullet.get()->getCb(distance, alpha)
                                + static_cast<BSL_TYPE>(this->Initialize_Para.isSeparate) * rd;
            }
            catch (...)
            {
                std::cout << "Fail to initialize PARAMETER: Cb!";
            }

            break;

        default:
            break;
    }
}

unsigned long ballistic_GP::_fun_RK4()
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
        
        para.setValue(0, time_interval,
                      dvVector.at(0), dvVector.at(1), dvVector.at(2),
                      Result_Rk4_Buffer.v_x, Result_Rk4_Buffer.v_y, Result_Rk4_Buffer.v_z);
        
        para.setValue(1, time_interval,
                      0.5f * para.Kvx[0] + dvVector.at(0),
                      0.5f * para.Kvy[0] + dvVector.at(1),
                      0.5f * para.Kvz[0] + dvVector.at(2),
                      0.5f * para.Kx[0] + Result_Rk4_Buffer.v_x,
                      0.5f * para.Ky[0] + Result_Rk4_Buffer.v_y,
                      0.5f * para.Kz[0] + Result_Rk4_Buffer.v_z);
        
        para.setValue(2, time_interval,
                      0.5f * para.Kvx[1] + dvVector.at(0),
                      0.5f * para.Kvy[1] + dvVector.at(1),
                      0.5f * para.Kvz[1] + dvVector.at(2),
                      0.5f * para.Kx[1] + Result_Rk4_Buffer.v_x,
                      0.5f * para.Ky[1] + Result_Rk4_Buffer.v_y,
                      0.5f * para.Kz[1] + Result_Rk4_Buffer.v_z);
        
        para.setValue(3, time_interval,
                      para.Kvx[1] + dvVector.at(0),
                      para.Kvy[1] + dvVector.at(1),
                      para.Kvz[1] + dvVector.at(2),
                      para.Kx[1] + Result_Rk4_Buffer.v_x,
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
                Result_Rk4_Buffer.v_x <= 0 ||
                (this->Theta > 150 &&
                 (fabs(Result_Rk4_Buffer.v_y) < 0.5 ||
                  Result_Rk4_Buffer.Pos_x * Result_Rk4_Buffer.Pos_x +
                  Result_Rk4_Buffer.Pos_y * Result_Rk4_Buffer.Pos_y +
                  Result_Rk4_Buffer.Pos_z * Result_Rk4_Buffer.Pos_z > 36000000.0)))
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


