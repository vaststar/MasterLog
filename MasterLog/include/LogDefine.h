#ifndef __LogDefine_h__
#define __LogDefine_h__

namespace MasterLog{
    enum class LogLevel{
        LOG_DEBUG = 1 << 0,
        LOG_INFO = 1 << 1,
        LOG_WARN = 1 << 2,
        LOG_ERROR = 1 << 3,
        LOG_FATAL = 1 << 4
    };
    enum class LogAppenderType{
        CONSOLE = 1 << 0,
        FILE = 1 << 1
    };

    inline int operator|(LogLevel lhs,LogLevel rhs)
    {
        return static_cast<int>(lhs) | static_cast<int>(rhs);
    }
    inline int operator|(int lhs,LogLevel rhs)
    {
        return lhs | static_cast<int>(rhs);
    }
    inline int operator|(LogLevel lhs,int rhs)
    {
        return static_cast<int>(lhs) | static_cast<int>(rhs);
    }
    inline bool operator&(LogLevel lhs,LogLevel rhs)
    {
        return 0 != (static_cast<int>(lhs) & static_cast<int>(rhs));
    }  
    inline bool operator&(int lhs,LogLevel rhs)
    {
        return 0 != (lhs & static_cast<int>(rhs));
    } 
    inline bool operator&(LogLevel lhs,int rhs)
    {
        return 0 != (static_cast<int>(lhs) & rhs);
    } 

    inline int operator|(LogAppenderType lhs,LogAppenderType rhs)
    {
        return static_cast<int>(lhs) | static_cast<int>(rhs);
    }
    inline int operator|(int lhs,LogAppenderType rhs)
    {
        return lhs | static_cast<int>(rhs);
    }
    inline int operator|(LogAppenderType lhs,int rhs)
    {
        return static_cast<int>(lhs) | static_cast<int>(rhs);
    }
    inline bool operator&(LogAppenderType lhs,LogAppenderType rhs)
    {
        return 0 != (static_cast<int>(lhs) & static_cast<int>(rhs));
    }  
    inline bool operator&(int lhs,LogAppenderType rhs)
    {
        return 0 != (lhs & static_cast<int>(rhs));
    }  
    inline bool operator&(LogAppenderType lhs,int rhs)
    {
        return 0 != (static_cast<int>(lhs) & rhs);
    }   
}
#endif // __LogDefine_h__