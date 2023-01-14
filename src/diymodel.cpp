// #include <windows.h>
// #include <commdlg.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "diymodel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "base/camera.h"

#include "base/stb_image.h"

const float SCR_WIDTH = 1280;
const float SCR_HEIGHT = 720;

float defPoints[] = {

    0.0,
    0.0,
    1,
    1.0,
    0.0,
    0,
    2.0,
    0.0,
    0,
    3.0,
    0.0,
    1,
    4.0,
    2.0,
    0,
    4.0,
    4.0,
    0,
    3.0,
    6.0,
    1,
    2.0,
    6.5,
    0,
    1.0,
    7.0,
    0,
    0.0,
    7.0,
    1,

};

float shiness[] = {

    16.0f, 256.0f, 64.0f, 64.0f, 32.0f, 32.0f

};
// 加载单点到数组
bool DIYmodel::load_active(vector<float> *pvalues)
{
    pvalues->clear();
    if (active_point < 0)
        return false;
    auto p = vertices[active_point];
    pvalues->push_back(p.x);
    pvalues->push_back(p.y);
    return true;
}
// 加载线框模型到数组
int DIYmodel::load_frame(vector<float> *pvalues)
{
    pvalues->clear();
    int index = 0;
    for (auto p : vertices)
    {
        pvalues->push_back(p.x);
        pvalues->push_back(p.y);
        pvalues->push_back(0.0);
        index++;
    }
    return index;
}
// 加载模型到数组
int DIYmodel::load_model(vector<float> *pvalues, vector<float> *tvalues, vector<float> *nvalues)
{
    int index = 0;
    pvalues->clear();
    tvalues->clear();
    nvalues->clear();
    for (auto myBezier : faces)
    {
        vector<int> ind = myBezier.getIndices();
        vector<Vertex> verts = myBezier.getVertices();
        for (int i = 0; i < myBezier.getNumIndices(); i++)
        {
            pvalues->push_back(verts[ind[i]].position.x);
            pvalues->push_back(verts[ind[i]].position.y);
            pvalues->push_back(verts[ind[i]].position.z);
            tvalues->push_back(verts[ind[i]].texCoord.s);
            tvalues->push_back(verts[ind[i]].texCoord.t);
            nvalues->push_back(verts[ind[i]].normal.x);
            nvalues->push_back(verts[ind[i]].normal.y);
            nvalues->push_back(verts[ind[i]].normal.z);
        }
        index += myBezier.getNumIndices();
    }
    return index;
}

int DIYmodel::load_circle(vector<float> *pvalues, int tid, bool lor)
{
    pvalues->clear();
    float y, r, pos;
    if (lor)
    {
        pos = textures[tid].l;
    }
    else
        pos = textures[tid].r;
    for (BezierFace f : faces)
    {
        if (f.getRadiance(pos, y, r))
        {
            for (float j = 0; j <= 100; j++)
            {
                double theta = j / 100 * 2 * 3.14159;
                pvalues->push_back(r * cos(theta));
                pvalues->push_back(y);
                pvalues->push_back(r * sin(theta));
            }
            return 303;
        }
    }
}

// 初始化
void DIYmodel::init()
{
    active_point = -1;
    active_tex = 0;
    material_idx = 0;
    offset = glm::vec3(0, 0, 0);
    for (int i = 0; i < 10; i++)
    {
        vertices.push_back(glm::vec3(defPoints[3 * i], defPoints[3 * i + 1], defPoints[3 * i + 2]));
    }
    makeFaces();
    computeBoundingBox();
}

// 初始化建立面
void DIYmodel::makeFaces()
{
    faces.clear();

    for (int i = 0; i + 9 < 30; i += 9)
    {
        vector<glm::vec2> vec;
        for (int j = 0; j < 4; j++)
            vec.push_back(glm::vec2(defPoints[i + 3 * j], defPoints[i + 3 * j + 1]));
        BezierFace bf = BezierFace(vec);

        faces.push_back(bf);
    }
}

