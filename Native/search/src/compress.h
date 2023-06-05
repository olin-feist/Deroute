#include <iostream>
#include <fstream>
#include <string.h>
#include <regex>
#include <sstream>
#include "utils.h"

typedef _Float16 real;
/**
 * Compresses a .vec file by prunning words and using half precisions
*/
int compress(std::string path, std::string out_path);