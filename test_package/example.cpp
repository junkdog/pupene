#include <string>
#include "pupdefs.h"
#include <pupene/json.h>
#include <pupene/binary.h>
#include <fstream>

const std::string raw_json = R"(
{
  "path": "font/primary.fnt",
  "inner": {
    "r": 1,
    "g": 1,
    "b": 0.5,
    "a": 1
  },
  "outer": {
    "r": 0.8,
    "g": 0.1,
    "b": 0.5,
    "a": 1
  }
})";

int main() {
    // load from json string
    auto fr = pupene::from_json<FontReference>(raw_json);

    // save binary representation to file
    std::ofstream out{"font_ref.bin", std::ofstream::out};
    pupene::to_binary(fr, out);
    out.flush();

    // load from binary data - debug print pup-ing
    std::ifstream in{"font_ref.bin", std::ofstream::in};
    auto ignored_fr = pupene::from_binary<FontReference>(in, true);

    return 0;
}
