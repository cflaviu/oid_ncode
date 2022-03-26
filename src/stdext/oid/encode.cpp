#include "stdext/oid/ncode.hpp"
#include "stdext/oid/ncode_raw.hpp"
#include <algorithm>
#include <array>

namespace stdext::oid
{
    int encode(const char* oid_begin, const char* const oid_end, std::uint8_t* binary_oid_dest) noexcept
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

        bool ok = true;
        for (const auto* begin = oid_begin; ok && begin != oid_end + 1; begin = ++oid_begin)
        {
            oid_begin = std::find(oid_begin, oid_end, '.');
            auto count = static_cast<std::uint8_t>(std::distance(begin, oid_begin));
            switch (count)
            {
                case 0u:
                {
                    break;
                }
                case 1u:
                {
                    buffer[index++] = std::uint8_t(*begin - '0' + 1);
                    try_store();
                    break;
                }
                case 2u:
                case 3u:
                case 4u:
                case 5u:
                case 6u:
                case 7u:
                case 8u:
                case 9u:
                case 10u:
                {
                    std::uint32_t value = atou(begin, count);
                    auto bytes = byte_count(value);
                    buffer[index++] = 10u + bytes;
                    try_store();

                    for (const std::uint8_t* src = reinterpret_cast<const std::uint8_t*>(&value) + bytes - 1; bytes--; --src)
                    {
                        buffer[index++] = *src >> 4u;
                        try_store();

                        buffer[index++] = *src & 0xFu;
                        try_store();
                    }

                    break;
                }
                default:
                {
                    ok = false;
                }
            }
        }

        if (ok && index == 1u)
        {
            buffer[1u] = 0u;
            *binary_oid_dest++ = std::uint8_t((buffer[1u] << 4u) | buffer[0u]);
        }

        return ok ? static_cast<int>(std::distance(start_dest, binary_oid_dest)) : -1;
    }
}
