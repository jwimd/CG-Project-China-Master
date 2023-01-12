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

// class DIYtexture{
//     public:
//     int type; //0 环绕 //1 顶部
//     int repeat;//一圈的重复数 顶部忽略
//     float l,r;//范围
//     int reverse; //反转
//     std::string name;
//     GLuint map;
//     DIYtexture(int t,int re,float l0,float r0,GLuint m){
//         type=t;
//         repeat=re;
//         l=l0;
//         r=r0;
//         map=m;
//         reverse=0;
//     }

//     DIYtexture(){type=repeat=l=map=0; r=1;};
//     ~DIYtexture(){};
// };

class DIYmodel : public Object3D
{
private:
    /* data */
    glm::vec3 offset;
    vector<glm::vec3> vertices; // 锚点
    int active_point;
    int active_tex;
    int material_idx;
    // DIYtexture material;
    vector<BezierFace> faces;
    // vector<DIYtexture> textures;

    GLuint VAO, VBO[3];

    vector<float> pvalues;  // 顶点坐标
    vector<float> tvalues;  // 纹理坐标
    vector<float> nvalues;  // 法线
    vector<float> fpvalues; // 锚点坐标
    vector<float> tfpvalues;
    vector<float> apvalues; // 锚点坐标

    int totalindex;

    // GLuint load_texture(string s,DIYtexture &diy);
    int load_model(vector<float> *pvalues, vector<float> *tvalues, vector<float> *nvalues);
    int load_frame(vector<float> *pvalues);
    // int load_circle(vector<float>* pvalues,int tid,bool lor);
    bool load_active(vector<float> *pvalues);
    void makeFaces();
    void init();
    int texIndex = 0;

public:
    std::unique_ptr<PhongMaterial> _phongMaterial;

    int get_point(float x, float y, Camera *camera);
    int get_line_start_point(float x, float y, Camera *camera);
    // int get_circle(float x,float y,Camera* camera,bool &lor);
    // int get_between_circle(float x,float y,Camera* camera);
    void active(int index);

    void split_point(int pid);
    void remove_point(int pid);
    void modify_point(float dx, float dy, Camera *camera);
    void modify_offset(float dx, float dy, Camera *camera, int d);

    int getTexIndex() { return texIndex; }
    void setTexIndex(int i) { texIndex = i; }

    glm::mat4 getModelMatrix();

    void remake();
    // void switch_material();

    // void add_texture();
    // void modify_circle(float dx,float dy,Camera camera,bool lor);
    // void remove_texture();
    // void add_repeat(bool t);
    // void trans_tex_type();
    // void reverse_tex();

    void draw();
    // void DrawFrame(Camera c,Shader s,bool d);
    // void DrawTexFrame(Camera c,Shader s,bool d);

    // void save_file();
    // void load_from_file();

    void setPhongShader(GLSLProgram *shader);

    DIYmodel();
    ~DIYmodel();
};
