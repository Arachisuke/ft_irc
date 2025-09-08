#include "../header/Server.hpp"
#include "../header/Client.hpp"

Client::Client()
{
    this->_isRegistered = 0;
    this->_Password_Status = 0;
    this->_Nickname_Status = 0;
    this->_Username_Status = 0;
    this->_isRegistered = 0;
    this->_fd = -1;
}
Client::~Client()
{

    std::cout << "Client disconnected\r\n"
              << std::endl;
}

int Client::Init(int epfd, int hote)
{
    this->_size_of_client = sizeof(this->_client);
    this->_hote = hote;

    this->_fd = accept(this->_hote, reinterpret_cast<sockaddr *>(&this->_client), &_size_of_client);
    if (_fd == -1)
        return (1);
    // fcntl(this->fd, F_SETFL, O_NONBLOCK);
    this->_event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP; // surveille lecture et tout probleme.
    this->_event.data.fd = this->_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, this->_fd, &this->_event);
    return (0);
}



const std::string Client::getPrefiksClient() const
{
    return (":" + _nickname + "!" + _username + "@localhost");
}

const std::string &Client::getNickname() const
{
    return (_nickname);
}
const std::string &Client::getUsername() const
{
    return (_username);
}
const std::string &Client::getMode() const
{
    return (_mode);
}
const std::string &Client::getBuffer() const
{
    return (_buffer);
}

void Client::setNickname(std::string nickName)
{
    _nickname = nickName;
}
void Client::setUsername(std::string userName)
{
    _username = userName;
}
void Client::setMode(std::string mode)
{
    _mode = mode;
}
std::string &Client::setBuffer()
{
    return (_buffer);
}

int Client::getFd() const
{
    return (_fd);
}
const std::vector<Channel *> &Client::getMyChannel() const
{
    return (_myChannels);
}
int Client::getPassword_Status() const
{
    return (_Password_Status);
}
int Client::getNickname_Status() const
{
    return (_Nickname_Status);
}
int Client::getUsername_Status() const
{
    return (_Username_Status);
}
int Client::getisRegistered() const
{
    return (_isRegistered);
}

std::vector<Channel *>& Client::setMyChannel()
{
    return(_myChannels);
}
void Client::setFd(int fd)
{
    _fd = fd;
}
void Client::setPasswordStatus(int passwordStatus)
{
    _Password_Status = passwordStatus;
}
void Client::setNicknameStatus(int nickNameStatus)
{
    _Nickname_Status = nickNameStatus;
}
void Client::setUsernameStatus(int userNameStatus)
{
    _Username_Status = userNameStatus;
}
void Client::setIsRegistered(int isRegistered)
{
    _isRegistered = isRegistered;
}
void Client::removeMyChannel(Channel *channelToRemove) 
{
    this->_myChannels.erase(std::find(_myChannels.begin(), _myChannels.end(), channelToRemove));
}