#ifndef CANFD_HPP
#define CANFD_HPP

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstdio> // For printf
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/exceptions.h> // For YAML::Exception

class CANFD {
public:
    CANFD(const std::string &config_path);
    ~CANFD();
    void runReader();
    void runWriter();

private:
    int socket_fd;
    struct sockaddr_can addr;
    struct ifreq ifr;
    std::string interface_name;
    std::vector<int> filter_ids;

    void loadConfig(const std::string &config_path);
    void setupSocket();
    void setupFilters();
    void readFromSocket();
    void writeToSocket(const struct canfd_frame &frame);
    void writeToSocketInteractive();
};

#endif // CANFD_HPP