// 刷新
void DIYmodel::remake()
{

    // load_texture(basic_texs[material_idx], material);
    faces.clear();
    float totl = 0;
    vector<float> lengths;
    for (int i = 0; i + 3 < vertices.size(); i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            float t = (vertices[i + j + 1].x - vertices[i + j].x) * (vertices[i + j + 1].x - vertices[i + j].x) +
                      (vertices[i + j + 1].y - vertices[i + j].y) * (vertices[i + j + 1].y - vertices[i + j].y);
            totl += sqrt(t);
            lengths.push_back(sqrt(t));
        }
    }

    float off = 0;

    for (int i = 0; i + 3 < vertices.size(); i += 3)
    {
        vector<glm::vec2> vec;

        for (int j = 0; j < 4; j++)
            vec.push_back(glm::vec2(vertices[i + j].x, vertices[i + j].y));

        float l = off;
        float r = l + (lengths[i] + lengths[i + 1] + lengths[i + 2]) / totl;
        BezierFace bf = BezierFace(vec, l, r);
        off = r;

        faces.push_back(bf);
    }

    totalindex = load_model(&pvalues, &tvalues, &nvalues);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STREAM_DRAW);
}

DIYmodel::DIYmodel()
{
    _phongMaterial.reset(new PhongMaterial());
    _phongMaterial->ka = glm::vec3(0.03f, 0.03f, 0.03f);
    _phongMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);
    _phongMaterial->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    _phongMaterial->ns = 10.0f;
    init();
}

DIYmodel::~DIYmodel()
{
}

bool clamp(float x, float y, float diff)
{

    if (abs(x - y) < diff)
    {
        return true;
    }
    return false;
}

glm::vec4 wild_screen_baroque(glm::vec4 pos, glm::mat4 view, glm::mat4 projection, glm::mat4 model)
{

    pos = model * pos;
    pos = view * pos;
    pos = projection * pos;

    pos.x = pos.x / pos.w;
    pos.y = pos.y / pos.w;
    pos.z = pos.z / pos.w;
    pos.w = 1;

    pos.x = (SCR_WIDTH / 2) * (pos.x + 1);
    pos.y = (SCR_HEIGHT / 2) * (1 - pos.y);

    return pos;
}

void DIYmodel::active(int index)
{
    if (index == active_point)
    {
        active_point = -1;
    }
    else
    {
        active_point = index;
    }
}
// 移动节点
void DIYmodel::modify_point(float dx, float dy, Camera *camera)
{
    if (active_point < 0)
        return;
    else
    {
        float z = camera->position.z - offset.z;
        float dist = 155 / z;
        if (z < 0)
        {
            vertices[active_point].y += dy / (-dist);
            vertices[active_point].x += dx / (dist);
        }
        else
        {
            vertices[active_point].y += dy / (dist);
            vertices[active_point].x += dx / (dist);
        }
    }
}

// 移动整体
void DIYmodel::modify_offset(float dx, float dy, Camera *camera, int dimension)
{
    if (active_point < 0)
        return;
    else
    {
        float z = camera->position.z - offset.z;
        float dist = 1000 / z;

        if (dimension == 0)
        {
            offset.x += dx / (dist);
        }
        if (dimension == 1)
        {
            if (z < 0)
            {
                offset.y += dy / (-dist);
            }
            else
            {
                offset.y += dy / (dist);
            }
        }
        if (dimension == 2)
        {
            if (z < 0)
            {
                offset.z -= dy / (dist);
            }
            else
            {
                offset.z -= dy / (dist);
            }
        }
    }
}
// 删除节点
void DIYmodel::remove_point(int pid)
{
    if (pid <= 0)
        return;
    if (pid == vertices.size() - 1)
        return;
    // 首尾不能删除
    if (pid % 3 != 0)
        return;

    int i = 0;

    for (vector<glm::vec3>::iterator it = vertices.begin(); it != vertices.end(); i++)
    {
        if (i == pid - 1)
        {
            vertices.erase(it, it + 3);
            return;
        }
        else
        {
            it++;
        }
    }
}
// 新建节点
void DIYmodel::split_point(int pid)
{
    if (pid < 0)
        return;
    int i = 0;
    for (vector<glm::vec3>::iterator it = vertices.begin(); it != vertices.end(); i++)
    {
        if (i == pid + 1)
        {
            // 在此点和下一个点之间插入三个点
            glm::vec4 pos0(vertices[i - 1].x, vertices[i - 1].y, 0.0, 1.0);
            glm::vec4 pos1(vertices[i].x, vertices[i].y, 0.0, 1.0);
            glm::vec4 posx;

            for (float j = 1; j <= 3; j++)
            {
                posx = (j / 4) * pos0 + (1 - j / 4) * pos1;
                it = vertices.insert(it, posx);
            }

            return;
        }
        else
        {
            it++;
        }
    }
}

