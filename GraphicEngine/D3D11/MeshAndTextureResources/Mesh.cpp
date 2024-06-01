#include "Mesh.h"
#include <GraphicEngine/Utilities/Headers/VertexMesh.h>

Mesh::Mesh(const int& mesh_ID, std::wstring mesh_name)
	:
	uID(mesh_ID),
	meshName(mesh_name)
{
}

Mesh::~Mesh()
{
}

std::vector<VertexMesh> Mesh::GetVertexMesh()
{
	return VertexAttributes;
}

std::vector<unsigned int> Mesh::GetIndicesList()
{
	return IndicesList;
}

const std::vector<MaterialSlot>& Mesh::GetMaterials()
{
	return MaterialList;
}

const unsigned int& Mesh::getSizeVertices()
{
	if (!SizeVertices) SizeVertices = VertexAttributes.size();
	return SizeVertices;
}

const unsigned int& Mesh::getSizeIndices()
{
	if (!SizeIndices) SizeIndices = IndicesList.size();
	return SizeIndices;
}

const unsigned int& Mesh::getVertexSize()
{
	if (!VertexSize) VertexSize = sizeof(VertexMesh);
	return VertexSize;
}

const size_t& Mesh::GetMaterialCount()
{
	if (!MaterialCount) MaterialCount = MaterialList.size();
	return MaterialCount;
}

const int& Mesh::GetmeshID()
{
	return uID;
}

std::wstring Mesh::GetMeshName()
{
	return meshName;
}

void Mesh::incrementCounter()
{
	UserCounter++;
}

void Mesh::decrementCounter()
{
	UserCounter--;
}
