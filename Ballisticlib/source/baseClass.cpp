#include "baseClass.h"


#include <utility>

using namespace std;

std::vector<BSL_TYPE> ballistic::randomData = std::vector<BSL_TYPE> {};

ballistic::ballistic(BSL::BSL_Bullet_Name bulletName, BSL_TYPE theta0, BSL::BSL_Initialize_Para para)
    : BulletName{bulletName}, Theta0{theta0}, Initialize_Para{para}
{

    bullet = bulletFactory.createAmmo(BulletName);
    this->setCb(theta0, BSL::CB_USE_THETA_LINEAR);
    this->setV0();
    this->setTheta(Theta0);
    BSL_Result_Init();
    setRotateMatrix();
}

ballistic::ballistic(BSL_Bullet_Name bulletName, BSL_Initialize_Para para)
    : BulletName{bulletName}, Initialize_Para{para}
{


    bullet = bulletFactory.createAmmo(BulletName);
    std::cout << Initialize_Para.x_target << " " << Initialize_Para.y_target << " " << Initialize_Para.z_target << std::endl;
    BSL_TYPE  distance = calDistance(Initialize_Para);
    cout << "Target Distance：" << distance << endl;
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

ballistic::~ballistic()
{

}

string ballistic::outString(const basic_string<char> &str, unsigned char setLen)
{
    string newStr = str;
    int len = str.size();

    if (len < setLen)
    {
        for (int i = 0; i < round((setLen - (len - 1) / 2.0) / 8.0); ++i)
        {
            newStr += '\t';
        }
    }

    return newStr;
}

void ballistic::writeHeader(ofstream &file, const string &writeTime)
{
    file.flags(ios::left);
    file << "----------------------------------------------------------------------------------------------------\n";
    file << "Record time: " + writeTime + "\n";
    file << "----------------------------------------------------------------------------------------------------\n";
    file << outString("Bullet Type", 40) << bullet.get()->getBulletName() + "\n";
    file << outString("Initial Velocity", 40) << to_string(V0) + "\n";
    file << outString("Ballistic Coefficient", 40) << to_string(Cb) + "\n";
    file << outString("Initial Angle", 40) << to_string(Theta0) + "\n";
    file << outString("Local Temperature", 40) << to_string(Initialize_Para.T) + "\n";
    file << outString("Crosswind", 40) << to_string(Initialize_Para.Wz) + "\n";
    file << outString("Vertical Wind", 40) << to_string(Initialize_Para.Wy) + "\n";
    file << outString("Longitudinal Wind", 40) << to_string(Initialize_Para.Wx) + "\n";
    file << outString("Ground Pressure", 40) << to_string(Initialize_Para.P0) + "\n";
    file << outString("Muzzle Coordinate X in World Coordinate System", 40) << to_string(Initialize_Para.xw) + "\n";
    file << outString("Muzzle Coordinate Y in World Coordinate System", 40) << to_string(Initialize_Para.yw) + "\n";
    file << outString("Muzzle Coordinate Z in World Coordinate System", 40) << to_string(Initialize_Para.zw) + "\n";
    file << outString("Body-to-Earth Coordinate Rotation Angle X", 40) << to_string(Initialize_Para.Rotate_x) + "\n";
    file << outString("Body-to-Earth Coordinate Rotation Angle Y", 40) << to_string(Initialize_Para.Rotate_y) + "\n";
    file << outString("Body-to-Earth Coordinate Rotation Angle Z", 40) << to_string(Initialize_Para.Rotate_z) + "\n";
    file << outString("Include Dispersion", 40) << to_string(Initialize_Para.isSeparate) + "\n";
    file << outString("Angle Error", 40) << to_string(Initialize_Para.E_theta_x) + "\n";
    file << outString("Azimuth Error", 40) << to_string(Initialize_Para.E_theta_z) + "\n";
    file << outString("Initial Velocity Error", 40) << to_string(Initialize_Para.E_v0) + "\n";
    file << outString("Ballistic Coefficient Error", 40) << to_string(Initialize_Para.E_cb) + "\n";
    file << outString("Vertical Parallax Correction Coefficient", 40) << to_string(Initialize_Para.verticalCorrection) + "\n";
    file << outString("Horizontal Parallax Correction Coefficient", 40) << to_string(Initialize_Para.horizontalCorrection) + "\n";
    file << outString("Iteration Cut-off Height", 40) << to_string(Initialize_Para.gun_height) + "\n";
    file << outString("Data Save Interval", 40) << to_string(Result_Rk4_Save_Interval) + "\n";
    file << "----------------------------------------------------------------------------------------------------\n";
}

void ballistic::writeContent(ofstream &file, BSL_Export_Method method) const
{
    if (method == BSL::EXP_ALL)
    {
        file << "Timestamp" << "\t" << "\t"
             << "Velocity x" << "\t" << "\t"
             << "Velocity y" << "\t" << "\t"
             << "Velocity z" << "\t" << "\t"
             << "Velocity" << "\t" << "\t"
             << "Local Coord x" << "\t"
             << "Local Coord y" << "\t"
             << "Local Coord z" << "\t"
             << "World Coord x" << "\t"
             << "World Coord y" << "\t"
             << "World Coord z" << "\n";

        for (int i = 0; i < Result_Rk4.size(); ++i)
        {
            file << to_string(Result_Rk4[i].timeStamp) << "\t"
                 << to_string(Result_Rk4[i].v_x) << "\t"
                 << to_string(Result_Rk4[i].v_y) << "\t"
                 << to_string(Result_Rk4[i].v_z) << "\t"
                 << to_string(Result_Rk4[i].v_remain) << "\t"
                 << to_string(Result_Rk4[i].Pos_x) << "\t"
                 << to_string(Result_Rk4[i].Pos_y) << "\t"
                 << to_string(Result_Rk4[i].Pos_z) << "\t"
                 << to_string(Result_Rk4[i].Pos_xw) << "\t"
                 << to_string(Result_Rk4[i].Pos_yw) << "\t"
                 << to_string(Result_Rk4[i].Pos_zw) + "\n";
        }
    }

    if (method == BSL::EXP_ABSOLUTE_POS)
    {
file << "Timestamp" << "\t" << "\t"
        << "Storage Speed" << "\t" << "\t"
        << "World Coordinate x" << "\t"
        << "World Coordinate y" << "\t"
        << "World Coordinate z" << "\n";

        for (int i = 0; i < Result_Rk4.size(); ++i)
        {
            file << to_string(Result_Rk4[i].timeStamp) << "\t"
                 << to_string(Result_Rk4[i].v_remain) << "\t"
                 << to_string(Result_Rk4[i].Pos_xw) << "\t"
                 << to_string(Result_Rk4[i].Pos_yw) << "\t"
                 << to_string(Result_Rk4[i].Pos_zw) + "\n";
        }
    }

    if (method == BSL::EXP_RELATIVE_POS)
    {
file << "Timestamp" << "\t" << "\t"
     << "Stored Speed x" << "\t" << "\t"
     << "Stored Speed y" << "\t" << "\t"
     << "Stored Speed z" << "\t" << "\t"
     << "Stored Speed" << "\t" << "\t"
     << "Local Coordinate x" << "\t"
     << "Local Coordinate y" << "\t"
     << "Local Coordinate z" << "\n";

        for (int i = 0; i < Result_Rk4.size(); ++i)
        {
            file << to_string(Result_Rk4[i].timeStamp) << "\t"
                 << to_string(Result_Rk4[i].v_x) << "\t"
                 << to_string(Result_Rk4[i].v_y) << "\t"
                 << to_string(Result_Rk4[i].v_z) << "\t"
                 << to_string(Result_Rk4[i].v_remain) << "\t"
                 << to_string(Result_Rk4[i].Pos_x) << "\t"
                 << to_string(Result_Rk4[i].Pos_y) << "\t"
                 << to_string(Result_Rk4[i].Pos_z) << "\n";
        }
    }
}

void ballistic::writeData(const string &path, BSL::BSL_Export_Method method)
{
    if (Result_Rk4.isEmpty(BSL::IGNORE_INIT_VALUE))
    {
        throw logic_error("ballistic result has been moved!");
        return;
    }

    string time = getCurrentTime();
    string fileName = time + "-" +
                      bullet.get()->getBulletName() + "-" +
                      to_string(bullet.get()->getSpeed()) + "-" +
                      to_string(Cb);
    cout << fileName << endl;
    ofstream fileOut;
    fileOut.open(path + fileName + ".txt");
    writeHeader(fileOut, time);
    writeContent(fileOut, method);
    fileOut.close();
}

std::vector<BSL_TYPE> ballistic::getGlobalCoordinatePOS()
{
    return std::vector<BSL_TYPE>();
}

std::shared_ptr<BSL::weatherMsgStruct> ballistic::decodeWethearMessage(const string &message)
{
    weatherFactory = make_shared<weatherMsgDecoderFactory>(weatherMsgDecoderFactory(message, this->bullet));
    weatherDecoder = weatherFactory->createDecoder();
    decodedWeatherMsg = weatherDecoder->getDecodedMsg();
    return decodedWeatherMsg;
}

void ballistic::setCb(BSL_TYPE value, BSL::BSL_Cb_Method method)
{
    BSL_TYPE rd = getOneRandomNum(this->Initialize_Para.E_cb);

    switch (method)
    {
        case BSL::CB_USE_VALUE:
        {
            ballistic::Cb = value + static_cast<BSL_TYPE>(this->Initialize_Para.isSeparate) * rd;
            break;
        }
use_linear:
        case BSL::CB_USE_THETA_LINEAR   :
            try
            {
            ballistic::Cb = bullet.get()->getCb(value)
                                + static_cast<BSL_TYPE>(this->Initialize_Para.isSeparate) * rd;
            }
            catch (...)
            {
                std::cout << "Fail to initialize PARAMETER: Cb!";
            }
            break;

        default:
            goto use_linear;
    }
}

BSL_TYPE ballistic::getCb(bool byCalculate) const
{
    if (byCalculate)
    {
        return ballistic::Cb;
    }
    else
    {
        if (Initialize_Para.i == 0 || Initialize_Para.m == 0 || Initialize_Para.d == 0)
        {
            cout << "Check i,m,d!" << endl;
            throw std::invalid_argument("invalid clear para!");
        }
        else
        {
            return static_cast<BSL_TYPE>(Initialize_Para.i * pow(Initialize_Para.d, 2) * pow(10, 3) / Initialize_Para.m);
        }
    }
}

string ballistic::version()
{
    string VERSION_MAJOR, VERSION_MINOR, VERSION_REVISE, VERSION_BUILD;
    string versionRet;
    VERSION_MAJOR = to_string(BSL_VERSION_MAJOR);
    VERSION_MINOR = to_string(BSL_VERSION_MINOR);
    VERSION_REVISE = to_string(BSL_VERSION_REVISE);
    VERSION_BUILD = to_string(BSL_VERSION_BUILD);
    versionRet = VERSION_MAJOR + "." + VERSION_MINOR + "." + VERSION_REVISE + "." + VERSION_BUILD;
    return versionRet;
}

vector<BSL_TYPE>
ballistic::generateRandomNum(BSL_TYPE inputParaA, BSL_TYPE inputParaB, int dataLen, BSL::BSL_Random_Method method)
{
    randomData.resize(0);
    std::random_device rd; 
    std::default_random_engine generator{rd()};

    switch (method)
    {
        case BSL::RANDOM_UNIFORM:
        {
            std::uniform_real_distribution<BSL_TYPE> distribution(inputParaA, inputParaB);

            for (int timeStart = 0; timeStart < dataLen; ++timeStart)
            {
                randomData.push_back(distribution(generator));
            }

            break;
        }

defaultLabel:

        case BSL::RANDOM_NORMAL:
        {
            std::normal_distribution<BSL_TYPE> distribution(inputParaB, inputParaA);

            for (int timeStart = 0; timeStart < dataLen; ++timeStart)
            {
                randomData.push_back(distribution(generator));
            }

            break;
        }

        case BSL::RANDOM_BINORMAL:
        {
            std::binomial_distribution<int> distribution(static_cast<int>(inputParaA), inputParaB);

            for (int timeStart = 0; timeStart < dataLen; ++timeStart)
            {
                randomData.push_back(static_cast<BSL_TYPE>(distribution(generator)));
            }

            break;
        }

        default:
            goto defaultLabel;
    }

    return randomData;
}

void ballistic::showRandomNumDistribution()
{
    if (randomData.empty())
    {
        cout << "empty random data vector!" << endl;
        return;
    }

    auto len = randomData.size();
    
    int p[50] = {};

    for (int i = 0; i < len; ++i)
    {
        double number = randomData.at(i);

        if ((number >= 3488.0) && (number < 3512.0))
        {
            ++p[(int) number + 12];
        }
    }

    for (int i = 0; i < 10; ++i)
    {
        std::cout << i << "-" << (i + 1) << ": ";
        std::cout << std::string(p[i] * 100 / len, '*') << p[i] << std::endl;
    }

    
}

int ballistic::getCurrentSecond()
{
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    int currentSecond = ltm->tm_hour * 3600 + ltm->tm_min * 60 + ltm->tm_sec;
    return currentSecond;
}

BSL_TYPE ballistic::getOneRandomNum()
{
    int currentSecond = getCurrentSecond();
    int len = static_cast<int>(randomData.size());
    currentSecond = (currentSecond > len ? len : currentSecond);

    try
    {
        BSL_TYPE ret = randomData.at(currentSecond);
        return ret;
    }
    catch (...)
    {
        cout << "empty random num vector!"
             << " or data underrun!" << endl;
        return 0;
    }
}

BSL_TYPE ballistic::getOneRandomNum(BSL_TYPE inputParaA, BSL_TYPE inputParaB, BSL::BSL_Random_Method method)
{
    generateRandomNum(inputParaA, inputParaB, 1, method);
    return randomData.back();
}

BSL_TYPE ballistic::getV0() const
{
    return V0;
}

void ballistic::setV0()
{
    BSL_TYPE v_tmp = bullet.get()->getSpeed();
    BSL_TYPE rd = getOneRandomNum(this->Initialize_Para.E_v0);
    this->V0 = v_tmp + static_cast<BSL_TYPE>(this->Initialize_Para.isSeparate) * rd;
}

BSL_TYPE ballistic::getTheta0() const
{
    return Theta0;
}

BSL_TYPE ballistic::getTheta() const
{
    return Theta;
}

void ballistic::setTheta(BSL_TYPE theta)
{
    Theta0 = theta;
    BSL_TYPE rd = getOneRandomNum(this->Initialize_Para.E_theta_x);
    this->Theta = Theta0 + static_cast<BSL_TYPE>(this->Initialize_Para.isSeparate) * rd;
}

void ballistic::BSL_Result_Init()
{
    Result_Rk4.BSL_Result_Clear();
    BSL::BSL_Point initPoint;
    
    initPoint.timeStamp = 0;
    
    initPoint.v_x = V0 * cos(angleConvert(
                                 static_cast<BSL_TYPE>(Initialize_Para.isSeparate) * getOneRandomNum(Initialize_Para.E_theta_z),
                                 BSL::CVT_MIL2RAD)) * cos(angleConvert(Theta, BSL::CVT_MIL2RAD));
    initPoint.v_y = V0 * cos(angleConvert(
                                 static_cast<BSL_TYPE>(Initialize_Para.isSeparate) * getOneRandomNum(Initialize_Para.E_theta_z),
                                 BSL::CVT_MIL2RAD)) * sin(angleConvert(Theta, BSL::CVT_MIL2RAD));
    initPoint.v_z = V0 * sin(angleConvert(
                                 static_cast<BSL_TYPE>(Initialize_Para.isSeparate) * getOneRandomNum(Initialize_Para.E_theta_z),
                                 BSL::CVT_MIL2RAD));
    
    initPoint.v_remain = getV0();
    
    initPoint.Pos_x = 0;
    initPoint.Pos_y = 0;
    initPoint.Pos_z = 0;
    
    initPoint.Pos_xw = Initialize_Para.xw;
    initPoint.Pos_yw = Initialize_Para.yw;
    initPoint.Pos_zw = Initialize_Para.zw;
    
    Result_Rk4 << initPoint;
    Result_Rk4_Buffer = initPoint;
}

BSL_TYPE ballistic::angleConvert(BSL_TYPE srcAngle, BSL::BSL_Convert_Angle method)
{
    BSL_TYPE ret;

    switch (method)
    {
        case BSL::CVT_ANGLE2MIL:
        {
            ret = srcAngle * 6000.0f / 360.0f;
            break;
        }

        case BSL::CVT_ANGLE2RAD:
        {
            ret = srcAngle * BSL_PI / 180.0f;
            break;
        }

        case BSL::CVT_MIL2ANGLE:
        {
            ret = srcAngle * 360.0f / 6000.0f;
            break;
        }

        case BSL::CVT_MIL2RAD:
        {
            ret = (srcAngle * 360.0f / 6000.0f) * BSL_PI / 180.0f;
            break;
        }

        case BSL::CVT_RAD2ANGLE:
        {
            ret = srcAngle * 180.0f / BSL_PI;
            break;
        }

        case BSL::CVT_RAD2MIL:
        {
            ret = (srcAngle * 180.0f / BSL_PI) * 6000.0f / 360.0f;
            break;
        }

        default:
            ret = srcAngle;
            break;
    }

    return ret;
}

BSL_TYPE ballistic::_fun_II(BSL_TYPE y, bool isStandard) const
{
    double ret = 0;

    if (y <= 9300)
    {
        ret = pow(1 - 2.1905f * pow(10, -5) * y, 5.4f);
    }
    else if (y > 9300 && y <= 12000)
    {
        ret = 0.29228f * exp(-2.12064f * (atan((2.344f * (y - 9300) - 6328) / 32221) + 0.193925f));
    }
    else
    {
        ret = 0.19372f * exp(-1 * (y - 12000) / 6483.3f);
    }

    if (isStandard)
    {
        return static_cast<BSL_TYPE>(ret);
    }
    else
    {
        if (Initialize_Para.P0 == 0)
        {
            cout << "Check P0!" << endl;
            throw std::invalid_argument("Invalid P0");
        }
        else
        {
            return static_cast<BSL_TYPE>(ret * Initialize_Para.P0 / BSL_Pon);
        }
    }
}

BSL_TYPE ballistic::_soundSpeed(const BSL_TYPE &yw) const
{
    BSL_TYPE Tv1 = BSL_VirtualTemp_Ground - BSL_G1 * yw;
    auto ret = static_cast<BSL_TYPE>(sqrt(BSL_K * BSL_R * Tv1));
    return ret;
}

BSL_TYPE ballistic::_fun_Fv(const BSL_TYPE &vt)
{
    BSL_TYPE ret{};

    if (vt < 250)
    {
        ret = static_cast<BSL_TYPE>(0.00007454f * pow(vt, 2));
    }
    else if ((vt < 400) && (vt >= 250))
    {
        ret = static_cast<BSL_TYPE>(629.61f - 6.0255f * vt + 1.8756f * pow(10, -2) * pow(vt, 2) -
                                    1.8613f * pow(10, -5) * pow(vt, 3));
    }
    else if ((vt <= 1400) && (vt >= 400))
    {
        ret = static_cast<BSL_TYPE>(6.394f * pow(10, -8) * pow(vt, 3) - 6.325f * pow(10, -5) * pow(vt, 2)
                                    + 0.1548f * vt - 26.63f);
    }
    else if (vt > 1400)
    {
        ret = static_cast<BSL_TYPE>(0.00012315 * pow(vt, 2));
    }

    return ret;
}

BSL_TYPE ballistic::_fun_Gv(const BSL_TYPE &vr, const BSL_TYPE &y)
{
    BSL_TYPE ret{};
    
    auto vt = static_cast<BSL_TYPE>(vr * sqrt(BSL_VirtualTemp_Ground / (BSL_VirtualTemp_Ground - BSL_G1 * y)));
    ret = _fun_Fv(vt) / vt;
    return ret;
}

BSL_TYPE ballistic::_fun_Vr(const BSL_TYPE &vx, const BSL_TYPE &vy, const BSL_TYPE &vz) const
{
    auto ret = static_cast<BSL_TYPE>(sqrt(pow(vx - Initialize_Para.Wx, 2) +
                                          pow(vy - Initialize_Para.Wy, 2) +
                                          pow(vz - Initialize_Para.Wz, 2)));
    return ret;
}

BSL_TYPE ballistic::_dvx_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod)
{

    BSL_TYPE cb = getCb(cbMethod);

    

    auto hy = static_cast<BSL_TYPE>(_fun_II(y, IIMethod) *
                                    (BSL_VirtualTemp_Ground / (this->Initialize_Para.T - BSL_G1 * y)));

    BSL_TYPE gv = _fun_Gv(vr, y);
    BSL_TYPE ret = -1 * cb * hy * gv * (Result_Rk4.v_x.back() - Initialize_Para.Wx);
    return ret;
}

