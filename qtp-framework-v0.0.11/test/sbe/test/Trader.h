/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _TEST_TRADER_H_
#define _TEST_TRADER_H_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#if __cplusplus >= 201103L
#  include <cstdint>
#  include <functional>
#  include <string>
#  include <cstring>
#endif

#include <sbe/sbe.h>


using namespace sbe;

namespace test {

class Trader
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + 8) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    Trader(void) : m_buffer(nullptr), m_offset(0) {}

    Trader(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    Trader(const Trader& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

#if __cplusplus >= 201103L
    Trader(Trader&& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

    Trader& operator=(Trader&& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

#endif

    Trader& operator=(const Trader& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

    Trader &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const std::uint64_t encodedLength(void)
    {
        return 8;
    }


    static const std::uint32_t idNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const std::uint32_t idMinValue()
    {
        return 0;
    }

    static const std::uint32_t idMaxValue()
    {
        return 4294967293;
    }

    std::uint32_t id(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 0)));
    }

    Trader &id(const std::uint32_t value)
    {
        *((std::uint32_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint32_t typeNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const std::uint32_t typeMinValue()
    {
        return 0;
    }

    static const std::uint32_t typeMaxValue()
    {
        return 4294967293;
    }

    std::uint32_t type(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 4)));
    }

    Trader &type(const std::uint32_t value)
    {
        *((std::uint32_t *)(m_buffer + m_offset + 4)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }
};
}
#endif
