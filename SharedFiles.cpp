//
// Created by Josh Szczesniak on 3/13/21.
//

#include "SharedFiles.h"
#include <filesystem>
using namespace std;
namespace fs = filesystem;

SharedFiles::SharedFiles(fs::path & path) {
    fs::create_directory(path);
}
