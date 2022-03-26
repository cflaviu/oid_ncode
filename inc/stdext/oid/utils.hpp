#pragma once
#ifndef PCH
    #include <cstdint>
    #include <type_traits>
#endif

namespace stdext::oid
{
    template <typename T>
    char* decimal_convert(T no, char* buffer_end) noexcept
    {
        auto p = buffer_end;
        do
        {
            *--p = static_cast<char>('0' + (no % 10u));
            no /= 10u;
        } while (no != 0);
        return p;
    }

    inline std::uint32_t atou(const char* ptr, const char* const ptr_end) noexcept
    {
        std::uint32_t value = 0;
        for (auto count = std::uint8_t(ptr_end - ptr); count--; ++ptr)
        {
            value = value * 10 + std::uint32_t(*ptr - '0');
        }

        return value;
    }

    template <typename T>
    std::uint32_t atou(const char* ptr, T count) noexcept
    {
        std::uint32_t value = 0;
        for (; count--; ++ptr)
        {
            value = value * 10 + std::uint32_t(*ptr - '0');
        }

        return value;
    }

    template <typename T, std::uint8_t storage_bytes>
    struct bytes_needed;

    template <typename T>
    struct bytes_needed<T, 1u>
    {
        constexpr std::uint8_t operator()(const T /*value*/) const noexcept { return 1u; }
    };

    template <typename T>
    struct bytes_needed<T, 2u>
    {
        std::uint8_t operator()(const T value) const noexcept { return (value < 0xFFu) ? 1u : 2u; }
    };

    template <typename T>
    struct bytes_needed<T, 4u>
    {
        std::uint8_t operator()(const T value) const noexcept
        {
            return (value < 0xFFFFu) ? (value < 0xFFu) ? 1u : 2u : (value < 0xFFFFFFu) ? 3u : 4u;
        }
    };

    template <typename T>
    std::uint8_t byte_count(const T value) noexcept
    {
        return bytes_needed<T, sizeof(T)>()(value);
    }

    template <typename T>
    constexpr auto operator+(T e) noexcept //-> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>>
    {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    enum class symbol_t : std::uint8_t
    {
        none = 0,

        no_0,
        no_1,
        no_2,
        no_3,
        no_4,
        no_5,
        no_6,
        no_7,
        no_8,
        no_9,

        no_1_byte, // no > 9
        no_2_bytes,
        no_3_bytes,
        no_4_bytes,

        reserved
    };
}
