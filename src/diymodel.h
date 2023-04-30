#pragma once
#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "bezierface.h"

#include "base/camera.h"
#include "base/model.h"

// 模型类
// 一个模型由多个贝叶斯旋转面连接而成

struct DIYtexture
{
    int type;    // 0 环绕 //1 顶部
    int repeat;  // 一圈的重复数 顶部忽略
    float l, r;  // 范围
    int reverse; // 反转
    std::string name;
    GLuint map;
    bool isRev = false, isTop = false;
    float pos = -0.2f, width = 0.4f;
    DIYtexture(int t, int re, float l0, float r0, GLuint m) : type(m), repeat(re), l(l0), r(r0), map(m)
    {
    }

    DIYtexture()
    {
        type = repeat = l = map = 0;
        r = 1;
    };
    ~DIYtexture(){};
};

class DIYmodel : public Object3D
{
private:
    /* data */
    vector<glm::vec3> vertices; // 锚点
    int active_point;

    // int material_idx;
    vector<BezierFace> faces;

    GLuint VAO, VBO[3];
    GLuint VAO2, VBO2;
    GLuint VAO3, VBO3;

    vector<float> pvalues;  // 顶点坐标
    vector<float> tvalues;  // 纹理坐标
    vector<float> nvalues;  // 法线
    vector<float> fpvalues; // 锚点坐标
    vector<float> tfpvalues;
    vector<float> apvalues; // 锚点坐标

    int totalindex, findex;

    GLuint load_texture(string s, DIYtexture &diy);
    int load_model(vector<float> *pvalues, vector<float> *tvalues, vector<float> *nvalues);
    int load_frame(vector<float> *pvalues);
    bool load_active(vector<float> *pvalues);
    void makeFaces();
    void init();

    // bounding box
    BoundingBox _boundingBox;
    BoundingBox _boundingBoxGlobal;

public:
    vector<DIYtexture> textures;
    int active_tex;

    glm::vec3 oldright = {0.0f, 0.0f, 0.0f};
    glm::vec3 oldup = {0.0f, 0.0f, 0.0f};
    glm::vec3 oldfront = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotateangle = {0.0f, 0.0f, 0.0f};
    std::unique_ptr<PhongMaterial> _phongMaterial;

    int get_point(float x, float y, Camera *camera);
    int get_line_start_point(float x, float y, Camera *camera);
    void active(int index);

    void split_point(int pid);
    void remove_point(int pid);
    void modify_point(float dx, float dy, Camera *camera);

    glm::mat4 getModelMatrix();
    void computeRotationQuat();

    void remake();

    void add_texture(std::string name, GLuint tex);
    void remove_texture(int index);

    void draw();
    void drawFrame(GLSLProgram *f);

    glm::vec4 wild_screen_baroque(glm::vec4 pos, glm::mat4 view, glm::mat4 projection, glm::mat4 model);

    void save_file(std::string path);
    void load_from_file(std::string path);

    void setPhongShader(GLSLProgram *shader);

    bool isInBoundingBoxGlobal(Camera *camera);
    BoundingBox getBoundingBox() const;
    void computeBoundingBox();

    DIYmodel();
    ~DIYmodel();
};
