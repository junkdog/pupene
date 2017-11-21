
## pupene

A non-invasive, multi-format serialization library for C++17.


### Features or Caveats 

This library was primarily conceived as a foundation for assisting with building 
tooling for gamedev projects. The approach and naming conventions are borrowed
from Daniel Randle's [Pupping - a method for serializing data][article]. 

- Works with default constructible types.
  - `pupene` relies on direct field access to read/write. 
- Concise and simple:
    - `pup()` - one serializer-agnostic read/write function per type.
    - `Pupper` - serializer, binary and optional JSON support included. 
    - Serialization format can not be customized, this makes it non-ideal for
      dealing with third-party formats.
    - Zero dependencies, unless built with JSON support. 
- Not limited to performing only pure serialization.
  - Example using [imgui and pupene][imgui_example] to auto-generate editor UI:s at runtime. 

 [imgui_example]: https://github.com/junkdog/pupene_example
 [article]: https://www.gamedev.net/articles/programming/general-and-gameplay-programming/pupping-a-method-for-serializing-data-r4485/


### Links

- Full API documentation, including this README: https://junkdog.github.io/pupene/index.html
- Github repository: https://github.com/junkdog/pupene
- Issues: https://github.com/junkdog/pupene/issues


### A brief introduction 

At the core, `pupene` is built around serializers called _puppers_, and `pup`
functions - a combined read/write per serializable type.

Including `<pupene/pupene.h>` gets everything except JSON support, which
is done with `<pupene/json.h>` - when built with `PUPENE_BUILD_JSON=true`.

Refer to the [API docs](https://junkdog.github.io/pupene) for more detailed
documentation. The [tests][tests] may be of some interest too.

Namespace `pupene` is omitted from example code, in the interest of brevity.

 [tests]: https://github.com/junkdog/pupene/tree/master/test


#### Pupper

`Pupper` is on the receiving end of `pup`; they only deal directly with values once
`pup` has flattened the object into a sequence of integer, decimal and string types. 

Included Puppers, and related convenience functions:
- JsonReader: `from_json()`
- JsonWriter: `to_json()`
- BinaryReader: `from_binary()`
- BinaryWriter: `to_binary()`
- DebugPupper: `to_debug()`
 

A `pup` function must exist for each serializable type. 

#### pup 

An object is serializable when there's a matching `pup` function; these describe 
the structure of the type by enumerating its fields. All `pup` functions must
be defined in the `pupene::fns` namespace.

A limited number of fundamental types are built-in:
- Integer and decimal types
- `std::string`
- STL containers, such as `std::vector` and `std::map`

`pup()` for a type `FooBar` has the following signature:

```cpp
template <typename P>
void pup(Pupper<P>& p,         // Pupper-agnostic - works with all  
         FooBar& value,        // must be reference
         const Meta& meta);    // name and Meta::Type
```


#### Example

You have some models:

```cpp
struct Color {
    float r, g, b, a;
};

struct FontReference {
    std::string path;             
    Color inner;
    Color outer;
};
```

To make the types eligible, each requires its own pup function in the namespace `pupene:fns`.

```cpp
namespace pupene::fns {               // required namespace

    template<typename P>
    void pup(Pupper<P>& p,            // the serializer in use 
             Color& color,            // always as non-const references 
             const Meta& meta) {      // metadata for color 
        
        // idiomatic boilerplate     
        pup_object(p, color, meta, [&color](auto&& fpup) {
            fpup(color.r, "r");       // enumerate each field
            fpup(color.g, "g");
            fpup(color.b, "b");
            fpup(color.a, "a");
        });
    }
    
    template<typename P>
    void pup(Pupper<P>& p,          
             FontReference& ref,    
             const Meta& meta) {    
        
        pup_object(p, ref, meta, [&ref](auto&& fpup) {
            fpup(ref.path,  "path");  
            fpup(ref.inner, "inner"); // dispatches to pup for Color
            fpup(ref.outer, "outer");
        });
    }
}
```

Once the necessary pup:s are implemented, serialization between different
format becomes possible e.g.:

```cpp
// load binary from input stream
std::istream& in = ...;
Color color = from_binary<Color>(in);

// save to json (assumes built with PUPENE_BUILD_JSON=true)
std::string raw_json = to_json(color);
```

`Color` and `FontReference` can now also be combined with some STL containers: 

```cpp
std::unordered_map<Color, FontReference> colored_fonts = {
    {colorA, small}, {colorB, medium}, {colorC, large}
};

// complex keys - such as Color - are ok
std::string raw_json = to_json(colored_fonts);

// (this doesn't require any user-supplied pup:s)
std::vector<int> numbers = {0, 1, 6, -2, 4, 5, 7};
to_binary(numbers, std::cout);
```


### Building

Set up project:

```bash
cmake -H. -B_build \
    -DCMAKE_INSTALL_PREFIX=_install \
    -DCMAKE_BUILD_TYPE=Release \
    -DPUPENE_BUILD_TESTS=true
```

Build project:
```bash
cmake --build _build
```

Tests are run with:

```bash
cmake --build _build  --target test    
```

Tests can also run with valgrind/memcheck:
```bash
cmake --build _build --target valgrind
```

### Using

#### Hunter 

[Hunter](https://github.com/ruslo/hunter) is a package manager for CMake.  

```cmake
### hunter init, top of CMakeLists.txt
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.99.tar.gz"
    SHA1 "55ca271e9276b9973176e3f5a8c0d0dc25e46a53"
    LOCAL # optional, will use cmake/Hunter/config.cmake 
)

### adding pupene
hunter_add_package(pupene)
find_package(pupene CONFIG REQUIRED)

### ... later, when linking
target_link_libraries(example_project PUBLIC pupene::pupene)

```

Build-time configuration can be customized in `cmake/Hunter/config.cmake`, for example:  

```cmake
hunter_config(
    pupene
    CMAKE_ARGS
      PUPENE_BUILD_JSON=true
)
```

#### Plain CMake
TODO
 

### Overarching TODO
- Types currently require a default constructor to be eligble for `pup`.
- Benchmarking and profiling remain.

### Alternative libraries:
- [MetaStuff](https://github.com/eliasdaler/MetaStuff): C++14, similar but expanded features compared to `pupene`
- [cereal](https://github.com/USCiLab/cereal): C++11, only binary serialization(?)
