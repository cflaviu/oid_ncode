#include "stdext/oid/ncode.hpp"
#include "stdext/oid/utils.hpp"
#include <array>

namespace stdext::oid
{
    template <typename Dest>
    void decode(const std::uint8_t* binary_oid_begin, const std::uint8_t* const binary_oid_end, Dest& oid_dest) noexcept
    {
        std::uint8_t index = 0u;

        auto get_nibble = [&]() noexcept -> symbol_t
        {
            const auto result = static_cast<symbol_t>((index == 0u) ? *binary_oid_begin & 0xFu : *binary_oid_begin >> 4u);
            if (++index == 2u)
            {
                index = 0u;
                ++binary_oid_begin;
            }

            return result;
        };

        while (binary_oid_begin != binary_oid_end)
        {
            const auto symbol = get_nibble();
            switch (symbol)
            {
                case symbol_t::no_0:
                case symbol_t::no_1:
                case symbol_t::no_2:
                case symbol_t::no_3:
                case symbol_t::no_4:
                case symbol_t::no_5:
                case symbol_t::no_6:
                case symbol_t::no_7:
                case symbol_t::no_8:
                case symbol_t::no_9:
                {
                    oid_dest(static_cast<std::uint8_t>(+symbol - 1u));
                    break;
                }
                case symbol_t::no_1_byte:
                case symbol_t::no_2_bytes:
                case symbol_t::no_3_bytes:
                case symbol_t::no_4_bytes:
                {
                    std::uint32_t no = 0u;
                    for (auto count = std::uint8_t((+symbol - +symbol_t::no_9) << 1u); count--;)
                    {
                        no = (no << 4u) | +get_nibble();
                    }

                    oid_dest(no);
                    break;
                }
                case symbol_t::none:
                default:
                {
                    binary_oid_begin = binary_oid_end;
                    break;
                }
            }
        }
    }

    struct Serializer
    {
        Serializer(char* oid_dest) noexcept: _oid_dest(oid_dest), _ptr(oid_dest) {}

        void operator()(const std::uint8_t value) noexcept
        {
            *_ptr++ = static_cast<char>(value + '0');
            *_ptr++ = '.';
        }

        void operator()(const uint32_t no) noexcept
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
