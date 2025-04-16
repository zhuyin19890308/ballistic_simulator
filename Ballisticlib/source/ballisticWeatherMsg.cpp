
#include "ballisticWeatherMsg.h"

decoder0000::decoder0000(const std::vector<string> &splitedMsg, std::shared_ptr<ammoBase> bulletHandle)
{
    this->msgSplited = splitedMsg;
    this->bullet = bulletHandle;
    decode();
}

bool decoder0000::validationCheck()
{
    if (msgSplited.empty() || msgSplited.size() < 7)
    {
        throw length_error("0000: incomplete weather message!");
    }
    for (int cnt = 0; cnt < msgSplited.size(); ++cnt)
    {
        try
        {
            boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        catch (boost::bad_lexical_cast &e)
        {
            cout << e.what() << endl;
            throw e;
        }

        if ((cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3) && (msgSplited.at(cnt).size() != 4))
        {
            throw length_error("Invalid length!");
        }
        else if ((cnt == 4 || cnt == 6) && (msgSplited.at(cnt).size() != 3))
        {
            throw length_error("Invalid length!");
        }
        else if (cnt == 5 && (msgSplited.at(cnt).size() != 2))
        {
            throw length_error("Invalid length!");
        }
    }

    
    if (boost::lexical_cast<int>(msgSplited.at(1).substr(0, 2)) > 24 ||
            boost::lexical_cast<int>(msgSplited.at(1).substr(2, 2)) >= 60)
    {
        throw logic_error("Invalid time message!");
    }

    return true;
}

void decoder0000::decode()
{
    try
    {
        validationCheck();
    }
    catch (std::exception &e)
    {
        cout << "Invalid weather message: " << e.what() << endl;
        throw e;
    }

    int msgLen = static_cast<int>(msgSplited.size());                   
    
    msgDecoded = std::make_shared<BSL::weatherMsgStruct>(BSL::weatherMsgStruct());
    BSL::singleWeatherMsg msg;

    for (int cnt = 0; cnt < msgLen; ++cnt)
    {
        if (cnt == 0)
        {
            msgDecoded->messageType = BSL::WEATHER_TYPE_0000;
        }
        else if (cnt == 1)
        {
            msgDecoded->time = msgSplited.at(cnt).substr(0, 2) + ":" + msgSplited.at(cnt).substr(2, 2);
        }
        else if (cnt == 2)
        {
            msgDecoded->weather_station_height = boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        else if (cnt == 3)
        {
            msgDecoded->weather_station_pressure = (boost::lexical_cast<BSL_TYPE>(msgSplited.at(cnt)) * 0.1f) * 100 / 760;
        }
        else if (cnt == 4)
        {
            msgDecoded->weather_station_temperature = boost::lexical_cast<BSL_TYPE>(msgSplited.at(cnt)) * 0.1f + 273.15f;
        }
        else if (cnt == 5)
        {
            msg.height = msgDecoded->weather_station_height;
            msg.temperature = msgDecoded->weather_station_temperature;
            msg.windAngle = boost::lexical_cast<int>(msgSplited.at(cnt)) * 100;
        }
        else if (cnt == 6)
        {
            msg.windSpeed = boost::lexical_cast<BSL_TYPE>(msgSplited.at(cnt)) * 0.1f;
        }
    }
    msgDecoded->Msg.push_back(msg);
}

decoder1111::decoder1111(const vector<string> &splitedMsg, std::shared_ptr<ammoBase> bulletHandle)
{
    this->msgSplited = splitedMsg;
    this->bullet = bulletHandle;
    decode();
}

void decoder1111::decode()
{
    try
    {
        validationCheck();
    }
    catch (std::exception &e)
    {
        cout << "Invalid weather message: " << e.what() << endl;
        throw e;
    }

    int msgLen = static_cast<int>(msgSplited.size());                       
    
    msgDecoded = std::make_shared<BSL::weatherMsgStruct>(BSL::weatherMsgStruct());
    BSL::singleWeatherMsg msg;

    for (int cnt = 0; cnt < msgLen; ++cnt)
    {
        if (cnt == 0)
        {
            msgDecoded->messageType = BSL::WEATHER_TYPE_1111;
        }
        else if (cnt == 1)
        {
            msgDecoded->time = msgSplited.at(cnt).substr(0, 2) + ":" + msgSplited.at(cnt).substr(2, 2);
        }
        else if (cnt == 2)
        {
            msgDecoded->weather_station_height = boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        else if (cnt == 3)
        {
            
            
            int isMinus_pressure = boost::lexical_cast<int>(msgSplited.at(cnt).substr(0, 1)) - 5 > 0 ? -1 : 1;
            int isMinus_temperature = boost::lexical_cast<int>(msgSplited.at(cnt).substr(3, 1)) - 5 > 0 ? -1 : 1;
            
            msgDecoded->weather_station_pressure =
                static_cast<BSL_TYPE>(760 + isMinus_pressure *
                                      (boost::lexical_cast<int>(msgSplited.at(cnt).substr(0, 3)) +
                                       (isMinus_pressure - 1) * 250)) * 100 / 760;
            
            msgDecoded->weather_station_temperature =
                BSL_VirtualTemp_Ground +
                static_cast<BSL_TYPE>((isMinus_temperature *
                                       (boost::lexical_cast<int>(msgSplited.at(cnt).substr(3, 2)) +
                                        (isMinus_temperature - 1) * 25.0)));
        }
        else if (cnt == msgLen - 1)
        {
            msgDecoded->measureHeight = boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        else
        {
            if (cnt % 2 == 0)
            {
                int height_code = boost::lexical_cast<int>(msgSplited.at(cnt));
                int height = height_code >= 10 ? 1000 * height_code : 100 * height_code;
                msg.height = height;
            }
            else
            {
                int isMinus = boost::lexical_cast<int>(msgSplited.at(cnt).substr(0, 1)) - 5 > 0 ? -1 : 1;
                msg.temperature = BSL_VirtualTemp_Ground + static_cast<BSL_TYPE>((isMinus * (boost::lexical_cast<int>(
                                      msgSplited.at(cnt).substr(0, 2)) + (isMinus - 1) * 25)));
                msg.windAngle = boost::lexical_cast<int>(msgSplited.at(cnt).substr(2, 2)) * 100;
                msg.windSpeed = boost::lexical_cast<BSL_TYPE>(msgSplited.at(cnt).substr(4, 2));
                msgDecoded->Msg.push_back(msg);
                msg.clear();
            }
        }
    }
}

bool decoder1111::validationCheck()
{
    
    if (msgSplited.empty() || msgSplited.size() < 6)
    {
        throw length_error("1111: incomplete weather message!");
    }

    for (int cnt = 0; cnt < msgSplited.size(); ++cnt)
    {
        try
        {
            boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        catch (boost::bad_lexical_cast &e)
        {
            cout << e.what() << endl;
            throw e;
        }

        if ((cnt == 0 || cnt == 1 || cnt == 2) && (msgSplited.at(cnt).size() != 4))
        {
            throw length_error("Invalid length!");
        }
        else if (cnt == 3)
        {
            if (msgSplited.at(cnt).size() != 5)
            {
                throw length_error("Invalid length!");
            }
        }
        else if (cnt > 3 && cnt % 2 == 0 && cnt != msgSplited.size() - 1)    
        {
            if (msgSplited.at(cnt).size() != 2)
            {
                throw length_error("Invalid length!");
            }
        }
        else if (cnt == msgSplited.size() - 1)          
        {
            if (msgSplited.back().size() != 3)
            {
                throw length_error("Invalid length!");
            }
        }
        else if (cnt > 4 && cnt % 2 == 1)           
        {
            if (msgSplited.at(cnt).size() != 6)
            {
                throw length_error("Invalid length!");
            }
        }
    }

    
    if (boost::lexical_cast<int>(msgSplited.at(1).substr(0, 2)) > 24 ||
            boost::lexical_cast<int>(msgSplited.at(1).substr(2, 2)) >= 60)
    {
        throw logic_error("Invalid time message!");
    }

    return true;
}

std::vector<BSL_TYPE> decoder1111::readWindSpeedSheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle)
{
    return weatherMsgBase::readWindSpeedSheet(windSpeed, deltaAngle);
}


decoder5555::decoder5555(const vector<string> &splitedMsg, std::shared_ptr<ammoBase> bulletHandle)
{
    this->msgSplited = splitedMsg;
    this->bullet = bulletHandle;
    decode();
}

void decoder5555::decode()
{
    try
    {
        validationCheck();
    }
    catch (std::exception &e)
    {
        cout << "Invalid weather message: " << e.what() << endl;
        throw e;
    }

    int msgLen = static_cast<int>(msgSplited.size());                       
    
    msgDecoded = std::make_shared<BSL::weatherMsgStruct>(BSL::weatherMsgStruct());
    BSL::singleWeatherMsg msg;

    for (int cnt = 0; cnt < msgLen; ++cnt)
    {
        if (cnt == 0)
        {
            msgDecoded->messageType = BSL::WEATHER_TYPE_5555;
        }
        else if (cnt == 1)
        {
            msgDecoded->time = msgSplited.at(cnt).substr(0, 2) + ":" + msgSplited.at(cnt).substr(2, 2);
        }
        else if (cnt == 2)
        {
            msgDecoded->weather_station_height = boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        else if (cnt == 3)
        {
            msgDecoded->weather_station_pressure = boost::lexical_cast<BSL_TYPE>(msgSplited.at(cnt)) * 0.1f * 100 / 760;
        }
        else if (cnt == msgLen - 1)
        {
            msgDecoded->measureHeight = boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        else
        {
            if ((cnt - 4) % 3 == 0)
            {
                int height_code = boost::lexical_cast<int>(msgSplited.at(cnt));
                int height = height_code >= 10 ? 1000 * height_code : 100 * height_code;
                msg.height = height;
            }
            else if ((cnt - 4) % 3 == 1)
            {
                msg.windAngle = boost::lexical_cast<int>(msgSplited.at(cnt).substr(0, 3)) * 10;
                msg.windSpeed = boost::lexical_cast<BSL_TYPE>(msgSplited.at(cnt).substr(3, 3)) * 0.1f;
            }
            else if ((cnt - 4) % 3 == 2)
            {
                msg.temperature = boost::lexical_cast<BSL_TYPE>(msgSplited.at(cnt)) * 0.1f;

                if (cnt == 6)
                {
                    msgDecoded->weather_station_temperature = msg.temperature;
                }

                msgDecoded->Msg.push_back(msg);
                msg.clear();
            }
        }
    }
}

bool decoder5555::validationCheck()
{
    
    if (msgSplited.empty() || msgSplited.size() < 7)
    {
        throw length_error("5555: incomplete weather message!");
    }

    
    for (int cnt = 0; cnt < msgSplited.size(); ++cnt)
    {
        
        try
        {
            boost::lexical_cast<int>(msgSplited.at(cnt));
        }
        catch (boost::bad_lexical_cast &e)
        {
            cout << e.what() << endl;
            throw e;
        }

        
        if ((cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3) && (msgSplited.at(cnt).size() != 4))
        {
            throw length_error("Invalid length!");
        }
        else if (cnt > 3 && (cnt - 4) % 3 == 0 && cnt != msgSplited.size() - 1)    
        {
            if (msgSplited.at(cnt).size() != 2)
            {
                throw length_error("Invalid length!");
            }
        }
        else if (cnt == msgSplited.size() - 1)          
        {
            if (msgSplited.back().size() != 3)
            {
                throw length_error("Invalid length!");
            }
        }
        else if (cnt > 4 && (cnt - 5) % 3 == 0 && cnt != msgSplited.size() - 1)           
        {
            if (msgSplited.at(cnt).size() != 6)
            {
                throw length_error("Invalid length!");
            }
        }
        else if (cnt > 5 && (cnt - 6) % 3 == 0 && cnt != msgSplited.size() - 1)           
        {
            if (msgSplited.at(cnt).size() != 4)
            {
                throw length_error("Invalid length!");
            }
        }
    }

    
    if (boost::lexical_cast<int>(msgSplited.at(1).substr(0, 2)) > 24 ||
            boost::lexical_cast<int>(msgSplited.at(1).substr(2, 2)) >= 60)
    {
        throw logic_error("Invalid time message!");
    }

    return true;
}

std::vector<BSL_TYPE> decoder5555::readWindSpeedSheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle)
{
    return weatherMsgBase::readWindSpeedSheet(windSpeed, deltaAngle);
}

void decoder5555::setBulletType(std::shared_ptr<ammoBase> b)
{
    this->bullet = b;
}

shared_ptr<BSL::weatherMsgStruct> weatherMsgBase::getDecodedMsg() const
{
    if (msgDecoded != nullptr)
    {
        return this->msgDecoded;
    }
    else
    {
        return nullptr;
    }
}

void weatherMsgBase::setBulletType(std::shared_ptr<ammoBase> b)
{
    bullet = b;
}

std::vector<BSL_TYPE>
weatherMsgBase::readWindSpeedSheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle)
{
    std::vector<BSL_TYPE> ret;

    if (bullet == nullptr)
    {
        throw logic_error("null bullettest pointer, please setBulletType first!");
    }
    else
    {
        ret = bullet->read_Ballistic_WindSpeed_sheet(windSpeed, deltaAngle);
    }

    return ret;
}

weatherMsgDecoderFactory::weatherMsgDecoderFactory(const string &Msg)
{
    if (Msg.empty())
    {
        throw length_error("Invalid weather message!");
    }

    string type = Msg.substr(0, 4);

    if (!boost::contains("0000 1111 2222 3333 4444 5555", type))
    {
        throw range_error("Unknown message type!");
    }

    this->messageType = static_cast<BSL::BSL_Weather_Msg_Type>(boost::lexical_cast<int>(type));
    this->msgInput = Msg;
    this->msgSplited = boost::split(this->msgSplited, this->msgInput, boost::is_any_of("-"),
                                    boost::token_compress_on);
}

weatherMsgDecoderFactory::weatherMsgDecoderFactory(const string &Msg, std::shared_ptr<ammoBase> bulletHandle)
{
    if (Msg.empty())
    {
        throw length_error("Invalid weather message!");
    }

    string type = Msg.substr(0, 4);

    if (!boost::contains("0000 1111 2222 3333 4444 5555", type))
    {
        throw range_error("Unknown message type!");
    }

    this->messageType = static_cast<BSL::BSL_Weather_Msg_Type>(boost::lexical_cast<int>(type));
    this->msgInput = Msg;
    this->msgSplited = boost::split(this->msgSplited, this->msgInput, boost::is_any_of("-"),
                                    boost::token_compress_on);
    this->setBulletType(bulletHandle);
}


std::shared_ptr<weatherMsgBase> weatherMsgDecoderFactory::createDecoder()
{
    switch (messageType)
    {
        case BSL::WEATHER_TYPE_0000:
            return std::make_shared<decoder0000>(decoder0000(msgSplited, bullet));

        case BSL::WEATHER_TYPE_1111:
            return std::make_shared<decoder1111>(decoder1111(msgSplited, bullet));

        case BSL::WEATHER_TYPE_2222:
            throw range_error("2222: this weather type is currently unsupported!");

        case BSL::WEATHER_TYPE_3333:
            throw range_error("3333: this weather type is currently unsupported!");

        case BSL::WEATHER_TYPE_4444:
            throw range_error("4444: this weather type is currently unsupported!");

        case BSL::WEATHER_TYPE_5555:
            return std::make_shared<decoder5555>(decoder5555(msgSplited, bullet));

        default :
            return std::make_shared<decoder1111>(decoder1111(msgSplited, bullet));
    }
}