void DIYmodel::modify_circle(float dx, float dy, Camera* camera, bool lor)
{
    if (active_tex < 0)
        return;
    else
    {
        float z = camera.Position.z;
        if (z < 0)
            z = 0 - z;
        float dist = 10000 / z;
        if (lor)
        {
            textures[active_tex].l += dy / (dist);
            if (textures[active_tex].l > 1)
                textures[active_tex].l = 0.999;
            if (textures[active_tex].l < 0)
                textures[active_tex].l = 0;
        }

        else
        {
            textures[active_tex].r += dy / (dist);
            if (textures[active_tex].r > 1)
                textures[active_tex].r = 0.999;
            if (textures[active_tex].r < 0)
                textures[active_tex].r = 0;
        }
    }
}

// 根据鼠标查找线段
int DIYmodel::get_point(float x, float y, Camera *camera)
{
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model;
    model = getModelMatrix();
    float zc = camera->position.z;

    for (int i = 0; i < vertices.size(); i++)
    {
        glm::vec4 pos(vertices[i].x, vertices[i].y, 0.0, 1.0);
        pos = wild_screen_baroque(pos, view, projection, model);

        //     cout<<"point "<<i<<"at"<<pos.x<<","<<pos.y<<endl;

        if (clamp(pos.x, x, 10.0) && clamp(pos.y, y, 10.0))
        {
            active(i);
            return i;
        }
    }
    return -1;
}