BSL_TYPE ballistic::_dvy_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod)
{
    
    BSL_TYPE cb = getCb(cbMethod);
    
    auto hy = static_cast<BSL_TYPE>(_fun_II(y, IIMethod) *
                                    (BSL_VirtualTemp_Ground / (this->Initialize_Para.T - BSL_G1 * y)));
    
    BSL_TYPE gv = _fun_Gv(vr, y);
    BSL_TYPE ret = -1 * cb * hy * gv * (Result_Rk4.v_y.back() - Initialize_Para.Wy) - BSL_g;
    return ret;
}

BSL_TYPE ballistic::_dvz_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod)
{
    
    BSL_TYPE cb = getCb(cbMethod);
    
    auto hy = static_cast<BSL_TYPE>(_fun_II(y, IIMethod) *
                                    (BSL_VirtualTemp_Ground / (this->Initialize_Para.T - BSL_G1 * y)));
    
    BSL_TYPE gv = _fun_Gv(vr, y);
    BSL_TYPE ret = -1 * cb * hy * gv * (Result_Rk4.v_z.back() - Initialize_Para.Wz);
    return ret;
}


std::vector<BSL_TYPE> ballistic::_faster_dv_dt(const BSL_TYPE &y, const BSL_TYPE &vr, bool cbMethod, bool IIMethod)
{
    
    BSL_TYPE cb = getCb(cbMethod);
    
    auto hy = _fun_II(y, IIMethod) * (BSL_VirtualTemp_Ground / (this->Initialize_Para.T - BSL_G1 * y));
    
    auto gv = _fun_Gv(vr, y);
    auto ret_x = -1 * cb * hy * gv * (Result_Rk4_Buffer.v_x - Initialize_Para.Wx);
    auto ret_y = -1 * cb * hy * gv * (Result_Rk4_Buffer.v_y - Initialize_Para.Wy) - BSL_g;
    auto ret_z = -1 * cb * hy * gv * (Result_Rk4_Buffer.v_z - Initialize_Para.Wz);
    std::vector<BSL_TYPE> ret;
    ret.push_back(static_cast<BSL_TYPE>(ret_x));
    ret.push_back(static_cast<BSL_TYPE>(ret_y));
    ret.push_back(static_cast<BSL_TYPE>(ret_z));
    return ret;
}

