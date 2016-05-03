#ifndef TimeNotifier_H
#define TimeNotifier_H


#include "Core.h"
#include "TimeEventHandler.h"
#include "Timespan.h"
#include "Timestamp.h"
#include <set>


namespace CORE {


/// Class TimeNotifier
/// It is used by TimeReactor
class CORE_API TimeNotifier
{
public:

	TimeNotifier();

	~TimeNotifier();

		/// @brief
		/// Adds the handler 
	void addEventHandler(TimeEventHandler *handler, char *name);

		/// @brief
		/// Removes the handler 
	void removeEventHandler();

		/// @brief
		/// Call back
	Int32 dispatch();

		/// @brief
		/// To see if the interval expires.
	bool isTimeout();

	void setTimeout(Int32 seconds);

	Int32 getId() const;

	void setId(Int32 id);

	TimeEventHandler* getHandler() const;

	void setClear(bool clear);

	bool	isClear();

	char *getName();

private:
	bool					m_isClear;
	TimeEventHandler*	 m_ptrHandler;
	Int32				m_nId;
	Int32				m_timeout;
	Int32				m_tmp;
	char					m_name[100];

	friend class TimeReactor;
};


} 



#endif

