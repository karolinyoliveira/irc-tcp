#include "../lib/utils.hpp"
#include "../lib/channel_controller.hpp"
#include <vector>

#define MAX_CLIENTS 100

int main() {   
    signal(SIGPIPE, SIG_IGN);
    ChannelController controller(MAX_CLIENTS);
    controller.start();
    return 0;
}