unsigned long ballistic::_fun_RK4()
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

unsigned long ballistic::calculate()
{

    unsigned long len = 0;
    len = _fun_RK4();
    return len;
}

void ballistic::getResult(BSL::BSL_Result &dst)
{
    if (Result_Rk4.isEmpty(BSL::DEFAULT))
    {
        return;
    }

    Result_Rk4.get_BSL_Result(dst);
    BSL_Result_Init();
}

matrix::Matrix<BSL_TYPE, 4, 4> ballistic::calRotateMat()
{
    BSL_TYPE Rotate_x = angleConvert(Initialize_Para.Rotate_x, BSL::CVT_MIL2RAD);
    BSL_TYPE Rotate_y = angleConvert(Initialize_Para.Rotate_y + Initialize_Para.horizontalCorrection, BSL::CVT_MIL2RAD);
    BSL_TYPE Rotate_z = angleConvert(Initialize_Para.Rotate_z + Initialize_Para.verticalCorrection, BSL::CVT_MIL2RAD);
    BSL_TYPE t_31[4] = {Initialize_Para.xw,
                        Initialize_Para.yw,
                        Initialize_Para.zw,
                        0
                       };
    BSL_TYPE rot_x[16] = {1, 0, 0, 0,
                          0, cos(Rotate_x), -1 * sin(Rotate_x), 0,
                          0, sin(Rotate_x), cos(Rotate_x), 0,
                          0, 0, 0, 0
                         };
    BSL_TYPE rot_y[16] = {cos(Rotate_y), 0, sin(Rotate_y), 0,
                          0, 1, 0, 0,
                          -1 * sin(Rotate_y), 0, cos(Rotate_y), 0,
                          0, 0, 0,
                         };
    BSL_TYPE rot_z[16] = {cos(Rotate_z), -1 * sin(Rotate_z), 0, 0,
                          sin(Rotate_z), cos(Rotate_z), 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 0
                         };
    matrix::Matrix<BSL_TYPE, 4, 1> t(t_31);
    matrix::Matrix<BSL_TYPE, 4, 4> rotateMatrix_x(rot_x), rotateMatrix_y(rot_y), rotateMatrix_z(rot_z);
    matrix::Matrix<BSL_TYPE, 4, 4> rotateMatrix_temp = rotateMatrix_x * rotateMatrix_y * rotateMatrix_z;
    rotateMatrix_temp.setCol(3, t);
    return rotateMatrix_temp;
}

