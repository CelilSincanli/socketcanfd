#include "canfd.hpp"
#include <cstdio>
#include <sys/prctl.h> 

int main() {
    printf("Starting CAN FD Reader...\n");
    std::string config_path = "../config/canfd_config.yaml";

    prctl(PR_SET_NAME, "CANFD_Reader", 0, 0, 0);

    try {
        CANFD canfd(config_path);
        canfd.runReader();
    } catch (const YAML::Exception& e) {
        printf("YAML Exception occurred: %s\n", e.what());
        return EXIT_FAILURE;
    } catch (const std::exception &e) {
        printf("Standard exception occurred: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
