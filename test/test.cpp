
#include <iostream>
#include <chrono>
#include <memory>
#include <exception>
#include "include/ballisticTypeDef.h"
#include "include/ballistic.h"

int main() {
    try {
        for (int i = 0; i < 100; i++) {
            auto start = std::chrono::system_clock::now();

            std::unique_ptr<ballistic> bullet;
            try {
                bullet = std::make_unique<ballistic>(BSL::NAME_AK47, 10, BSL::BSL_Initialize_Para());
            } catch (const std::exception &e) {
                std::cerr << "Exception: " << e.what() << std::endl;
                return -1;
            }

            int len = bullet->calculate();
            std::cout << "len: " << len << std::endl;

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::system_clock::now() - start);
            std::cout << "Cost " << duration.count() / 1e6 << "s" << std::endl;

            BSL::BSL_Result result(len);
            bullet->getResult(result);
            std::cout << "Last Position X: " << result.pData.get()[len - 1].Pos_x << std::endl;
        }
        std::cout << "end" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return -1;
    }
    return 0;
}

//#include <iostream>
//#include <chrono>
//#include <memory>
//#include <exception>
//#include "include/ballisticTypeDef.h"
//#include "include/ballistic.h"
//
//using namespace std;
//
//int main() {
//    try {
//        for (int i = 0; i < 100; i++) {
//
//            std::unique_ptr<ballistic> bullet;
//            BSL::BSL_Initialize_Para para;
//
//
//            auto start = std::chrono::system_clock::now();
//
//            try {
//                bullet = std::make_unique<ballistic>(BSL::NAME_AK47, 10, para);
//            } catch (const std::exception &e) {
//                std::cerr << "Exception: " << e.what() << std::endl;
//                return -1;
//            }
//
//
//            int len = bullet->calculate();
//            std::cout << "len: " << len << std::endl;
//
//
//            auto end = std::chrono::system_clock::now();
//            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//            std::cout << "Cost "
//                      << double(duration.count()) *
//                         std::chrono::microseconds::period::num /
//                         std::chrono::microseconds::period::den
//                      << "s" << std::endl;
//
//
//            BSL::BSL_Result result(len);
//            bullet->getResult(result);
//            std::cout << "Last Position X: " << result.pData.get()[len - 1].Pos_x << std::endl;
//        }
//
//        std::cout << "end" << std::endl;
//    } catch (const std::exception &e) {
//        std::cerr << "Unhandled exception: " << e.what() << std::endl;
//        return -1;
//    } catch (...) {
//        std::cerr << "Unknown error occurred." << std::endl;
//        return -1;
//    }
//
//    return 0;
//}