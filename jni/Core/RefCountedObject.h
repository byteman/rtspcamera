#ifndef RefCountedObject_H
#define RefCountedObject_H


#include "Core.h"
#include "Core_Mutex.h"


namespace CORE {

/// Class RefCountedObject
/// Reference-counted objects.
class CORE_API RefCountedObject
{
public:
	RefCountedObject();

		/// Increases the object's reference count.
	void duplicate() const;

		/// decreases the object's reference count.
	void release() const;

		/// returns the object's reference count.
	Int32 referenceCount() const;

protected:
	virtual ~RefCountedObject();

private:
	RefCountedObject(const RefCountedObject&);
	RefCountedObject& operator = (const RefCountedObject&);

	mutable Int32 _rc;
	mutable Core_FastMutex _rcCore_Mutex;
};


//
// inlines
//
inline Int32 RefCountedObject::referenceCount() const
{
	return _rc;
}


}


#endif 