// 根据鼠标查找线段
int DIYmodel::get_line_start_point(float x, float y, Camera *camera)
{
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model;
    model = getModelMatrix();
    float zc = camera->position.z;

    for (int i = 0; i + 1 < vertices.size(); i++)
    {
        glm::vec4 pos0(vertices[i].x, vertices[i].y, 0.0, 1.0);
        pos0 = wild_screen_baroque(pos0, view, projection, model);

        glm::vec4 pos1(vertices[i + 1].x, vertices[i + 1].y, 0.0, 1.0);
        pos1 = wild_screen_baroque(pos1, view, projection, model);

        //     cout<<"point "<<i<<"at"<<pos.x<<","<<pos.y<<endl;

        bool c1 = clamp((y - pos0.y) / (x - pos0.x), (y - pos1.y) / (x - pos1.x), 1.0);     // 共线
        bool c0 = clamp((x - pos0.x) / (y - pos0.y), (x - pos1.x) / (y - pos1.y), 1.0);     // 共线
        bool c2 = (x < pos0.x + 5 && x > pos1.x - 5) || (x < pos1.x + 5 && x > pos0.x - 5); // 在线段上
        bool c3 = (y < pos0.y + 5 && y > pos1.y - 5) || (y < pos1.y + 5 && y > pos0.y - 5); // 在线段上
        if ((c1 || c0) && c2 && c3)
            return i;
    }
    return -1;
}
// 根据鼠标寻找大圆
int DIYmodel::get_circle(float x, float y, Camera* camera, bool &lor)
{
    glm::mat4 projection = glm::perspective(glm::radians(camera->fovy), float(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model;
    model = glm::translate(model, offset);
    float zc = camera->position.z;

    float yl, yr, r;

    for (int i = 0; i < textures.size(); i++)
    {
        for (BezierFace f : faces)
        {
            if (f.getRadiance(textures[i].l, yl, r))
            {

                break;
            }
        }
        glm::vec4 pos(r, yl, 0.0, 1.0);
        pos = wild_screen_baroque(pos, view, projection, model);
        if (clamp(pos.y, y, 10.0))
        {
            lor = true;
            active_tex = i;
            return i;
        }

        for (BezierFace f : faces)
        {
            if (f.getRadiance(textures[i].r, yr, r))
            {

                break;
            }
        }
        pos = glm::vec4(r, yr, 0.0, 1.0);
        pos = wild_screen_baroque(pos, view, projection, model);
        if (clamp(pos.x, x, 10.0) && clamp(pos.y, y, 10.0))
        {

            lor = false;
            active_tex = i;
            return i;
        }
    }
    return -1;
}

void DIYmodel::load_from_file(std::string path)
{
    // 显示选择的文件。
    ifstream is;
    is.open(path);
    glm::vec3 t;
    // DIYtexture dt;
    vertices.clear();
    // textures.clear();
    string s;
    int size;

    // is>>s;//"material"
    // is>>material_idx;
    is >> s;
    is >> size;

    for (int i = 0; i < size; i++)
    {
        is >> t.x >> t.y >> t.z;
        vertices.push_back(t);
    }

    // is>>s;
    // is>>size;

    // for (int i=0;i<size;i++)
    // {
    //     is>>dt.type>>dt.repeat>>dt.reverse>>dt.l>>dt.r>>dt.name;
    //     load_texture(dt.name,dt);
    //     textures.push_back(dt);
    // }

    is.close();
}

void DIYmodel::save_file(std::string path)
{

    ofstream os;
    os.open(path);

    // os<<"material"<<endl;
    // os<<material_idx<<endl;
    os << "vertices_begin" << ' ' << vertices.size() << endl;

    for (auto x : vertices)
    {
        os << x.x << ' ' << x.y << ' ' << x.z << endl;
    }

    // os<<"texs_begin"<<' '<<textures.size()<<endl;
    // for(auto x:textures){
    //     os<<x.type<<' '<<x.repeat<<' '<<x.reverse<<' '<<x.l<<' '<<x.r<<' '<<x.name<<endl;
    // }

    // os<<"texs_end"<<endl;

    os.close();
}

void DIYmodel::draw()
{

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, totalindex);
    glBindVertexArray(0);
}

void DIYmodel::drawFrame(GLSLProgram *frameShader)
{
    findex = load_frame(&fpvalues);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, fpvalues.size() * 4, &fpvalues[0], GL_STREAM_DRAW);

    bool active = load_active(&apvalues);
    if (active)
    {
        glGenVertexArrays(1, &VAO3);
        glGenBuffers(1, &VBO3);
        glBindVertexArray(VAO3);
        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
        glBufferData(GL_ARRAY_BUFFER, apvalues.size() * 4, &apvalues[0], GL_STREAM_DRAW);
    }

    frameShader->setVec3("color", glm::vec3(1, 0.0, 1));
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, findex);

    frameShader->setVec3("color", glm::vec3(1.0, 1.0, 1.0));

    glPointSize(8);
    glDrawArrays(GL_POINTS, 0, findex);

    if (active)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(0);
        frameShader->setVec3("color", glm::vec3(0.0, 1.0, 0.0));
        glDrawArrays(GL_POINTS, 0, 1);
    }

    glDepthFunc(GL_LESS);

    fpvalues.clear();
}

// void DIYmodel::DrawTexFrame(Camera camera, Shader frameShader, bool texframedisplay)
// {
//     fpvalues.clear();
//     if (textures.size() == 0)
//         return;

//     glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
//     glm::mat4 view = camera.GetViewMatrix();
//     glm::mat4 model;
//     model = glm::translate(model, offset);

