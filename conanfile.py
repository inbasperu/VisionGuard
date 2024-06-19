from conan import ConanFile


class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # self.requires("openvino/2024.1.0")
        self.requires("openvino/2024.1.0@_/_#f3d07ec52c65324d631928da6f540821a224a6b0", override=True)
        self.requires("opencv/4.9.0")
        self.requires("qt/6.7.1")
        self.requires("libpng/1.6.43", override=True)
        

    def layout(self):
        self.folders.generators = ""