void ballistic::setHorizantalCorrection(BSL_TYPE targetDistance, BSL_TYPE opticalDistance)
{
    BSL_TYPE correction = atan(opticalDistance / targetDistance);
    this->Initialize_Para.horizontalCorrection = correction;
}

void ballistic::setVerticalCorrection(BSL_TYPE targetDistance, BSL_TYPE opticalDistance)
{
    BSL_TYPE correction = atan(opticalDistance / targetDistance);
    this->Initialize_Para.verticalCorrection = correction;
}

const matrix::Matrix<BSL_TYPE, 4, 4> &ballistic::getRotateMatrix() const
{
    return rotateMatrix;
}

void ballistic::setRotateMatrix()
{
    ballistic::rotateMatrix = this->calRotateMat();
}

void ballistic::setInitializePara(const BSL::BSL_Initialize_Para &initializePara)
{
    Initialize_Para = initializePara;
    setRotateMatrix();
}

string ballistic::getCurrentTime()
{
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    
    string time;
    string year = to_string(1900 + ltm->tm_year) + "-";
    string month = to_string(1 + ltm->tm_mon) + "-";
    string day = to_string(ltm->tm_mday) + "-";
    string hour = to_string(ltm->tm_hour) + "-";
    string minute = to_string(ltm->tm_min) + "-";
    string second = to_string(ltm->tm_sec);
    time = year + month + day + hour + minute + second;
    return time;
}

