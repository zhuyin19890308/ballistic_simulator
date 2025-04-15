#ifndef BALLISTICLIBRARY_AMMODATA_H
#define BALLISTICLIBRARY_AMMODATA_H

#include "ballisticTypeDef.h"
#include "../../Ammolib/source/NAME_UNKNOWN_AMMO.h"
#include "../../Ammolib/source/NAME_AK47.h"



using namespace BSL;

class ammoFactory : public ammoBase {
public:

    std::shared_ptr<ammoBase> createAmmo2(BSL::BSL_Bullet_Name Name){
        switch (Name) {
                case NAME_AK47:
                    return std::make_shared<NAMESPACE_AK47::NAME_AK47>();

                default:

                    throw std::logic_error("Unknown ammo name");
        }
        return nullptr;
    }

    ammoBase *createAmmo(BSL::BSL_Bullet_Name Name) {
        switch (Name) {

            case NAME_AK47:
                 return  new NAMESPACE_AK47::NAME_AK47;

            default:
                
                throw std::logic_error("Unknown ammo name");
        }
        return nullptr;
    }
};

#endif 
