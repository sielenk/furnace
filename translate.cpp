#include "db.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdio>

//                   1     2     3     4     5     6     7     8     9
//                   10     11    12    13    14    15     16    17
//                   18    19    20    21    22    23    24    25
//                   26    27    28    29    30     31     32     33
//                   34    35    36    37    38    39    40    41 42
// 13.01.16 21:10:28 00002 00000 00473 00427 00920 00147 00000 00600
// 00413 -32768 00705 00373 00049 00000 -32768 00064 00200 00411 00403
// 00411 00406 00000 00000 10000 09963 09838 08943 01000 01000 -32768
// -32768 -32768 51456 00000 00000 00077 00444 00211 00300 01000 01000
// 00000

struct Sample {
  int day, month, year, hour, minute, second;
  short int v[42];

  Sample(std::string const& line) {
    auto const result(
        sscanf(line.c_str(),
               "%2d.%2d.%2d %2d:%2d:%2d"
               " %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd"
               " %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd"
               " %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd"
               " %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd"
               " %hd %hd",
               &day, &month, &year, &hour, &minute, &second, v + 0,
               v + 1, v + 2, v + 3, v + 4, v + 5, v + 6, v + 7, v + 8,
               v + 9, v + 10, v + 11, v + 12, v + 13, v + 14, v + 15,
               v + 16, v + 17, v + 18, v + 19, v + 20, v + 21, v + 22,
               v + 23, v + 24, v + 25, v + 26, v + 27, v + 28, v + 29,
               v + 30, v + 31, v + 32, v + 33, v + 34, v + 35, v + 36,
               v + 37, v + 38, v + 39, v + 40, v + 41));

    if (result != 48) {
      throw std::runtime_error("invalid line");
    }
  }
};


int main(int argc, char const* argv[]) {
  std::ifstream file("lines.txt");
  std::string line;

  while (getline(file, line)) {
    try {
      Sample sample(line);

      std::cout << sample.hour << ':' << sample.minute << ':'
                << sample.second << std::endl;
    } catch (std::runtime_error const& e) {
    }
  }

  return 0;
}