void ballistic::updateInitPara(BSL_TYPE yw)
{
    if (decodedWeatherMsg == nullptr)
    {
        return;
    }

    if (decodedWeatherMsg->isEmpty())
    {
        return;
    }
    else
    {
        BSL::singleWeatherMsg msgTmp = decodedWeatherMsg->getNearestWeather(yw);
        this->Initialize_Para.T = msgTmp.temperature;
        this->Initialize_Para.Wz = (weatherDecoder->readWindSpeedSheet
                                    (msgTmp.windSpeed, Initialize_Para.Rotate_y - msgTmp.windAngle)).at(1);
        this->Initialize_Para.Wx = (weatherDecoder->readWindSpeedSheet
                                    (msgTmp.windSpeed, Initialize_Para.Rotate_y - msgTmp.windAngle)).at(0);
        this->Initialize_Para.P0 = decodedWeatherMsg->weather_station_pressure;
    }
}

int ballistic::getResultRk4SaveInterval() const
{
    return Result_Rk4_Save_Interval;
}

void ballistic::setResultRk4SaveInterval(int resultRk4SaveInterval)
{
    Result_Rk4_Save_Interval = resultRk4SaveInterval;
}


ballistic::Result::Result()
{
    BSL_Result_Clear();
}

ballistic::Result::~Result()
{
    BSL_Result_Clear();
}


