#pragma once
#ifndef PCH
    #include <algorithm>
    #include <stdext/oid/utils.hpp>
#endif

namespace stdext::oid
{
    /// @brief Encode a numeric OID to binary form.
    template <typename T>
    int encode(const T* numeric_oid_begin, const T* const numeric_oid_end, std::uint8_t* binary_oid_dest) noexcept
    {
        auto start_dest = binary_oid_dest;
        std::uint8_t buffer[2u] = {0u};
        std::uint8_t index = 0u;

        auto try_store = [&]() noexcept
        {
            if (index == 2u)
            {
                index = 0u;
                *binary_oid_dest++ = std::uint8_t((buffer[1u] << 4u) | buffer[0u]);
            }
        };

        for (const auto* begin = numeric_oid_begin; begin != numeric_oid_end; begin = ++numeric_oid_begin)
        {
            auto no = *begin;
            if (no < 10u)
            {
                buffer[index++] = static_cast<std::uint8_t>(+symbol_t::no_0 + no);
                try_store();
            }
            else
            {
                auto bytes = byte_count(no);
                buffer[index++] = 10u + bytes;
                try_store();

                for (const std::uint8_t* src = reinterpret_cast<const std::uint8_t*>(&no) + bytes - 1; bytes--; --src)
                {
                    buffer[index++] = *src >> 4u;
                    try_store();

                    buffer[index++] = *src & 0xFu;
                    try_store();
                }
            }
        }

        if (index == 1u)
        {
            *binary_oid_dest++ = std::uint8_t(buffer[0u]);
        }

        return static_cast<int>(std::distance(start_dest, binary_oid_dest));
    }

    /// @brief Decode a binary OID using a functor.
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
                    oid_dest(static_cast<std::uint8_t>(+symbol - +symbol_t::no_0));
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
}
