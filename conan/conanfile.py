# Copyright (c) 2022, Rerito
# SPDX-License-Identifier: MIT

from conans import ConanFile, CMake, tools

class Penjing(ConanFile):
    name = "Penjing"
    version = "1.0"
    license = "MIT"
    description = "Header only library providing suffix tree implementation"
    homepage = "https://github.com/Rerito/Penjing"
    topics = ("suffix-tree", "searching")
    generators = "cmake_find_package", "cmake_paths"
    # MSVC builds cannot be tested at this time so its support is disabled.
    settings = {"build_type": None, "compiler": ["gcc", "clang", "Visual Studio"], "arch": None}

    def build(self):
        cmake = CMake(self)
        # Even though it is defined, it seems conan doesn't forward the cppstd
        # to CMake so let's enforce it.
        cmake.definitions['CMAKE_CXX_STANDARD'] = str(self.settings.compiler.cppstd)
        cmake.configure()
        cmake.build()
        cmake.test()
        cmake.install()

    def requirements(self):
        self.requires("gtest/1.11.0")

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            tools.check_min_cppstd(self,"17")

        compiler_version = tools.Version(self.settings.compiler.version)
        if self.settings.compiler == "gcc":
            min_gcc = "10.0"
            if compiler_version < min_gcc:
                raise ConanInvalidConfiguration("gcc minimum version is 10, got " +
                    str(compiler_version))
