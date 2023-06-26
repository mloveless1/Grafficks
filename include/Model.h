#pragma once
#include <mesh.h>
#include <Material.h>

class Model{
public:
	Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	Material* GetMaterial() { return _material.get(); }
	Mesh* GetMesh() { return _mesh.get(); }

private:
	std::shared_ptr<Material> _material;
	std::shared_ptr<Mesh> _mesh;

};

