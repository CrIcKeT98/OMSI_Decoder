#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

#include "OMSI_utils.h"

bool checkFile(std::string&& filename);
bool decodeFile(std::string&& filename);
void mixSalt(std::uint16_t& salt_product_id, std::uint16_t num_vertices, std::uint8_t& salt);