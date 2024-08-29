#include "canfd.hpp"
#include <cstdio>  // For printf and perror
#include <cstring> // For strlen
#include <cassert>

CANFD::CANFD(const std::string &config_path) {
    printf("Initializing CANFD with config file: %s\n", config_path.c_str());
    loadConfig(config_path);
    setupSocket();
    setupFilters();
}

CANFD::~CANFD() {
    close(socket_fd);
    printf("Socket closed.\n");
}

void CANFD::loadConfig(const std::string &config_path) {
    try {
        YAML::Node config = YAML::LoadFile(config_path);
        interface_name = config["default_can_interface_name"].as<std::string>();
        filter_ids = config["default_can_filter_ids"].as<std::vector<int>>();
        printf("Configuration loaded successfully.\n");
        printf("Interface name: %s\n", interface_name.c_str());
        printf("Filter IDs: ");
        for (int id : filter_ids) {
            printf("%d ", id);
        }
        printf("\n");
    } catch (const YAML::BadFile& e) {
        printf("Error: Unable to open file: %s\n", config_path.c_str());
        printf("Exception: %s\n", e.what());
        throw; // Rethrow the exception after logging
    } catch (const YAML::ParserException& e) {
        printf("Error: YAML parsing error\n");
        printf("Exception: %s\n", e.what());
        throw; // Rethrow the exception after logging
    } catch (const YAML::RepresentationException& e) {
        printf("Error: YAML representation error\n");
        printf("Exception: %s\n", e.what());
        throw; // Rethrow the exception after logging
    } catch (const std::exception& e) {
        printf("Error: Standard exception occurred\n");
        printf("Exception: %s\n", e.what());
        throw; // Rethrow the exception after logging
    }
}

void CANFD::setupSocket() {
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int canfd_enabled = 1;
    if (setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_enabled, sizeof(canfd_enabled)) < 0) {
        perror("Setting CAN FD option failed");
        exit(EXIT_FAILURE);
    }

    strcpy(ifr.ifr_name, interface_name.c_str());
    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl failed");
        exit(EXIT_FAILURE);
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket setup complete.\n");
}

void CANFD::setupFilters() {
    if (!filter_ids.empty()) {
        std::vector<struct can_filter> rfilter(filter_ids.size());
        for (size_t i = 0; i < filter_ids.size(); ++i) {
            rfilter[i].can_id = filter_ids[i];
            rfilter[i].can_mask = CAN_SFF_MASK;
        }
        if (setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FILTER, rfilter.data(), rfilter.size() * sizeof(struct can_filter)) < 0) {
            perror("Setting CAN filter failed");
            exit(EXIT_FAILURE);
        }
        printf("CAN filters set up.\n");
    }
}

void CANFD::readFromSocket() {
    struct canfd_frame frame;
    int nbytes = read(socket_fd, &frame, sizeof(frame));

    if (nbytes < 0) {
        perror("CAN FD Raw socket read failed");
        return;
    }

    if (nbytes < sizeof(frame)) {
        perror("Read: Incomplete CAN FD frame");
        return;
    }

    printf("CAN ID: %u\n", frame.can_id);
    printf("Data length: %d\n", static_cast<int>(frame.len));
    printf("Received frame data: ");
    for (int i = 0; i < frame.len; ++i) {
        printf("%02X ", frame.data[i]);
    }
    printf("\n");
}

void CANFD::writeToSocket(const struct canfd_frame &frame) {
    int nbytes = write(socket_fd, &frame, sizeof(frame));

    if (nbytes < 0) {
        perror("CAN FD Raw socket write failed");
        return;
    }

    if (nbytes < sizeof(frame)) {
        perror("Write: Incomplete CAN FD frame");
    }
}

void CANFD::writeToSocketInteractive() {
    struct canfd_frame frame;
    int data_length;
    int flags;

    printf("Please enter a CAN ID (number from 0 to 999): ");
    scanf("%u", &frame.can_id);

    do {
        printf("Please enter CAN flags (number from 0-15): ");
        scanf("%d", &flags);
    } while (flags < 0 || flags > 15);

    frame.flags = flags;
    getchar(); // To consume the newline left by scanf

    printf("Please enter CAN data: ");
    fgets(reinterpret_cast<char *>(frame.data), sizeof(frame.data), stdin);

    data_length = strlen(reinterpret_cast<char *>(frame.data));
    frame.len = data_length;

    writeToSocket(frame);
}

void CANFD::runReader() {
    while (true) {
        readFromSocket();
    }
}

void CANFD::runWriter() {
    while (true) {
        writeToSocketInteractive();
    }
}
