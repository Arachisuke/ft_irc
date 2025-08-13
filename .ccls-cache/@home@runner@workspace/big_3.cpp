#include "../header/client.hpp"

void Big_3(std::vector<Client*> client_list, int nbrclient, std::string ERROR_MSG)
{
    client_list[nbrclient]->PushMsg(ERROR_MSG);
    std::cout << "Client disconnected\r\n" << std::endl;
    close(client_list[nbrclient]->fd);
    delete client_list[nbrclient];
    client_list.erase(client_list.begin() + nbrclient);
}