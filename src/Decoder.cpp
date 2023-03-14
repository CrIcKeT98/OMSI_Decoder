#include "Decoder.h"

bool checkFile(std::string&& filename) {
	using namespace std;

	ifstream in_file(filename, ios_base::binary);
	in_file.unsetf(std::ios::skipws);

	if (!in_file.is_open()) {
		cout << "Can't open file!" << endl << "Check filename or path" << endl;
		return false;
	}

	if (uint16_t l_header; in_file.read((char*)(&l_header), 2) && l_header != 0x1984) {
		cout << "This is not a valid o3d file!" << endl;
		return false;
	}

	uint8_t file_version;
	in_file.read((char*)(&file_version), 1);

	if (file_version > 7) {
		cout << "The file is too new for this OMSI version!" << endl;
		return false;
	}

	in_file.close();

	return true;
}

bool decodeFile(std::string&& filename) {
	using namespace std;

	string filename_out(filename.begin(), filename.end() - 4);
	filename_out += "_decoded.o3d";

	ifstream  in_file(filename, ios::binary);
	ofstream out_file(filename_out, ios::binary);
	out_file << in_file.rdbuf();
	in_file.close();
	out_file.close();

	fstream o3d_file(filename_out, ios_base::binary | ios_base::in | ios_base::out);
	
	O3D_Mesh mesh;
	o3d_file.seekp(2, ios::beg);

	uint8_t file_version;
	o3d_file.read((char*)(&file_version), 1);

	if (file_version < 3) {
		mesh.has_long_tris = false;
		mesh.has_encryption = false;
	}
	else {
		uint8_t buff;
		o3d_file.read((char*)(&buff), 1);
		mesh.has_long_tris = buff & 1;
		mesh.has_encryption = buff & 2;
	}

	bool o3d_has_product_id;
	uint16_t product_id_salt;

	if (file_version < 4)
		o3d_has_product_id = 0;
	else {
		o3d_file.read((char*)(&mesh.product_id), 4);

		if (mesh.product_id == 0xFFFFFFFF) {
			cout << "This modes is not encrypted!" << endl;
			return false;
		}

		if (mesh.product_id == 0xFFFF)
			o3d_has_product_id = false;	
		else
			o3d_has_product_id = true;
		
		if (o3d_has_product_id)
			product_id_salt = mesh.product_id + file_version - 4;
		else
			product_id_salt = mesh.product_id;
	}

	if (o3d_has_product_id) {
		if (mesh.has_encryption)
			product_id_salt = (product_id_salt + 381) % 65000;
		else
			product_id_salt = product_id_salt % 65000;
	}
	else {
		product_id_salt = 0;
		mesh.product_id = 0;
	}

	uint8_t data_type;
	uint32_t data_count;
	bool l_flag = true;

	uint32_t vertex_section;

	while (l_flag && !o3d_file.eof()) {
		o3d_file.read((char*)(&data_type), 1);
		uint32_t l_buff = 0;

		if (file_version >= 3)
			o3d_file.read((char*)(&data_count), 4);
		else
			o3d_file.read((char*)(&data_count), 2);

		switch (data_type)
		{
			case O3D_DATA_TYPES::VERTEX:
				l_buff = product_id_salt + 0x02;
				if (l_buff > 0xFFFF) {
					cout << "OUT OF RANGE VERTICES" << endl;
					return false;
				}

				product_id_salt = l_buff;

				if (data_count < 0) {
					cout << "UNCORRECT NUM OF VERTICES!" << endl;
					return false;
				}

				vertex_section = o3d_file.tellg();
				l_flag = false;
				break;

			case O3D_DATA_TYPES::MATERIALS:
				o3d_file.seekg(data_count * sizeof(O3D_Materials), ios::cur);
				break;

			case O3D_DATA_TYPES::TRIS:
				if(mesh.has_long_tris)
					o3d_file.seekg(data_count * sizeof(O3D_Tris_Long), ios::cur);
				else
					o3d_file.seekg(data_count * sizeof(O3D_Tris_Short), ios::cur);
				
				break;

			case O3D_DATA_TYPES::BONES:
				o3d_file.seekg(data_count * sizeof(O3D_Bones), ios::cur);
				break;

			case O3D_DATA_TYPES::TRANSFORM:
				o3d_file.seekg(data_count * sizeof(O3D_Transform), ios::cur);
				break;

			default:
				break;
		}
	}

	mesh.num_vertices = data_count;
	unique_ptr<O3D_Vertex[]> readVertices(new O3D_Vertex[mesh.num_vertices]);

	mesh.num_vertices %= 65000;
	uint32_t l_counter = mesh.num_vertices;
	uint32_t vert_index = 0;
	uint8_t salt = 0;

	//read O3D_Vertex
	do {
		o3d_file.read((char*)(&readVertices[vert_index]), sizeof(O3D_Vertex));

		if (o3d_has_product_id) {
			if (!mesh.product_id) {
				if (mesh.has_encryption)
					product_id_salt = 304u;
				else
					product_id_salt = 0;
			}

			mixSalt(product_id_salt, mesh.num_vertices, salt);

			float integral_x;
			float fractional_x = modf(readVertices[vert_index].position.x, &integral_x);

			float integral_y;
			float fractional_y = modf(readVertices[vert_index].position.y, &integral_y);

			float integral_z;
			float fractional_z = modf(readVertices[vert_index].position.z, &integral_z);

			salt = fabsf(fractional_x * fractional_y * fractional_z) * 600.0f;;

			if (product_id_salt >= 1000u) {
				if (product_id_salt >= 3000u) {
					if (product_id_salt > 7000u) {
						swap(readVertices[vert_index].position.y, readVertices[vert_index].position.z);
					}
				}
				else {
					swap(readVertices[vert_index].position.x, readVertices[vert_index].position.z);
				}
			}
			else {
				swap(readVertices[vert_index].position.x, readVertices[vert_index].position.y);
			}

			if ((product_id_salt & 3) == 0)
				readVertices[vert_index].normal.x = -readVertices[vert_index].normal.x;
			if (!(product_id_salt % 6u))
				readVertices[vert_index].normal.y = -readVertices[vert_index].normal.y;
			if (!(product_id_salt % 7u))
				readVertices[vert_index].normal.z = -readVertices[vert_index].normal.z;
			if (product_id_salt >= 600u) {
				if (product_id_salt > 4500u)
					swap(readVertices[vert_index].normal.x, readVertices[vert_index].normal.y);
			}
			else
				swap(readVertices[vert_index].normal.y, readVertices[vert_index].normal.z);
			
			if (!(product_id_salt % 5u))
				readVertices[vert_index].uv.x = readVertices[vert_index].uv.x - (product_id_salt % 0x64u * (product_id_salt % 0x64u) / 10000.0f);
			
			if (!(product_id_salt % 3u))
				readVertices[vert_index].uv.y = readVertices[vert_index].uv.y - (product_id_salt % 0x32u * (product_id_salt % 0x32u) / 2500.0f);
		}

		vert_index++;
		l_counter--;
	
	} while (l_counter);

	o3d_file.seekp(4, ios::beg);
	uint32_t decoded_product_id = 0xFFFFFFFF;
	o3d_file.write(reinterpret_cast<char*>(&decoded_product_id), sizeof(uint32_t));
	o3d_file.seekp(vertex_section, ios::beg);

	for (size_t i = 0; i < mesh.num_vertices; ++i) {
		o3d_file.write(reinterpret_cast<char*>(&readVertices[i]), sizeof(O3D_Vertex));
	}
}

void mixSalt(std::uint16_t& product_id_salt, std::uint16_t num_vertices, std::uint8_t& salt) {
	std::uint16_t buff = ((salt * num_vertices) + (num_vertices * product_id_salt)) % 8000;
	product_id_salt = buff;
	salt = buff / 8000;
}