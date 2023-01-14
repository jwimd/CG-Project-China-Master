#pragma once

#include <memory>
#include <vector>

#include <glad/glad.h>
#include "base/stb_image.h"
#include "base/stb_image_write.h"

#include "base/application.h"
#include "base/glsl_program.h"
#include "base/camera.h"
#include "base/model.h"
#include "base/skybox.h"
#include "base/light.h"
#include "base/framebuffer.h"
#include "base/fullscreen_quad.h"
#include "base/texture.h"

#include "diymodel.h"

struct Wall
{
	Model *floor;
	Model *back;
	Model *left;
	Model *right;
};

struct Geometry
{
	Model *cylinder;
	Model *cone;
	Model *prism;
	Model *cube;
	Model *prismaticTable4;
	Model *prismaticTable6;
	Model *knot;
	DIYmodel *gameObject;
};

struct Tex
{
	std::unique_ptr<Texture2D> wood;
	std::unique_ptr<Texture2D> brick;
	std::unique_ptr<Texture2D> marbleBrown;
	std::unique_ptr<Texture2D> marblePurple;
	std::unique_ptr<Texture2D> metalBare;
	std::unique_ptr<Texture2D> metalPainted;
	std::unique_ptr<Texture2D> plastic;
	std::unique_ptr<Texture2D> table;
	std::unique_ptr<Texture2D> objTex[4];
	std::unique_ptr<Texture2D> objSideTex[2];
	std::unique_ptr<Texture2D> objTopTex[2];
};

struct TransparentMaterial
{
	glm::vec3 albedo;
	float ka;
	glm::vec3 kd;
	float transparent;
};

enum class RenderMode
{
	PhongTexture,
	AlphaBlending
};

class FinalProject : public Application
{
public:
	FinalProject(const Options &options);

	~FinalProject();

	void handleInput() override;

	void renderFrame() override;

private:
	std::vector<Camera *> _cameras;

	enum RenderMode _renderMode = RenderMode::PhongTexture;

	int activeCameraIndex = 0;

	int sunOrder = 0;
	int sunOrderSign = 1;

	int panIndex = 0;

	int postureFrameNumber = 0;
	int playOrder = 1;

	bool showCursor = true;
	bool firstPressControl = true;
	bool opencd = false;
	bool gameMode = false;
	bool PointSelect = false;
	bool notChange = true;

	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	std::vector<std::unique_ptr<Model>> _postures;

	struct Wall _wall;

	struct Geometry _geometry;

	std::unique_ptr<Model> _spotLightSphere;

	struct Tex _texture;

	std::unique_ptr<SkyBox> _skybox;
	std::unique_ptr<SkyBox> gameSkybox;

	GLSLProgram *_phongShader;
	GLSLProgram *_dancerShader;
	GLSLProgram *_sphereShader;
	GLSLProgram *_frameShader;
	GLSLProgram *_depthShader;

	std::unique_ptr<GLSLProgram> _depthPeelingInitShader;
	std::unique_ptr<GLSLProgram> _depthPeelingShader;
	std::unique_ptr<GLSLProgram> _depthPeelingBlendShader;
	std::unique_ptr<GLSLProgram> _depthPeelingFinalShader;

	// lights
	std::unique_ptr<DirectionalLight>
		_directionalLight;
	std::unique_ptr<SpotLight> _spotLight;

	std::unique_ptr<GLSLProgram> _alphaBlendingShader;

	// depth peeling resources
	std::unique_ptr<TransparentMaterial> _knotMaterial;
	std::unique_ptr<TransparentMaterial> _diyMaterial;
	std::unique_ptr<FullscreenQuad> _fullscreenQuad;

	std::unique_ptr<Framebuffer> _colorBlendFbo;
	std::unique_ptr<DataTexture2D> _colorBlendTexture;
	std::unique_ptr<Framebuffer> _fbos[2];
	std::unique_ptr<DataTexture2D> _colorTextures[2];
	std::unique_ptr<DataTexture2D> _depthTextures[2];

	// gameMode

	void initPhongShader();
	void initSphereShader();
	void initDancerShader();
	void initFrameShader();
	void initDepthShader();

	void initDepthPeelingShaders();
	void initDepthPeelingResources();
	void renderWithDepthPeeling(Model *model);

	void initAlphaBlendingShader();
	void renderWithAlphaBlending(Model *model);
	void renderWithAlphaBlending(DIYmodel *model);

	void screenShot();

	void setPanCamera(Model *model);
	void setPanCamera(DIYmodel *model);

	bool isInBoundingBoxGlobal(Camera *camera);
};