void ballistic::Result::BSL_Result_Clear()
{
    this->timeStamp.clear();
    this->v_x.clear();                                
    this->v_y.clear();                                
    this->v_z.clear();                                
    this->v_remain.clear();                           
    this->Pos_x.clear();                              
    this->Pos_y.clear();                              
    this->Pos_z.clear();                              
    this->Pos_xw.clear();                             
    this->Pos_yw.clear();                             
    this->Pos_zw.clear();                             
}

void ballistic::Result::get_BSL_Result(BSL::BSL_Result &res) const
{
    int len = static_cast<int>(this->size());

    if (res.len != len)
    {
        std::cout << "fatal error: check data length" << endl;
        return;
    }

    for (int i = 0; i < len; ++i)
    {
        (res.pData)[i] = (*this)[i];
    }
}

BSL::BSL_Point ballistic::Result::operator[](unsigned int i) const
{
    BSL::BSL_Point Point;

    if (i >= this->timeStamp.size())
    {
        throw std::out_of_range("out of range");
    }
    else
    {
        Point.timeStamp = this->timeStamp.at(i);
        Point.v_x = this->v_x.at(i);
        Point.v_y = this->v_y.at(i);
        Point.v_z = this->v_z.at(i);
        Point.v_remain = this->v_remain.at(i);
        Point.Pos_x = this->Pos_x.at(i);
        Point.Pos_y = this->Pos_y.at(i);
        Point.Pos_z = this->Pos_z.at(i);
        Point.Pos_xw = this->Pos_xw.at(i);
        Point.Pos_yw = this->Pos_yw.at(i);
        Point.Pos_zw = this->Pos_zw.at(i);
        return Point;
    }
}

