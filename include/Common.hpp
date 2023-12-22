#ifndef SIMPLEAMQP_INCLUDE_COMMON_HPP
#define SIMPLEAMQP_INCLUDE_COMMON_HPP

#if __GNUC__ >= 4
    #define DLLExport __attribute__ ((visibility ("default")))
#else
    #define DLLExport
#endif

#endif // SIMPLEAMQP_INCLUDE_COMMON_HPP