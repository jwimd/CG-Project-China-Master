#include <iostream>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include "model.h"

Model::Model(const std::string& filepath) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<Vertex, uint32_t> uniqueVertices;
	std::vector<glm::vec3> v, vn;
	std::vector<glm::vec2> vt;
	std::string line, str;
	float f;
	int i = 0;

	std::ifstream filein;
	filein.open(filepath.c_str());

	while (getline(filein, line)) {
		if (line[0] == 'v') {
			if (line[1] == 'n') {
				std::istringstream in(line);
				glm::vec3 data;
				in >> str >> data.x >> data.y >> data.z;
				vn.push_back(data);
			}
			else if (line[1] == 't') {
				std::istringstream in(line);
				glm::vec2 data;
				in >> str >> data.x >> data.y;
				vt.push_back(data);
			}
			else {
				std::istringstream in(line);
				glm::vec3 data;
				in >> str >> data.x >> data.y >> data.z;
				v.push_back(data);
			}
		}
		else if (line[0] == 'f') {
			std::istringstream in(line);
			in >> str;
			if (count(line.begin(), line.end(), '/') == 6) {
				int index[3][3] = { 0, 0, 0 };
				Vertex vertex[3];
				for (int i = 0; i < 3; i++) {
					in >> str;
					size_t pos1 = str.find_first_of('/');
					size_t pos2 = str.find_last_of('/');
					if (pos1 != 0) {
						index[i][0] = stof(str.substr(0, pos1));
						vertex[i].position = v[index[i][0] - 1];
					}
					if (pos1 + 1 != pos2) {
						index[i][1] = stof(str.substr(pos1 + 1, pos2));
						vertex[i].texCoord = vt[index[i][1] - 1];
					}
					if (pos2 + 1 != str.length()) {
						index[i][2] = stof(str.substr(pos2 + 1));
						vertex[i].normal = vn[index[i][2] - 1];
					}
					if (uniqueVertices.count(vertex[i]) == 0) {
						uniqueVertices[vertex[i]] = static_cast<uint32_t>(vertices.size());
						vertices.push_back(vertex[i]);
					}
				}
				indices.push_back(uniqueVertices[vertex[0]]);
				indices.push_back(uniqueVertices[vertex[1]]);
				indices.push_back(uniqueVertices[vertex[2]]);
			}
			else if (count(line.begin(), line.end(), '/') == 8) {
				int index[4][3] = { 0, 0, 0 };
				Vertex vertex[4];
				for (int i = 0; i < 4; i++) {
					in >> str;
					size_t pos1 = str.find_first_of('/');
					size_t pos2 = str.find_last_of('/');
					if (pos1 != 0) {
						index[i][0] = stof(str.substr(0, pos1));
						vertex[i].position = v[index[i][0] - 1];
					}
					if (pos1 + 1 != pos2) {
						index[i][1] = stof(str.substr(pos1 + 1, pos2));
						vertex[i].texCoord = vt[index[i][1] - 1];
					}
					if (pos2 + 1 != str.length()) {
						index[i][2] = stof(str.substr(pos2 + 1));
						vertex[i].normal = vn[index[i][2] - 1];
					}
					if (uniqueVertices.count(vertex[i]) == 0) {
						uniqueVertices[vertex[i]] = static_cast<uint32_t>(vertices.size());
						vertices.push_back(vertex[i]);
					}
				}
				indices.push_back(uniqueVertices[vertex[0]]);
				indices.push_back(uniqueVertices[vertex[1]]);
				indices.push_back(uniqueVertices[vertex[2]]);
				indices.push_back(uniqueVertices[vertex[2]]);
				indices.push_back(uniqueVertices[vertex[3]]);
				indices.push_back(uniqueVertices[vertex[0]]);
			}
		}
	}

    _vertices = vertices;
    _indices = indices;
    
	_phongMaterial.reset(new PhongMaterial());
	_phongMaterial->ka = glm::vec3(0.03f, 0.03f, 0.03f);
	_phongMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);
	_phongMaterial->ks = glm::vec3(1.0f, 1.0f, 1.0f);
	_phongMaterial->ns = 10.0f;

    computeBoundingBox();

    initGLResources();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        cleanup();
        throw std::runtime_error("OpenGL Error: " + std::to_string(error));
    }
}

Model::Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    : _vertices(vertices), _indices(indices) {

    computeBoundingBox();

    initGLResources();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        cleanup();
        throw std::runtime_error("OpenGL Error: " + std::to_string(error));
    }
}

Model::Model(Model&& rhs) noexcept
    : _vertices(std::move(rhs._vertices)),
      _indices(std::move(rhs._indices)),
      _boundingBox(std::move(rhs._boundingBox)),
      _vao(rhs._vao), _vbo(rhs._vbo), _ebo(rhs._ebo) {
    _vao = 0;
    _vbo = 0;
    _ebo = 0;
}

Model::~Model() {
    cleanup();
}

BoundingBox Model::getBoundingBox() const {
    return _boundingBox;
}

