#include "unistd.h"
#include "stdint.h"

#include "sys/socket.h"

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

	int tcpSocket = socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK);

	if(tcpSocket != -1)
	{

	}
	else
	{
		//todo(jarrett): logging
	}
	
	close(tcpSocket);

	return result;
}