//     frameShader.use();
//     frameShader.setMat4("projection", projection);
//     frameShader.setMat4("view", view);
//     frameShader.setMat4("model", model);

//     for (int i = 0; i < textures.size(); i++)
//     {
//         frameShader.setVec3("color", glm::vec3(0.0, 0.5, 0.5));
//         findex = load_circle(&fpvalues, i, true);

//         glGenVertexArrays(1, &VAO2);
//         glGenBuffers(1, &VBO2);
//         glBindVertexArray(VAO2);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//         glBufferData(GL_ARRAY_BUFFER, fpvalues.size() * 4, &fpvalues[0], GL_STREAM_DRAW);

//         glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
//         glEnableVertexAttribArray(0);

//         if (texframedisplay && i == active_tex)
//             glDrawArrays(GL_LINE_STRIP, 0, findex);

//         glDisable(GL_DEPTH_TEST);
//         frameShader.setVec3("color", glm::vec3(1.0, 1.0, 1.0));
//         if (texframedisplay)
//             glDrawArrays(GL_POINTS, 0, 1);

//         glEnable(GL_DEPTH_TEST);

//         frameShader.setVec3("color", glm::vec3(0.5, 0.0, 0.5));

//         findex = load_circle(&fpvalues, i, false);
//         glGenVertexArrays(1, &VAO2);
//         glGenBuffers(1, &VBO2);
//         glBindVertexArray(VAO2);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//         glBufferData(GL_ARRAY_BUFFER, fpvalues.size() * 4, &fpvalues[0], GL_STREAM_DRAW);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
//         glEnableVertexAttribArray(0);

//         if (texframedisplay && i == active_tex)
//             glDrawArrays(GL_LINE_STRIP, 0, findex);

//         glDisable(GL_DEPTH_TEST);
//         frameShader.setVec3("color", glm::vec3(1.0, 1.0, 1.0));
//         if (texframedisplay)
//             glDrawArrays(GL_POINTS, 0, 1);

//         glEnable(GL_DEPTH_TEST);
//     }

//     model = glm::mat4(1.0);
//     frameShader.setMat4("model", model);
// }

// GLuint DIYmodel::load_texture(string s, DIYtexture &diytex)
// {
//     GLuint texture;
//     glGenTextures(1, &texture);
//     glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
//     // set the texture wrapping parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     // set texture filtering parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     // load image, create texture and generate mipmaps
//     int width, height, nrChannels;
//     // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
//     unsigned char *data = stbi_load(s.c_str(), &width, &height, &nrChannels, 0);
//     if (data)
//     {
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//     }
//     else
//     {
//         std::cout << "Failed to load texture" << std::endl;
//     }
//     stbi_image_free(data);
//     diytex.map = texture;

//     return texture;
// }

// void DIYmodel::switch_material()
// {

//     material_idx += 1;
//     if (material_idx >= 6)
//         material_idx = 0;
// }

// void DIYmodel::add_texture()
// {
//     DIYtexture newtx;

//     OPENFILENAME ofn;       // 公共对话框结构。
//     TCHAR szFile[MAX_PATH]; // 保存获取文件名称的缓冲区。
//     // 初始化选择文件对话框。
//     ZeroMemory(&ofn, sizeof(OPENFILENAME));
//     ofn.lStructSize = sizeof(OPENFILENAME);
//     ofn.hwndOwner = NULL;
//     ofn.lpstrFile = szFile;
//     ofn.lpstrFile[0] = '\0';
//     ofn.nMaxFile = sizeof(szFile);
//     ofn.lpstrFilter = "All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";
//     ofn.nFilterIndex = 1;
//     ofn.lpstrFileTitle = NULL;
//     ofn.nMaxFileTitle = 0;
//     ofn.lpstrInitialDir = NULL;

//     ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//     // ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);
//     //  显示打开选择文件对话框。

//     if (GetOpenFileName(&ofn))
//     {
//         load_texture(szFile, newtx);

