
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "signal.h"
#include "errno.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"

#include "arpa/inet.h"

#define internal static
#define global static
#define local static

#define Assert(expr) if(!(expr)) {*(int *)0 = 1;}

#define Kilobytes(b) b * 1024
#define Megabytes(b) Kilobytes(b) * 1024
#define Gigabytes(b) Megabytes(b) * 1024

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef u32 b32;

typedef size_t memory_index;

typedef struct
{
	memory_index size;
	memory_index used;
	u8 *base;
} memory_arena;

internal void
InitializeArena(memory_arena *arena, memory_index size)
{
	//*arena = {0};
	arena->size = size;
	arena->base = (u8 *)malloc(size);
}

#define PushStruct(arena, type) (type *)PushStruct_(arena, sizeof(type))
#define PushArray(arena, type, count) (type *)PushStruct_(arena, sizeof(type) * count)
internal inline void *
PushStruct_(memory_arena *arena, size_t size)
{
	void *result = arena->base + arena->used;
	Assert(arena->used + size <= arena->size);
	arena->used += size;
	return result;
}

typedef struct
{
	int len;
	char *data;

	void *next;
} line;

typedef struct
{
	int lineCount;
	line linesHead;
} line_list;

global memory_arena GlobalArena = {0};
global int GlobalTCPSocket;

internal int
StringLength(char *str)
{
	int result = 0;
	while(*str++) result++;
	return result;
}

internal int 
IndexOf(char *src, char *target)
{
	int targetLen = StringLength(target);
	int srcLen = StringLength(src);

	for(int srcIndex = 0;
			srcIndex < srcLen;
			++srcIndex)
	{
		int compareResult = strncmp(src + srcIndex, target, targetLen);
		if(compareResult == 0)
		{
			return srcIndex;
		}
	}

	return -1;
}

#if 0
internal void 
sigHandler(int signalNumber)
{

}

internal void 
sigAction(int, siginfo_t *, void *)
{
}
#endif

int main(int argc, char *argv[])
{
	u16 result = 0;

#if 0
	struct sigaction s;
	s.sa_handler = sigHandler;
	//s.sa_sigaction;
	s.sa_mask;
	s.sa_flags;
	//s.sa_restorer;
	sigaction(SIGINT, &s, 0);
#endif
	InitializeArena(&GlobalArena, Megabytes(512));
	memset(GlobalArena.base, 0, GlobalArena.size);

	GlobalTCPSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(GlobalTCPSocket != -1)
	{
		struct sockaddr_in sockAddress = {0};
		sockAddress.sin_family = AF_INET;
		sockAddress.sin_port = htons(8000);
		sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		//sockAddress.sin_addr.s_addr = inet_aton("192.168.242.130");
		//inet_aton("192.168.242.130", &sockAddress.sin_addr);
		//6 == TCP
		b32 useAddr = 1;
		setsockopt(GlobalTCPSocket, SOL_SOCKET, SO_REUSEADDR, &useAddr, sizeof(useAddr));

		int bindResult = bind(GlobalTCPSocket, (struct sockaddr *)&sockAddress, sizeof(sockAddress));
		if(bindResult != -1)
		{
			int listenResult = listen(GlobalTCPSocket, 10);
			if(!listenResult)
			{
				struct sockaddr_in peer;
				socklen_t peerLen = sizeof(peer);
				int acceptSocket = accept(GlobalTCPSocket, (struct sockaddr *)&peer, &peerLen);
				if(acceptSocket != -1)
				{
					char buffer[4096] = {0};
					int bytesRead = read(acceptSocket, buffer, sizeof(buffer));
					if(bytesRead > 0)
					{
						printf("%s\n", buffer);
						line_list list = {0};
						int byteIndex = 0;
						line *head = &list.linesHead;
						while(byteIndex < bytesRead)
						{
							int eolIndex = IndexOf(buffer + byteIndex, "\r\n");
							if(eolIndex > 0)
							{
								list.lineCount++;
								head->len = eolIndex;
								head->data = PushArray(&GlobalArena, char, head->len + 1);
								memcpy(head->data, buffer + byteIndex, head->len);
								
								byteIndex += eolIndex + 2;
								head->next = (void *)PushStruct(&GlobalArena, line);
								head = (line *)head->next;
							}
							else
							{
								//todo(jarrett): do something
								break;
							}
						}

						char responseBuffer[4096] = {0};
						sprintf(responseBuffer, "HTTP/1.1 200 OK\r\n");
					}
					else
					{

					}
				}
				else
				{
					printf("%s\n", strerror(errno));
					//todo(jarrett): logging
				}

			}
			else
			{
				printf("%s\n", strerror(errno));
				//todo(jarrett): logging
			}
		}
		else
		{
			printf("%s\n", strerror(errno));
		}
	}
	else
	{
		printf("%s\n", strerror(errno));
		//todo(jarrett): logging
	}
	
	close(GlobalTCPSocket);

	return result;
}