void ballistic::Result::operator<<(BSL::BSL_Point &point)
{
    this->timeStamp.push_back(point.timeStamp);
    this->v_x.push_back(point.v_x);
    this->v_y.push_back(point.v_y);
    this->v_z.push_back(point.v_z);
    this->v_remain.push_back(point.v_remain);
    this->Pos_x.push_back(point.Pos_x);
    this->Pos_y.push_back(point.Pos_y);
    this->Pos_z.push_back(point.Pos_z);
    this->Pos_xw.push_back(point.Pos_xw);
    this->Pos_yw.push_back(point.Pos_yw);
    this->Pos_zw.push_back(point.Pos_zw);
}

bool ballistic::Result::isEmpty(BSL::BSL_Result_Is_Empty_Flag flag) const
{
    bool ret;

    switch (flag)
    {
        case BSL::DEFAULT:
        {
            ret = timeStamp.empty() && v_x.empty() && v_y.empty() && v_z.empty() &&
                  v_remain.empty() && Pos_x.empty() && Pos_y.empty() && Pos_z.empty() &&
                  Pos_xw.empty() && Pos_yw.empty() && Pos_zw.empty();
            break;
        }

        case BSL::IGNORE_INIT_VALUE:
        {
            ret = timeStamp.size() == 1 &&
                  v_x.size() == 1 &&
                  v_y.size() == 1 &&
                  v_z.size() == 1 &&
                  v_remain.size() == 1 &&
                  Pos_x.size() == 1 &&
                  Pos_y.size() == 1 &&
                  Pos_z.size() == 1 &&
                  Pos_xw.size() == 1 &&
                  Pos_yw.size() == 1 &&
                  Pos_zw.size() == 1;
            break;
        }

        default:
            ret = true;
            break;
    }

    return ret;
}

