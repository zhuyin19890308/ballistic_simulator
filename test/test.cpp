
#include <iostream>
#include <memory>
#include <exception>
#include "include/ballisticTypeDef.h"
#include "include/ballistic.h"

int main() {
    int iter_num = 100;
    auto bullet_name = BSL::NAME_AK47;
    try {
        for (int i = 0; i < iter_num; i++) {
            std::unique_ptr<ballistic> bullet;
            try {
                //step 1——————Init
                bullet = std::make_unique<ballistic>(bullet_name, 10, BSL::BSL_Initialize_Para());
            } catch (const std::exception &e) {
                std::cerr << "Exception: " << e.what() << std::endl;
                return -1;
            }
            //step 2——————calculate the ballistic trajectory
            int len = static_cast<int>(bullet->calculate());
            //step 3——————Initialize variables to store ballistic data
            BSL::BSL_Result result(len);
            //step 4——————Copy result from bullet handler
            bullet->getResult(result);
            //step 5——————Print data: for example, print the shooting distance
            std::cout << "Last Position X: " << result.pData.get()[len - 1].Pos_x << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return -1;
    }
    return 0;
}
