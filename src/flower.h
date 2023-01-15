#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
using namespace std;
#include "base/camera.h"
#include "base/texture.h"
#include "base/glsl_program.h"
#include "base/light.h"

struct Flower
{
    glm::vec3 position;
    glm::vec3 rotation;
    float rotation_i;
    float rotation_y;
    glm::vec3 velocity;
    bool statical;
    int life;
};

class FlowerSystem
{
private:
    /* data */
    
    int maxlifespan;
    unsigned int VAO, VBO[2];
    vector<float> pvalues; // 顶点坐标
    vector<float> tvalues; // 纹理坐标

    // square
    const float square[18] = {
        // positions            // texture coords
        0.5f, 0.5f, 0.0f,  //   // top right
        0.5f, -0.5f, 0.0f, //  // bottom right
        -0.5f, 0.5f, 0.0f, //   // top left

        0.5f, -0.5f, 0.0f,  //  // bottom right
        -0.5f, -0.5f, 0.0f, //   // bottom left
        -0.5f, 0.5f, 0.0f,  //    // top left
    };

    const float texcoords[12] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f

    };

    

    const float gravity = 20.0f;

    GLSLProgram *shader;

protected:
public:
    int type = 0;
    vector<Flower> flowers;
    const string flowerpath[2] = {
        "../media/resources/flower.jpg",
        "../media/resources/maple.jpg"};
    std::unique_ptr<Texture2D> texture[2];
    FlowerSystem();

    ~FlowerSystem(){}

    // void init(int totnum);
    void update(GLfloat deltaTime);
    void draw(Camera* c);

};