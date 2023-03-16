#pragma once
#include <utility>

#include "DX_Types.h"

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

struct Vector4D {
	Vector4D() : x(0), y(0), z(0), w(0) {};
	Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	float x;
	float y;
	float z;

	float w;
};

struct O3D_Vertex {
	Vector3D position;
	Vector3D normal;
	Vector2D uv;
};

struct O3D_Transform {
	Vector4D x;
	Vector4D y;
	Vector4D z;

	Vector4D pos;
};

#pragma pack(push, 1)
struct O3D_Materials {
	D3DCOLORVALUE diffuse;
	D3DCOLOR_XRGB specular;
	D3DCOLOR_XRGB emissive;

	float power;
	std::uint8_t name_length;
	std::ptrdiff_t* name;
};

struct O3D_Tris_Long {
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint16_t id;
};

struct O3D_Tris_Short {
	uint16_t a;
	uint16_t b;
	uint16_t c;
	uint16_t id;
};

struct O3D_Faces_Short {
	uint16_t a;
	uint16_t b;
	uint16_t c;

	uint16_t material_id;
};

struct O3D_Faces_Long{
	std::uint32_t a;
	std::uint32_t b;
	std::uint32_t c;

	std::uint16_t material_id;
};

struct O3D_Weights {
	std::uint16_t index;
	float weight;
};

struct O3D_Bones {
	std::uint8_t name_length;
	std::uintptr_t* name;
	std::uint16_t weights_count;
	O3D_Weights* weights; //array
};
#pragma pack(pop)

struct O3D_Mesh {
	std::uint8_t unk[4];
	std::uintptr_t* filename;
	std::uint32_t num_faces;
	std::uint32_t num_vertices;
	
	std::uint32_t has_long_tris;
	O3D_Vertex* vertices;
	O3D_Faces_Short* short_faces;
	O3D_Faces_Long* long_faces;
	O3D_Materials* materials;
	D3DMATRIX matrix;
	O3D_Bones* weights;

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

enum O3D_DATA_TYPES : std::uint8_t {
	VERTEX = 0x17,
	MATERIALS = 0x26,
	TRIS = 0x49,
	BONES = 0x54,
	TRANSFORM = 0x79
};