#include "stdext/oid/ncode.hpp"
#include "stdext/oid/ncode_raw.hpp"
#include "stdext/oid/utils.hpp"
#include <array>

namespace stdext::oid
{
    struct Serializer
    {
        Serializer(char* oid_dest) noexcept: _oid_dest(oid_dest), _ptr(oid_dest) {}

        void operator()(const std::uint8_t value) noexcept
        {
            *_ptr++ = static_cast<char>(value + '0');
            *_ptr++ = '.';
        }

        void operator()(const std::uint32_t no) noexcept
        {
            auto begin = decimal_convert(no, _buffer.end());
            std::copy(begin, _buffer.end(), _ptr);
            _ptr += std::distance(begin, _buffer.end());
            *_ptr++ = '.';
        }

        int size() const noexcept { return std::distance(_oid_dest, _ptr - 1); }

    private:
        std::array<char, 10u> _buffer {};
        char* _oid_dest;
        char* _ptr;
    };

    int decode(const std::uint8_t* binary_oid_begin, const std::uint8_t* const binary_oid_end, char* oid_dest) noexcept
    {
        Serializer serializer(oid_dest);
        decode(binary_oid_begin, binary_oid_end, serializer);
        return serializer.size();
    }

} // namespace stdext::oid
