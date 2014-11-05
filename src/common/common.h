#pragma once

#if defined(__APPLE__)
    #include "TargetConditionals.h"
    #if ANGEL_MOBILE
        #include <sys/time.h>
        #import <Availability.h>
        #ifndef __IPHONE_5_0
            #warning "This project uses features only available in iPhone SDK 5.0 and later."
        #endif
        #ifdef __OBJC__
            #import <Foundation/Foundation.h>
            #import <UIKit/UIKit.h>
        #else
            #include <CoreFoundation/CoreFoundation.h>
        #endif
    #else
        #ifndef __OBJC__
            #include <CoreFoundation/CoreFoundation.h>
        #endif
    #endif
#endif
#if defined(__APPLE__)
    #include <unordered_map>
#elif defined(__linux__)
    #include <ext/hash_map>

    //GCC is picky about what types are allowed to be used as indices to hashes.
    // Defining this here lets us use std::strings to index, which is useful.
    #define hashmap_ns __gnu_cxx
    namespace hashmap_ns
    {
        template<> struct hash< std::string >
        {
            size_t operator()( const std::string& x ) const
            {
                return hash< const char* >()( x.c_str() );
            }
        };
    }
#endif

#include "AppConfig.h"
#include "../juce_core/juce_core.h"
