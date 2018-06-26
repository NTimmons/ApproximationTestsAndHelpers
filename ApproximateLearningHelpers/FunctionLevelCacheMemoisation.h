#pragma once

#include "FunctionLevelCache.h"


//Memoise Function
#define CACHE_SIZE 10
template<typename Fn, Fn func, int percentageErrorAllowed, typename... Args>
typename std::result_of<Fn(Args...)>::type MemoiseWrapperFunction(Args&&... args)
{
    ////Approximate Information
    static std::array< FunctionInformation<std::result_of<Fn(Args...)>::type, Args...>, CACHE_SIZE > globalFunctionResultCache;
    static      int             currentIndex        = 0;
    constexpr	double          percentageError     = double(percentageErrorAllowed) / (100.f);
    constexpr	unsigned int    inputVariableCount  = sizeof...(Args);
    constexpr	auto            argOffsets          = GetTypesOffsets<Args...>();

    //Check if there is a stored close enough value
    for (int ii = 0; ii < CACHE_SIZE; ii++)
    {
        const char*	voidFi = (const char*)&globalFunctionResultCache[ii];
        bool		inRange = true;
        int			variableIndex = 0;

        const bool results[] =
        { 
            (CheckValid(  args, 
                        MemberVariablePointer   <   decltype(argOffsets),
                                                    std::result_of<Fn(Args...)>::type,
                                                    std::remove_reference<decltype(args)>::type
                                                >(voidFi, variableIndex++, argOffsets) ,
                        percentageError
                      )
            )... 
        };

        for (const bool res : results)
        {
            if (!res)
            {
                inRange = false; break;
            }
        }

        if (inRange)
        {
            //We have a match in the cache.
            return globalFunctionResultCache[ii]._returnValue;
        }
    }

    //No match found.
    //Do work and store cache result
    const int writeindex = currentIndex;
    globalFunctionResultCache[writeindex]._returnValue = func(std::forward<Args>(args)...);
    globalFunctionResultCache[writeindex].Set(std::forward<Args>(args)...);

    //Setup next cache index
    currentIndex++;
    currentIndex = currentIndex % CACHE_SIZE;

    //Return results
    return globalFunctionResultCache[writeindex]._returnValue;
}
#define MemoWrapper(FUNC, ERRORPERC) MemoiseWrapperFunction<decltype(&FUNC), &FUNC, ERRORPERC>