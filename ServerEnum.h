#ifndef SERVER_ENUM
#define SERVER_ENUM

typedef enum MessageType : char
{
	Chat = 1,
	Join,
	Exit,
	Move,
}MessageType;

#endif // !SERVER_ENUM
