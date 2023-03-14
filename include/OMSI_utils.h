#pragma once
#pragma pack(1)
#include <utility>

#include <DX_Types.h>

struct Vector2D {
	Vector2D() : x(0), y(0) {};
	Vector2D(float x, float y) : x(x), y(y) {};

	float x;
	float y;
};

struct Vector3D {
	Vector3D() : x(0), y(0), z(0) {};
	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {};
	
	float x;
	float y;
	float z;
};

struct O3D_Vertex {
	Vector3D position;
	Vector3D normal;
	Vector2D uv;
};


struct O3D_Materials {
	D3DCOLORVALUE diffuse;
	D3DCOLORVALUE specular;
	D3DCOLORVALUE emissive;

	std::uint32_t power;
	std::uintptr_t* texture; //check type
};

struct O3D_Faces {
	std::uint32_t a;
	std::uint32_t b;
	std::uint32_t c;

	std::uint16_t d;
};

//check all types
struct O3D_Weights {
	std::uintptr_t* name;
	std::uintptr_t* vertices;
	std::uintptr_t* weights;
};

struct O3D_Mesh {
	std::uint8_t unk[4];
	std::uintptr_t* filename;
	std::uint32_t num_faces;
	std::uint32_t num_vertices;
	
	std::uint32_t has_long_tris;
	O3D_Vertex* vertices;
	O3D_Faces* faces;
	O3D_Materials* materials;
	D3DMATRIX matrix;
	O3D_Weights* weights;

	std::uint32_t has_encryption;
	std::int32_t product_id;
};

struct OMSI_AddonItem {
	std::uintptr_t* addon_name;
	std::uintptr_t* id_as_str;

	std::uint32_t unk1;
	std::uint32_t unk2;

	std::uint32_t id_as_int;
};