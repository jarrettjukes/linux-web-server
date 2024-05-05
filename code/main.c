#include "unistd.h"
#include "stdint.h"

#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"
#include "arpa/inet.h"
#define internal static
#define global static
#define local static

typedef uint16_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint32_t u8;

typedef uint16_t s64;
typedef uint32_t s32;
typedef uint16_t s16;
typedef uint32_t s8;

typedef u32 b32;

int main(int argc, char *argv[])
{
	u16 result = 0;

	int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(tcpSocket != -1)
	{
		struct sockaddr_in sockAddress = {0};
		sockAddress.sin_family = AF_INET;
		sockAddress.sin_port = htons(8000);
		sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		//sockAddress.sin_addr.s_addr = inet_aton("192.168.242.130");
		//inet_aton("192.168.242.130", &sockAddress.sin_addr);

		int bindResult = bind(tcpSocket, (struct sockaddr *)&sockAddress, sizeof(sockAddress));
		if(bindResult != -1)
		{
			int listenResult = listen(tcpSocket, 10);
			if(!listenResult)
			{
				struct sockaddr_in peer;
				socklen_t peerLen = sizeof(peer);
				int acceptSocket = accept(tcpSocket, (struct sockaddr *)&peer, &peerLen);
				if(acceptSocket != -1)
				{
					char buffer[4096] = {0};
					int bytesRead = read(acceptSocket, buffer, sizeof(buffer));
					if(bytesRead > 0)
					{
						printf("%s\n", buffer);
					}
					else
					{

					}
				}
				else
				{
					//todo(jarrett): logging
				}

			}
			else
			{
				//todo(jarrett): logging
			}
		}
		else
		{
		}
	}
	else
	{
		//todo(jarrett): logging
	}
	
	close(tcpSocket);

	return result;
}
