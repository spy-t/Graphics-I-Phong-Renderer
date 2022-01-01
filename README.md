# Graphics Project

## Building

```bash
make deps
make release_build
cd build
make -j <num threads> # (or cmake --build build if on cmake 3.15+)
```

## Running

```bash
./build_release/graphics_project
```
