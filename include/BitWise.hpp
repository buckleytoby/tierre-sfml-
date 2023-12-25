// bit flags for HandleInputActions, following https://github.com/TackyTortoise/Medium/blob/main/BitWise.cpp
// include guard
#ifndef BITWISE_HPP
#define BITWISE_HPP

#include <iostream>
#include <assert.h>
enum class EFlagValue
{
    Flag1 = 1 << 0, // 1
    Flag2 = 1 << 1, // 2
    Flag3 = 1 << 2, // 4
    Flag4 = 1 << 3, // 8
    Flag5 = 1 << 4, // 16
    Flag6 = 1 << 5, // 32
    Flag7 = 1 << 6, // 64
    Flag8 = 1 << 7  // 128
};
struct BitFlag
{
    //Sets flag to true
    void SetFlag(EFlagValue flag)
    {
        m_FlagValue |= (int)flag;
    }

    //Sets flag to false
    void UnsetFlag(EFlagValue flag)
    {
        m_FlagValue &= ~(int)flag;
    }

    //Sets a flag value from true to false and vice versa
    void FlipFlag(EFlagValue flag)
    {
        m_FlagValue ^= (int)flag;
    }

    //Check whether a flag is set to true
    bool HasFlag(EFlagValue flag)
    {
        return (m_FlagValue & (int)flag) == (int)flag;
    }

    bool HasAnyFlag(EFlagValue multiFlag)
    {
        return (m_FlagValue & (int)multiFlag) != 0;
    }

    void ClearFlag()
    {
        m_FlagValue = 0;
    }

    uint8_t m_FlagValue = 0;
};

#endif // BITWISE_HPP