void Model::draw() const {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

GLuint Model::getVao() const {
    return _vao;
}

size_t Model::getVertexCount() const {
    return _vertices.size();
}

size_t Model::getFaceCount() const {
    return _indices.size() / 3;
}

void Model::initGLResources() {
    // create a vertex array object
    glGenVertexArrays(1, &_vao);
    // create a vertex buffer object
    glGenBuffers(1, &_vbo);
    // create a element array buffer
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 
        sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);

    // specify layout, size of a vertex, data type, normalize, sizeof vertex array, offset of the attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Model::computeBoundingBox() {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = -std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();
    float maxZ = -std::numeric_limits<float>::max();

    for (const auto& v : _vertices) {
        minX = std::min(v.position.x, minX);
        minY = std::min(v.position.y, minY);
        minZ = std::min(v.position.z, minZ);
        maxX = std::max(v.position.x, maxX);
        maxY = std::max(v.position.y, maxY);
        maxZ = std::max(v.position.z, maxZ);
    }

	glm::vec3 min = glm::vec3(minX, minY, minZ);
	glm::vec3 max = glm::vec3(maxX, maxY, maxZ);
	_boundingBox.center = (min + max) / 2.0;
	_boundingBox.extents = max - _boundingBox.center;
}

void Model::cleanup() {
    if (_ebo != 0) {
        glDeleteBuffers(1, &_ebo);
        _ebo = 0;
    }

    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}

glm::mat4 Model::getModelMatrix() const {
	return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

void Model::saveAsObjFile(const std::string& filename) const {
	std::ofstream  fileout("../output/" + filename + ".obj", std::ios::out);
	fileout << "mtllib "+filename+".mtl" << std::endl;
	fileout << "g default" << std::endl;
	for (int i = 0; i < _vertices.size(); i++) {
		fileout << std::setiosflags(std::ios::fixed) << std::setprecision(6);
		fileout << "v " << _vertices[i].position.x << ' ' << _vertices[i].position.y << ' ' << _vertices[i].position.z << std::endl;
	}
	for (int i = 0; i < _vertices.size(); i++) {
		fileout << std::setiosflags(std::ios::fixed) << std::setprecision(6);
		fileout << "vt " << _vertices[i].texCoord.x << ' ' << _vertices[i].texCoord.y << std::endl;
	}
	for (int i = 0; i < _vertices.size(); i++) {
		fileout << std::setiosflags(std::ios::fixed) << std::setprecision(6);
		fileout << "vn " << _vertices[i].normal.x << ' ' << _vertices[i].normal.y << ' ' << _vertices[i].normal.z << std::endl;
	}
	fileout << "s 1" << std::endl;
	fileout << "g p"+filename+"1" << std::endl;
	fileout << "usemtl file1" << std::endl;
	for (int i = 0; i < (_indices.size() / 3); i++) {
		fileout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
		fileout << "f " << ' ' << _indices[3 * i] + 1 << '/' << _indices[3 * i] + 1 << '/' << _indices[3 * i] + 1 << ' '
								<< _indices[3 * i + 1] + 1 << '/' << _indices[3 * i + 1] + 1 << '/' << _indices[3 * i + 1] + 1 << ' '
								<< _indices[3 * i + 2] + 1 << '/' << _indices[3 * i + 2] + 1 << '/' << _indices[3 * i + 2] + 1 << std::endl;
	}
}

void Model::computeRotationQuat() {
	rotation = glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f };
	oldright = getRight();
	rotation = glm::quat{ cos(rotateangle.x / 2.0f), getRight().x * sin(rotateangle.x / 2.0f), getRight().y * sin(rotateangle.x / 2.0f), getRight().z * sin(rotateangle.x / 2.0f) } * rotation;
	oldup = getUp();
	rotation = glm::quat{ cos(rotateangle.y / 2.0f), getUp().x * sin(rotateangle.y / 2.0f), getUp().y * sin(rotateangle.y / 2.0f), getUp().z * sin(rotateangle.y / 2.0f) } * rotation;
	oldfront = getFront();
	rotation = glm::quat{ cos(rotateangle.z / 2.0f), -getFront().x * sin(rotateangle.z / 2.0f), -getFront().y * sin(rotateangle.z / 2.0f), -getFront().z * sin(rotateangle.z / 2.0f) } * rotation;
}

void Model::setPhongShader(GLSLProgram *shader) {
	shader->setMat4("model", getModelMatrix());
	shader->setVec3("material.ka", _phongMaterial->ka);
	shader->setVec3("material.kd", _phongMaterial->kd);
	shader->setVec3("material.ks", _phongMaterial->ks);
	shader->setFloat("material.ns", _phongMaterial->ns);
}

bool Model::isInBoundingBoxGlobal(Camera *camera) {
	_boundingBoxGlobal.center = getModelMatrix() * glm::vec4(_boundingBox.center, 1.0f);

	const glm::vec3 right = scale.x * getRight() * _boundingBox.extents.x;
	const glm::vec3 up = scale.y * getUp() * _boundingBox.extents.y;
	const glm::vec3 forward = scale.z * getFront() * _boundingBox.extents.z;

	_boundingBoxGlobal.extents.x = std::fabs(right.x) + std::fabs(up.x) + std::fabs(forward.x);
	_boundingBoxGlobal.extents.y = std::fabs(right.y) + std::fabs(up.y) + std::fabs(forward.y);
	_boundingBoxGlobal.extents.z = std::fabs(right.z) + std::fabs(up.z) + std::fabs(forward.z);

	if (std::fabs(camera->position.x - _boundingBoxGlobal.center.x) <= (_boundingBoxGlobal.extents.x + 0.15f) &&
		std::fabs(camera->position.y - _boundingBoxGlobal.center.y) <= (_boundingBoxGlobal.extents.y + 0.15f) &&
		std::fabs(camera->position.z - _boundingBoxGlobal.center.z) <= (_boundingBoxGlobal.extents.z + 0.15f)) return true;
	else return false;
}