unsigned long ballistic::Result::size() const
{
    return this->timeStamp.size();
}


void ballistic::setEtheta()
{
    if (Initialize_Para.isSeparate)
    {
        Initialize_Para.E_theta_x = bullet.get()->get_ETheta_x(Theta0);
        Initialize_Para.E_theta_z = bullet.get()->get_ETheta_z(Theta0);
    }
}

BSL_TYPE ballistic::calDistance(BSL_Initialize_Para &para)
{
    BSL_TYPE res = sqrt(pow(para.xw - para.x_target, 2) +
                        pow(para.yw - para.y_target, 2) +
                        pow(para.zw - para.z_target, 2));
    return res;
}

BSL_TYPE ballistic::calDistance(BSL_TYPE &x, BSL_TYPE &y, BSL_TYPE &z, BSL_TYPE &x_target, BSL_TYPE &y_target, BSL_TYPE &z_target)
{
    BSL_TYPE res = sqrt(pow(x - x_target, 2) +
                        pow(y - y_target, 2) +
                        pow(z - z_target, 2));
    return res;
}

BSL_TYPE ballistic::iteration(BSL_TYPE startAngle, BSL_TYPE endAngle, BSL_TYPE target, BSL_TYPE delta)
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
        std::cout << "迭代了：" << cnt << "次" << std::endl;
        std::cout << "迭代差值：" << delta_tmp << std::endl;
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


BSL_TYPE ballistic::calOnce(BSL_TYPE theta)
{

    BSL_Initialize_Para para=this->Initialize_Para;
    para.isSeparate=false;
    std::shared_ptr<ballistic> iter(new ballistic(this->BulletName, theta, para));
    int len = iter->calculate();
    BSL::BSL_Result result(len);
    iter->getResult(result);
    return result.pData[len - 1].Pos_x;
}


BSL_TYPE ballistic::getTime(BSL_TYPE distance)
{
    return bullet.get()->getTime(distance);
}