//         newtx.l = 0.3;
//         newtx.r = 0.4;
//         newtx.repeat = 4;
//         newtx.reverse = 0;
//         newtx.name = szFile;

//         active_tex = textures.size();
//         textures.push_back(newtx);
//     }
// }

void DIYmodel::add_repeat(bool t)
{
    if (t)
    {
        textures[active_tex].repeat++;
    }
    else
    {
        textures[active_tex].repeat--;
        if (textures[active_tex].repeat < 1)
            textures[active_tex].repeat = 1;
    }
}

void DIYmodel::trans_tex_type()
{
    textures[active_tex].type = 1 - textures[active_tex].type;
}
void DIYmodel::reverse_tex()
{
    textures[active_tex].reverse = 1 - textures[active_tex].reverse;
}

void DIYmodel::remove_texture()
{
    if (textures.size() <= 0)
        return;
    vector<DIYtexture>::iterator i = textures.begin() + active_tex;
    active_tex -= 1;
    textures.erase(i);
}

void DIYmodel::setPhongShader(GLSLProgram *shader)
{
    shader->setMat4("model", getModelMatrix());
    shader->setVec3("material.ka", _phongMaterial->ka);
    shader->setVec3("material.kd", _phongMaterial->kd);
    shader->setVec3("material.ks", _phongMaterial->ks);
    shader->setFloat("material.ns", _phongMaterial->ns);
}

glm::mat4 DIYmodel::getModelMatrix()
{
    return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

void DIYmodel::computeRotationQuat()
{
    rotation = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
    oldright = getRight();
    rotation = glm::quat{cos(rotateangle.x / 2.0f), getRight().x * sin(rotateangle.x / 2.0f), getRight().y * sin(rotateangle.x / 2.0f), getRight().z * sin(rotateangle.x / 2.0f)} * rotation;
    oldup = getUp();
    rotation = glm::quat{cos(rotateangle.y / 2.0f), getUp().x * sin(rotateangle.y / 2.0f), getUp().y * sin(rotateangle.y / 2.0f), getUp().z * sin(rotateangle.y / 2.0f)} * rotation;
    oldfront = getFront();
    rotation = glm::quat{cos(rotateangle.z / 2.0f), -getFront().x * sin(rotateangle.z / 2.0f), -getFront().y * sin(rotateangle.z / 2.0f), -getFront().z * sin(rotateangle.z / 2.0f)} * rotation;
}

void DIYmodel::computeBoundingBox()
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = -std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();
    float maxZ = -std::numeric_limits<float>::max();

    for (auto face : faces)
        for (const auto &v : face.getVertices())
        {
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

BoundingBox DIYmodel::getBoundingBox() const
{
    return _boundingBox;
}

bool DIYmodel::isInBoundingBoxGlobal(Camera *camera)
{
    _boundingBoxGlobal.center = getModelMatrix() * glm::vec4(_boundingBox.center, 1.0f);

    const glm::vec3 right = scale.x * getRight() * _boundingBox.extents.x;
    const glm::vec3 up = scale.y * getUp() * _boundingBox.extents.y;
    const glm::vec3 forward = scale.z * getFront() * _boundingBox.extents.z;

    _boundingBoxGlobal.extents.x = std::fabs(right.x) + std::fabs(up.x) + std::fabs(forward.x);
    _boundingBoxGlobal.extents.y = std::fabs(right.y) + std::fabs(up.y) + std::fabs(forward.y);
    _boundingBoxGlobal.extents.z = std::fabs(right.z) + std::fabs(up.z) + std::fabs(forward.z);

    if (std::fabs(camera->position.x - _boundingBoxGlobal.center.x) <= (_boundingBoxGlobal.extents.x + 0.15f) &&
        std::fabs(camera->position.y - _boundingBoxGlobal.center.y) <= (_boundingBoxGlobal.extents.y + 0.15f) &&
        std::fabs(camera->position.z - _boundingBoxGlobal.center.z) <= (_boundingBoxGlobal.extents.z + 0.15f))
        return true;
    else
        return false;
}