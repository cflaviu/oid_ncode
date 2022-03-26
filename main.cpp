#include "stdext/oid/ncode.hpp"
#include <array>
#include <iostream>
#include <string>

int main()
{
    using namespace stdext::oid;

    // std::string input_oid= "1.3.6.1.4.1.9.9.492.1.1.3";
    // std::string input_oid= "1.3.6.1.4.1.9.9.147.1.2.2.1.1.4.2560";
    // std::string input_oid = "4.1.9.9.147.1.290.2.1.1.4.2560";
    std::string input_oid = "1.3.6.1.2.1.4.24.7.1.7.1.4.0.0.0.0.0.2.0.0.1.4.192.168.1.1";

    std::array<std::uint8_t, 32> encoded_oid = {0};
    std::string decoded_oid;
    decoded_oid.resize(64);

    auto encoded = encoded_oid.data();
    auto encoded_size = encode(input_oid.data(), input_oid.data() + input_oid.size(), encoded);
    auto decoded = const_cast<char*>(decoded_oid.data());
    auto decoded_size = decode(encoded_oid.data(), encoded_oid.data() + encoded_oid.size(), decoded);
    decoded_oid.resize(decoded_size);

    std::cout << "input-size=" << input_oid.size() << '\n';
    std::cout << "encoded-size=" << encoded_size << '\n';
    std::cout << "decoded-size=" << decoded_size << '\n';
    std::cout << "decoded-oid=" << decoded_oid << '\n';
    std::cout << "same-result=" << (input_oid == decoded_oid) << '\n';
    return 0;
}
