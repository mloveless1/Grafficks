#include <Model.h>

Model::Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material){
	_mesh = mesh;
	_material = material;
}
