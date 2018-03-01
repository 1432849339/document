/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _TEST_LOGINQRY_H_
#define _TEST_LOGINQRY_H_

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

#include "Trader.h"
#include "VarDataEncoding.h"

using namespace sbe;

namespace test {

class LoginQry
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t *m_positionPtr;
    std::uint64_t m_offset;
    std::uint64_t m_position;
    std::uint64_t m_actingBlockLength;
    std::uint64_t m_actingVersion;

    inline void reset(
        char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength,
        const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        m_buffer = buffer;
        m_offset = offset;
        m_bufferLength = bufferLength;
        m_actingBlockLength = actingBlockLength;
        m_actingVersion = actingVersion;
        m_positionPtr = &m_position;
        position(offset + m_actingBlockLength);
    }

public:

    LoginQry(void) : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    LoginQry(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    LoginQry(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    LoginQry(const LoginQry& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

#if __cplusplus >= 201103L
    LoginQry(LoginQry&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

    LoginQry& operator=(LoginQry&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

#endif

    LoginQry& operator=(const LoginQry& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

    static const std::uint16_t sbeBlockLength(void)
    {
        return (std::uint16_t)20;
    }

    static const std::uint16_t sbeTemplateId(void)
    {
        return (std::uint16_t)10001;
    }

    static const std::uint16_t sbeSchemaId(void)
    {
        return (std::uint16_t)1;
    }

    static const std::uint16_t sbeSchemaVersion(void)
    {
        return (std::uint16_t)0;
    }

    static const char *sbeSemanticType(void)
    {
        return "";
    }

    std::uint64_t offset(void) const
    {
        return m_offset;
    }

    LoginQry &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    LoginQry &wrapForDecode(
         char *buffer, const std::uint64_t offset, const std::uint64_t actingBlockLength,
         const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingBlockLength, actingVersion);
        return *this;
    }

    std::uint64_t position(void) const
    {
        return m_position;
    }

    void position(const std::uint64_t position)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
        {
            throw std::runtime_error("buffer too short [E100]");
        }
        m_position = position;
    }

    std::uint64_t encodedLength(void) const
    {
        return position() - m_offset;
    }

    char *buffer(void)
    {
        return m_buffer;
    }

    std::uint64_t actingVersion(void) const
    {
        return m_actingVersion;
    }

    static const std::uint16_t capMgrIdId(void)
    {
        return 1;
    }

    static const std::uint64_t capMgrIdSinceVersion(void)
    {
         return 0;
    }

    bool capMgrIdInActingVersion(void)
    {
        return (m_actingVersion >= capMgrIdSinceVersion()) ? true : false;
    }


    static const char *capMgrIdMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const std::uint32_t capMgrIdNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const std::uint32_t capMgrIdMinValue()
    {
        return 0;
    }

    static const std::uint32_t capMgrIdMaxValue()
    {
        return 4294967293;
    }

    std::uint32_t capMgrId(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 0)));
    }

    LoginQry &capMgrId(const std::uint32_t value)
    {
        *((std::uint32_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint16_t capUnitIdId(void)
    {
        return 1;
    }

    static const std::uint64_t capUnitIdSinceVersion(void)
    {
         return 0;
    }

    bool capUnitIdInActingVersion(void)
    {
        return (m_actingVersion >= capUnitIdSinceVersion()) ? true : false;
    }


    static const char *capUnitIdMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const std::uint32_t capUnitIdNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const std::uint32_t capUnitIdMinValue()
    {
        return 0;
    }

    static const std::uint32_t capUnitIdMaxValue()
    {
        return 4294967293;
    }

    std::uint32_t capUnitId(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 4)));
    }

    LoginQry &capUnitId(const std::uint32_t value)
    {
        *((std::uint32_t *)(m_buffer + m_offset + 4)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint16_t tradeUnitIdId(void)
    {
        return 2;
    }

    static const std::uint64_t tradeUnitIdSinceVersion(void)
    {
         return 0;
    }

    bool tradeUnitIdInActingVersion(void)
    {
        return (m_actingVersion >= tradeUnitIdSinceVersion()) ? true : false;
    }


    static const char *tradeUnitIdMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const std::uint32_t tradeUnitIdNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const std::uint32_t tradeUnitIdMinValue()
    {
        return 0;
    }

    static const std::uint32_t tradeUnitIdMaxValue()
    {
        return 4294967293;
    }

    std::uint32_t tradeUnitId(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 8)));
    }

    LoginQry &tradeUnitId(const std::uint32_t value)
    {
        *((std::uint32_t *)(m_buffer + m_offset + 8)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint16_t traderId(void)
    {
        return 3;
    }

    static const std::uint64_t traderSinceVersion(void)
    {
         return 0;
    }

    bool traderInActingVersion(void)
    {
        return (m_actingVersion >= traderSinceVersion()) ? true : false;
    }


    static const char *traderMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

private:
    Trader m_trader;

public:

    Trader &trader(void)
    {
        m_trader.wrap(m_buffer, m_offset + 12, m_actingVersion, m_bufferLength);
        return m_trader;
    }
};
}
#endif
