#define TINYOBJLOADER_IMPLEMENTATION
#include<Dependencies/tiny_obj_loader/include/tiny_obj_loader.h>
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<GraphicEngine/Utilities/Math/Vector2D.h>
#include<GraphicEngine/Utilities/Headers/VertexMesh.h>
#include<GraphicEngine/Utilities/UtilitiyFuncs/utilityFunctions.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/Mesh.h>

void computeTangent(
	const Vector3D& v0,
	const Vector3D& v1,
	const Vector3D& v2,
	const Vector2D& t0,
	const Vector2D& t1,
	const Vector2D& t2,
	Vector3D& tangent,
	Vector3D& binormal
)
{
	Vector3D deltaPos1 = v1 - v0;
	Vector3D deltaPos2 = v2 - v0;

	Vector2D deltaUV1 = t1 - t0;
	Vector2D deltaUV2 = t2 - t0;

	float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
	tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y);
	tangent = Vector3D::normalize(tangent);
	binormal = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x);
	binormal = Vector3D::normalize(binormal);
}




bool ParseObjFile(const wchar_t* full_path, std::vector<VertexMesh>& list_vertices, std::vector<unsigned int>& list_indices, std::vector<MaterialSlot>& list_material)
{
	//requirements for parsing obj
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;
	bool bMtl = true;

	const std::wstring wstr = full_path;
	std::string inputfile = UtilityFuncs::unicodeToMultibyte(wstr);
	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldir.c_str());

	/*if (materials.size() == 0)
	{
		throw std::exception("no MTL file found for current mesh");
	}*/

	if (!err.empty()) return false;
	if (!res) return false;

	size_t size_vertex_index_lists = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_vertex_index_lists += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_lists);
	list_indices.reserve(size_vertex_index_lists);
	list_material.resize(materials.size());

	size_t MaterialSize = materials.size();
	if (MaterialSize == 0)
	{
		bMtl = false;
		MaterialSize = 1;
		list_material.resize(MaterialSize);
	}

	size_t index_global_offset = 0;

	//Parsing materials
	for (size_t m = 0; m < MaterialSize; ++m)
	{
		list_material[m].start_index = index_global_offset;
		list_material[m].material_id = m;

		//Parsing OBJ - shapes
		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			//parsing faces
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				//check if face is of current material id
				if (bMtl)
				{
					if (shapes[s].mesh.material_ids[f] != m)
					{
						index_offset += num_face_verts;
						continue;
					}
				}


				//----------for calculating tangents and binormals per face/vertex---------//
				Vector3D vertices_face[3];
				Vector2D texcoords_face[3];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					vertices_face[v] = Vector3D(vx, vy, vz);
					texcoords_face[v] = Vector2D(tx, ty);
				}

				Vector3D tangent, binormal;
				computeTangent(vertices_face[0], vertices_face[1], vertices_face[2],
					texcoords_face[0], texcoords_face[1], texcoords_face[2],
					tangent, binormal);


				//parsing vertices of face
				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					//index of indice data, vertex data and other shit
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					tinyobj::real_t nx = 0;
					tinyobj::real_t ny = 0;
					tinyobj::real_t nz = 0;

					if (attribs.normals.size())
					{
						nx = attribs.normals[index.normal_index * 3 + 0];
						ny = attribs.normals[index.normal_index * 3 + 1];
						nz = attribs.normals[index.normal_index * 3 + 2];
					}

					Vector3D v_tangent, v_binormal;
					v_binormal = Vector3D::cross(Vector3D(nx, ny, nz), tangent);
					v_tangent = Vector3D::cross(v_binormal, Vector3D(nx, ny, nz));

					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz), v_tangent, v_binormal);
					list_vertices.push_back(vertex);
					list_indices.push_back((unsigned int)index_global_offset + v);

				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		list_material[m].num_indices = index_global_offset - list_material[m].start_index;
	}


	return true;
}