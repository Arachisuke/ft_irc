#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>



class Channel {
public:
    Channel();
    ~Channel();
    std::string name;
    std::string topic;
    std::string mode;
    std::vector<std::string> users;

private:
};

#endif