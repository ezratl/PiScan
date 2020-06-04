/* cross platform thread naming, copied from this stackoverflow thread:
    https://stackoverflow.com/questions/10121560/stdthread-naming-your-thread
*/

#pragma once

#include <thread>

#include <loguru.hpp>

#ifdef _WIN32
#include <windows.h>
const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)


inline void _SetThreadName(uint32_t dwThreadID, const char* threadName)
{

  // DWORD dwThreadID = ::GetThreadId( static_cast<HANDLE>( t.native_handle() ) );

   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName;
   info.dwThreadID = dwThreadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
}
inline void _SetThreadName( const char* threadName)
{
    _SetThreadName(GetCurrentThreadId(),threadName);
}

inline void _SetThreadName( std::thread* thread, const char* threadName)
{
    DWORD threadId = ::GetThreadId( static_cast<HANDLE>( thread->native_handle() ) );
    _SetThreadName(threadId,threadName);
}

#else
inline void _SetThreadName(std::thread* thread, const char* threadName)
{
   auto handle = thread->native_handle();
   pthread_setname_np(handle,threadName);
}


#include <sys/prctl.h>
inline void _SetThreadName( const char* threadName)
{
  prctl(PR_SET_NAME,threadName,0,0,0);
}

#endif

/* convenience func to integrate with loguru */
inline void setThreadName(const char* name){
    loguru::set_thread_name(name);
    _SetThreadName(name);
}
