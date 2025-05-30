#ifndef BALLISTICLIBRARY_BALLISTICWEATHERMSG_H
#define BALLISTICLIBRARY_BALLISTICWEATHERMSG_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "ballisticTypeDef.h"
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "ammoData.h"
#include "ammoBase.h"

using namespace std;

class weatherMsgBase
{
public:
    
    virtual bool validationCheck() = 0;
    virtual void decode() = 0;
    virtual std::vector<BSL_TYPE>
    readWindSpeedSheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle);
    shared_ptr<BSL::weatherMsgStruct> getDecodedMsg() const;
    virtual void setBulletType(std::shared_ptr<ammoBase> b);

public:
    std::shared_ptr<ammoBase> bullet = nullptr;
    string msgInput;
    vector<string> msgSplited;
    std::shared_ptr<BSL::weatherMsgStruct> msgDecoded = nullptr;
};


class decoder0000 : public weatherMsgBase
{
public:
    explicit decoder0000(const std::vector<string> &splitedMsg, std::shared_ptr<ammoBase> bulletHandle);
    void decode() final;

private:
    bool validationCheck() override;
};

class decoder1111 : public weatherMsgBase
{
public:
    explicit decoder1111(const std::vector<string> &splitedMsg, std::shared_ptr<ammoBase> bulletHandle);
    void decode() final;

private:
    bool validationCheck() override;
    std::vector<BSL_TYPE> readWindSpeedSheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle) final;
};

class decoder5555 : public weatherMsgBase
{
public:
    explicit decoder5555(const std::vector<string> &splitedMsg, std::shared_ptr<ammoBase> bulletHandle);
    void decode() final;
    void setBulletType(std::shared_ptr<ammoBase> b) override;

private:
    bool validationCheck() override;
    std::vector<BSL_TYPE> readWindSpeedSheet(const BSL_TYPE &windSpeed, const BSL_TYPE &deltaAngle) final;
};

class weatherMsgDecoderFactory : public weatherMsgBase
{
public:
    explicit weatherMsgDecoderFactory(const std::string &Msg);
    explicit weatherMsgDecoderFactory(const std::string &Msg, std::shared_ptr<ammoBase> bulletHandle);
    std::shared_ptr<weatherMsgBase> createDecoder();
    bool validationCheck() final
    {
        return false;
    };
    void decode() final {};

private:
    BSL::BSL_Weather_Msg_Type messageType;
};


#endif 
