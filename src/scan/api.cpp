#include <tuple>

#include "PiScan.h"

namespace piscan::app::data
{
    /*
	Retrieve the entire System tree
*/
    BasicReturnTuple getScanList()
    {
        return std::make_tuple(NOT_IMPLEMENTED, nullptr);
    }

    /*
	Retrieve list of Systems and their indices, tags, and types
*/
    BasicReturnTuple getSystemList()
    {
        return std::make_tuple(NOT_IMPLEMENTED, nullptr);
    }

    /*
	Retrieve tree of System and its Entries at index
*/
    BasicReturnTuple getSystemByIndex(size_t sys_index)
    {
        return std::make_tuple(NOT_IMPLEMENTED, nullptr);
    } //TODO

    /*
	Retrieve list of Entries within indexed System and their indices and descriptors
*/
    BasicReturnTuple getEntryList(size_t sys_index)
    {
        return std::make_tuple(NOT_IMPLEMENTED, nullptr);
    }

    /*
	Retrieve Entry at index
*/
    BasicReturnTuple getEntryByIndex(size_t sys_index, size_t entry_index)
    {
        return std::make_tuple(NOT_IMPLEMENTED, nullptr);
    } //TODO

    namespace system
    {
        /* 
		Create a new Radio System
	*/
        BasicReturnTuple create(/*TODO data*/)
        {
            return std::make_tuple(NOT_IMPLEMENTED, nullptr);
        }

        /* 
		Replace the Radio System header at index. Entries will be retained unless the system type is changed.
	*/
        BasicReturnTuple replace(size_t sys_index /*, TODO new*/)
        {
            return std::make_tuple(NOT_IMPLEMENTED, nullptr);
        }

        /*
		Remove the Radio System and its Entries at index. Indices of succeeding Systems will be updated upon success
	*/
        BasicReturnTuple remove(size_t sys_index)
        {
            return std::make_tuple(NOT_IMPLEMENTED, nullptr);
        }

        /*
		Set lockout status of System at index.
		- '0' for unlocked
		- '-1' for persistent lock
		- '>1' lock for duration in seconds
	*/
        BasicReturnTuple setLockout(size_t sys_index, int duration_seconds)
        {
            return std::make_tuple(NOT_IMPLEMENTED, nullptr);
        }

        /*
		Move Radio System from original index to new index. All other indices are updated upon success
	*/
        BasicReturnTuple setIndex(size_t original_sys_index, size_t new_sys_index)
        {
            return std::make_tuple(NOT_IMPLEMENTED, nullptr);
        }

        namespace entry
        {
            /* 
			Create a new Entry within the indexed System
		*/
            BasicReturnTuple create(size_t sys_index /*,TODO data*/)
            {
                return std::make_tuple(NOT_IMPLEMENTED, nullptr);
            }

            /* 
			Replace the Entry at index
		*/
            BasicReturnTuple replace(size_t sys_index, size_t entry_index /*, TODO new*/)
            {
                return std::make_tuple(NOT_IMPLEMENTED, nullptr);
            }

            /* 
			Remove the Entry at index. Succeeding indices within the System are updated upon success
		*/
            BasicReturnTuple remove(size_t sys_index, size_t entry_index)
            {
                return std::make_tuple(NOT_IMPLEMENTED, nullptr);
            }

            /*
			Set lockout status of Entry at index.
			- '0' for unlocked
			- '-1' for persistent lock
			- '>1' lock for duration in seconds
		*/
            BasicReturnTuple setLockout(size_t sys_index, size_t entry_index, int duration_seconds)
            {
                return std::make_tuple(NOT_IMPLEMENTED, nullptr);
            }

            /*
			Move Entry within System from original index to new index. All other indices are updated upon success
		*/
            BasicReturnTuple setIndex(size_t sys_index, size_t original_entry_index, size_t new_entry_index)
            {
                return std::make_tuple(NOT_IMPLEMENTED, nullptr);
            }
        }
    }
}
