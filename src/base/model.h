#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>

#include "vertex.h"
#include "object3d.h"
#include "camera.h"
#include "bounding_box.h"
#include "glsl_program.h"

struct PhongMaterial {
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	float ns;
};

class Model : public Object3D {
public:
	glm::vec3 oldright = { 0.0f, 0.0f, 0.0f };
	glm::vec3 oldup = { 0.0f, 0.0f, 0.0f };
	glm::vec3 oldfront = { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotateangle = { 0.0f, 0.0f, 0.0f };

	std::unique_ptr<PhongMaterial> _phongMaterial;

    Model(const std::string& filepath);

    Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

    Model(Model&& rhs) noexcept;

    virtual ~Model();

    GLuint getVao() const;

    size_t getVertexCount() const;

    size_t getFaceCount() const;

    BoundingBox getBoundingBox() const;

    virtual void draw() const;

	glm::mat4 getModelMatrix() const;

	void saveAsObjFile(const std::string& filename) const;

	void computeRotationQuat();

	void setPhongShader(GLSLProgram *shader);

	bool isInBoundingBoxGlobal(Camera *camera);

protected:
    // vertices of the table represented in model's own coordinate
    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;

    // bounding box
    BoundingBox _boundingBox;
	BoundingBox _boundingBoxGlobal;

    // opengl objects
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;

    void computeBoundingBox();

    void initGLResources();

    void cleanup();
};