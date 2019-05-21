#include <echo/EchoReader.hpp>

echo::EchoReader *echo::EchoReader::instance = nullptr;

echo::EchoReader *echo::EchoReader::getInstance()
{
    if (instance == nullptr)
    {
        instance = new EchoReader();
    }
    return instance;
}
int echo::EchoReader::read(xs_SOCKET sock, char *buffer, int bufferLen, int i)
{
    if (i > deadLock)
        return -1;
    int readBytes = xs_recv(1, buffer, bufferLen, 0);
    if (readBytes < bufferLen)
    {
        /*d - delta*/
        int d = bufferLen - readBytes;
        read(sock, buffer + readBytes, d, i++);
    }
    return 0;
}

echo::Chat *echo::EchoReader::read(xs_SOCKET sock)
{
    if (sock == SOCKET_ERROR)
        return nullptr;
    void *c = (char *)new Chat();
    int readBytes = read(sock, (char *)c, sizeof(Chat), 0);
    if (readBytes <= 0)
    {
        return nullptr;
    }
    Chat *chat = (Chat *)c;
    return chat;
}
