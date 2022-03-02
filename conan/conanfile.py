# Copyright (c) 2022, Rerito
# SPDX-License-Identifier: MIT

from packaging import version
from conans import ConanFile, CMake, tools

class Penjing(ConanFile):
    name = "Penjing"
    version = "1.0"
    license = "MIT"
    description = "Header only library providing suffix tree implementation"
    homepage = "https://github.com/Rerito/Penjing"
    topics = ("suffix-tree", "searching")
    generators = "cmake_find_package", "cmake_paths"
    settings = {"build_type": None, "compiler": ["gcc", "clang"]}

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def requirements(self):
        self.requires("gtest/1.11.0")

    def validate(self):
        tools.check_min_cppstd(self,"20")

        compiler_version = version.parse(str(self.settings.compiler.version))
        if (self.settings.compiler == "gcc" and
            compiler_version < version.parse("10.0")):
            raise ConanInvalidConfiguration("gcc minimum version is 10, got " +
                str(compiler_version))
