#include <catch2/catch_all.hpp>
#include <stdext/oid/ncode_raw.hpp>

namespace stdext::oid
{
    using data_t = std::vector<std::uint16_t>;

    struct Simple_serializer
    {
        Simple_serializer(data_t& data) noexcept: _data(&data) {}

        void operator()(const data_t::value_type value) noexcept { _data->push_back(value); }

        int size() const noexcept { return static_cast<int>(_data->size()); }

    private:
        data_t* _data;
    };

    class Raw_fixture
    {
    public:
        void execute(data_t oid)
        {
            input_oid = std::move(oid);

            auto encoded = encoded_oid.data();
            encoded_size = encode<data_t::value_type>(input_oid.data(), input_oid.data() + input_oid.size(), encoded);

            Simple_serializer serializer(decoded_oid);
            decode(encoded_oid.data(), encoded_oid.data() + encoded_oid.size(), serializer);
        }

        data_t input_oid {};
        data_t decoded_oid {};
        std::array<std::uint8_t, 32> encoded_oid = {0};
        size_t encoded_size {};
    };

    TEST_CASE_METHOD(Raw_fixture, "test_raw1")
    {
        execute({1, 3, 6, 1, 4, 1, 9, 9, 492, 1, 1, 3});
        REQUIRE(input_oid == decoded_oid);
    }

    TEST_CASE_METHOD(Raw_fixture, "test_raw2")
    {
        execute({1, 3, 6, 1, 4, 1, 9, 9, 147, 1, 2, 2, 1, 1, 4, 2560});
        REQUIRE(input_oid == decoded_oid);
    }

    TEST_CASE_METHOD(Raw_fixture, "test_raw3")
    {
        execute({4, 1, 9, 9, 147, 1, 290, 2, 1, 1, 4, 2560});
        REQUIRE(input_oid == decoded_oid);
    }

    TEST_CASE_METHOD(Raw_fixture, "test_raw4")
    {
        execute({1, 3, 6, 1, 2, 1, 4, 24, 7, 1, 7, 1, 4, 0, 0, 0, 0, 0, 2, 0, 0, 1, 4, 192, 168, 1, 1});
        REQUIRE(input_oid == decoded_oid);
    }
}
