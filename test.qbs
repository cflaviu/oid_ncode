import qbs 1.0

CppApplication {
    Depends { name: 'oid_ncode' }
    consoleApplication: true
	cpp.debugInformation: true
	cpp.cxxLanguageVersion: "c++20"
	cpp.includePaths: [
	    "api",
		"inc",
	]
	cpp.staticLibraries: [
	    "/usr/local/lib/libCatch2Main.a",
		"/usr/local/lib/libCatch2.a",
	]
	files: [
	    "test/unit.cpp",
	]
}

