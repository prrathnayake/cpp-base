import os
from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class BaseRecipe(ConanFile):
    name = "cpp_base"
    version = "1.0"
    
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)
        self.requires("app_utils/1.0")
        self.requires("app_rabbitmq/1.0")
        self.requires("app_kafka/1.0")
        self.requires("app_database/1.0")
        self.requires("app_blockchain/1.0")
        self.requires("app_algorithms/1.0")
        self.requires("app_h3/1.0")
        self.requires("app_http/1.0")
        self.requires("app_config/1.0")
        self.requires("app_auth/1.0")
        self.requires("app_monitoring/1.0")
        self.requires("app_cache/1.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)
