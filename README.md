```bash
rm -rf ./build && mkdir build && cd build
```

```bash
cmake .. && cmake --build . -j $(nproc) && cmake --build . --target cbased-test_gfx -j $(nproc)
```

```bash
cmake -B build && cmake --build build -j (nproc) && cmake --build build --target tests
```
