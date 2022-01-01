SRCS := $(shell find src -type f -name "*.cpp" -or -name "*.h" -or -name "*.hpp")

all:
	@echo "format - run clang format on source files"
	@echo "dev_build - initialize cmake for development"
	@echo "release_build - initialize cmake for release"

deps:
	mkdir -p third_party/assimp
	curl -sSfL https://github.com/assimp/assimp/archive/refs/tags/v5.1.5.tar.gz | tar xvz -C third_party/assimp --strip-components=1
	mkdir -p third_party/glfw
	curl -sSfL https://github.com/glfw/glfw/archive/refs/tags/3.3.6.tar.gz | tar xvz -C third_party/glfw --strip-components=1
	mkdir -p third_party/glm
	curl -sSfL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz | tar xvz -C third_party/glm --strip-components=1

format:
	clang-format -i $(SRCS)

dev_build:
	cmake -S . -B build_debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -G Ninja

release_build:
	cmake -S . -B build_release -DCMAKE_BUILD_TYPE=Release

.PHONY: deps format dev_build release_build
