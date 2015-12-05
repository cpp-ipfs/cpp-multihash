# cpp-multihash

[multihash](//github.com/jbenet/multihash) implementation in c++.

## Example

Run [bin/multihash](src/main.cpp)

```
cd build
cmake ..
make
echo -n "foo" | ../bin/multihash --hash-type sha1
11140beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33 -
```

## License

MIT
