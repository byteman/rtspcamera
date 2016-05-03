#include "NetEventHandler.h"


namespace CORE {


NetEventHandler::NetEventHandler()
{


}


NetEventHandler:: ~NetEventHandler()
{

}
		

Int32 NetEventHandler::handle_input(Socket &sock)
{
	return 0;
}
	

Int32 NetEventHandler::handle_output(Socket &sock)
{
	return 0;
}
	

Int32 NetEventHandler::handle_exception(Socket &sock)
{
	return 0;
}
	

Int32 NetEventHandler::handle_timeout(Socket &sock)
{
	return 0;
}


Int32 NetEventHandler::handle_close(Socket &sock)
{
	return 0;
}


} 

