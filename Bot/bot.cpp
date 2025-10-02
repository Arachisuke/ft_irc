/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:02:53 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/01 15:26:18 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <climits>
#include <csignal>
#include <map>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

#define LECTURE_SIZE 512

bool stop = false;

void handler(int)
{
    stop = true;
}

int range_port(char *port)
{
    const char *s = port;
    char *end;
    long val;

    errno = 0;
    val = strtol(s, &end, 10);
    if (errno == ERANGE || val > 65535 || val <= 0)
        return (std::cerr << "Port number out of range." << std::endl, -1);
    if (*end != '\0')
        return (std::cerr << "Invalid port number." << std::endl, -1);
    return (static_cast<int>(val));
}
void clearsinzero(struct sockaddr_in *addr)

{
    for (int i = 0; i < 8; ++i)
        addr->sin_zero[i] = 0;
}

int init(int port)
{
    int fd;
    sockaddr_in hote;
    hote.sin_family = AF_INET;
    hote.sin_port = htons(port);
    hote.sin_addr.s_addr = inet_addr("127.0.0.1");
    clearsinzero(&hote);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(fd, reinterpret_cast<struct sockaddr *>(&hote), sizeof(hote)) == -1)
    {
        close(fd);
        throw(std::runtime_error("ERR_CONNECT"));
    }
    return (fd);
}

int sendToServer(int fd, std::string msg)
{
    msg += "\r\n";
    if (send(fd, msg.c_str(), msg.size(), 0) == -1)
        return (close(fd), stop = 1);

    if (errno == EPIPE)
        return (close(fd), stop = 1);
    return (0);
}

int extractLine(int bytes, int fd, char *lecture, std::string &entry, std::string &buffer)
{
    size_t pos;
    if (bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (1);
        std::cerr << "ERR_READ" << std::endl;
        return (close(fd), 1);
    }
    else if (bytes > 0)
    {
        buffer.append(lecture, bytes);
        pos = buffer.find("\r\n");
        while (pos != std::string::npos)
        {
            entry = buffer.substr(0, pos);
            buffer.erase(0, pos + 2);
            pos = buffer.find("\r\n");
        }
        return (0);
    }
    return (0);
}

std::string callGrok(int fd, std::string entry, std::string token)
{
    if (stop)
        throw(std::runtime_error("ERROR"));
    std::string norme = "Réponds sur 3-4 lignes maximum uniquement en texte brut, sans aucune mise en page, sans retour à la ligne, sans liste, sans balises, sans markdown, sans gras, sans italique";
    std::vector<std::string> args;
    args.push_back("-s");
    args.push_back("-X");
    args.push_back("POST");
    args.push_back("https://models.github.ai/inference/chat/completions");
    args.push_back("-H");
    args.push_back("Authorization: Bearer " + token);
    args.push_back("-H");
    args.push_back("Content-Type: application/json");
    args.push_back("-d");
    args.push_back("{\"model\":\"gpt-4.1\",\"messages\":[{\"role\":\"user\",\"content\":\"" + norme + entry + "\"}]}");
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1)
        throw(std::runtime_error("ERROR PIPE"));
    pid = fork();
    if (pid == -1)
        throw(std::runtime_error("ERROR FORK"));

    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        close(fd);

        std::vector<char *> argv;
        argv.push_back(const_cast<char *>("curl"));
        for (size_t i = 0; i < args.size(); ++i)
            argv.push_back(const_cast<char *>(args[i].c_str()));
        argv.push_back(NULL);

        execvp("curl", argv.data());
        _exit(1);
    }
    else
    {
        close(pipe_fd[1]);
        std::string result;
        char buffer[4096];
        ssize_t n;
        while ((n = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[n] = '\0';
            result += buffer;
        }
        close(pipe_fd[0]);
        waitpid(pid, NULL, 0);
        return result;
    }
}

void sendMsg(std::string entry, int fd, std::string token)
{
    if (stop)
        throw(std::runtime_error("ERROR"));

    std::string nickname;
    size_t pos = entry.find("!");
    nickname = entry.substr(1, pos - 1);
    entry = entry.substr(1);
    pos = entry.find(":");
    std::string prvmsg = "PRIVMSG ";

    std::string msg = entry.substr(pos + 2);
    msg = callGrok(fd, msg, token);

    pos = msg.find("annotations");

    msg = msg.substr(pos + 27);
    pos = msg.find("refusal");
    msg = msg.substr(0, pos - 3);
    msg = prvmsg + nickname + " :" + msg;

    sendToServer(fd, msg);
}

std::string getToken(char **env)
{
    int i = 0;
    std::string token;
    while (env[i])
    {
        if (strncmp(env[i], "TOKEN=", 6) == 0)
        {
            token = env[i];
            token = token.substr(6);
            return (token);
        }
        i++;
    }
    return (std::cout << "NOTOKEN BRO" << std::endl, "ERROR");
}

int main(int ac, char **av, char **env)

{
    if (ac != 3)
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);

    signal(SIGINT, &handler);
    signal(SIGQUIT, &handler);
    signal(SIGPIPE, SIG_IGN);
    srand(time(NULL));

    std::string ip = av[1];
    int port = range_port(av[1]);
    int fd = -1;

    try
    {
        fd = init(port);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    std::string msg = "PASS " + static_cast<std::string>(av[2]);

    sendToServer(fd, msg);
    sendToServer(fd, "NICK WALL-E");
    sendToServer(fd, "USER WALL-E WALL-E 0 :IM WALL-E THE BOT");

    int bytes;

    char lecture[LECTURE_SIZE];
    std::string entry;
    std::string buffer;
    std::string token = getToken(env);
    if (token == "ERROR")
        return (close(fd), 1);
    while (!stop)
    {

        bytes = recv(fd, &lecture, LECTURE_SIZE, MSG_DONTWAIT);
        if (extractLine(bytes, fd, lecture, entry, buffer) == 0)
        {

            if (entry.find("PRIVMSG WALL-E") != std::string::npos)
            {
                try
                {
                    sendMsg(entry, fd, token);
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                    break;
                }
            }
        }
    }
    close(fd);
    return (0);
}