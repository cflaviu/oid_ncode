#include <catch2/catch_all.hpp>
#include <stdext/oid/ncode.hpp>

namespace stdext::oid
{
    class Fixture
    {
    public:
        void execute(const char* oid)
        {
            input_oid = oid;
            decoded_oid.resize(input_oid.size());

            auto encoded = encoded_oid.data();
            encoded_size = encode(input_oid.data(), input_oid.data() + input_oid.size(), encoded);
            auto decoded = const_cast<char*>(decoded_oid.data());
            decoded_size = decode(encoded_oid.data(), encoded_oid.data() + encoded_oid.size(), decoded);
        }

        std::string input_oid {};
        std::string decoded_oid {};
        std::array<std::uint8_t, 32> encoded_oid = {0};
        size_t encoded_size {};
        size_t decoded_size {};
    };

    TEST_CASE_METHOD(Fixture, "test1")
    {
        execute("1.3.6.1.4.1.9.9.492.1.1.3");
        REQUIRE(encoded_size < decoded_size);
        REQUIRE(input_oid.size() == decoded_size);
        REQUIRE(input_oid == decoded_oid);
    }

    TEST_CASE_METHOD(Fixture, "test2")
    {
        execute("1.3.6.1.4.1.9.9.147.1.2.2.1.1.4.2560");
        REQUIRE(encoded_size < decoded_size);
        REQUIRE(input_oid.size() == decoded_size);
        REQUIRE(input_oid == decoded_oid);
    }

    TEST_CASE_METHOD(Fixture, "test3")
    {
        execute("4.1.9.9.147.1.290.2.1.1.4.2560");
        REQUIRE(encoded_size < decoded_size);
        REQUIRE(input_oid.size() == decoded_size);
        REQUIRE(input_oid == decoded_oid);
    }

    TEST_CASE_METHOD(Fixture, "test4")
    {
        execute("1.3.6.1.2.1.4.24.7.1.7.1.4.0.0.0.0.0.2.0.0.1.4.192.168.1.1");
        REQUIRE(encoded_size < decoded_size);
        REQUIRE(input_oid.size() == decoded_size);
        REQUIRE(input_oid == decoded_oid);
    }
}
