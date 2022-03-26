import qbs

StaticLibrary {
    Depends { name: 'cpp' }
    name: 'oid_ncode'
    cpp.cxxLanguageVersion: "c++20"
    cpp.enableRtti: false
    cpp.includePaths: ["api", "inc"]
    install: true
    files: [
        "api/stdext/oid/ncode.hpp",
        "api/stdext/oid/ncode_raw.hpp",
        "api/stdext/oid/utils.hpp",
        "src/stdext/oid/encode.cpp",
        "src/stdext/oid/decode.cpp",
    ]
}
