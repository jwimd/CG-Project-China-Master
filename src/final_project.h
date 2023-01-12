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

#include "diymodel.h"

struct Wall {
	Model* floor;
	Model* back;
	Model* left;
	Model* right;
};

struct Geometry {
	Model* cylinder;
	Model* cone;
	Model* prism;
	Model* cube;
	Model* prismaticTable4;
	Model* prismaticTable6;
	Model *knot;
	DIYmodel *gameObject;
};

struct Tex {
	std::unique_ptr<Texture2D> wood;
	std::unique_ptr<Texture2D> brick;
	std::unique_ptr<Texture2D> marbleBrown;
	std::unique_ptr<Texture2D> marblePurple;
	std::unique_ptr<Texture2D> metalBare;
	std::unique_ptr<Texture2D> metalPainted;
	std::unique_ptr<Texture2D> plastic;
	std::unique_ptr<Texture2D> table;
	std::unique_ptr<Texture2D> objTex[7];
};

class FinalProject : public Application {
public:
	FinalProject(const Options& options);
	
	~FinalProject();

	void handleInput() override;

	void renderFrame() override;

private:
	std::vector<Camera *> _cameras;

	int activeCameraIndex = 0;

	int panIndex = 0;

	int postureFrameNumber = 0;

	bool showCursor = true;
	bool firstPressControl = true;
	bool opencd = false;
	bool gameMode = false;

	std::vector<std::unique_ptr<Model>> _postures;

	struct Wall _wall;

	struct Geometry _geometry;

	std::unique_ptr<Model> _spotLightSphere;

	struct Tex _texture;

	std::unique_ptr<SkyBox> _skybox;
	std::unique_ptr<SkyBox> gameSkybox;

	GLSLProgram *_phongShader;
	GLSLProgram* _dancerShader;
	GLSLProgram* _sphereShader;

	// lights
	std::unique_ptr<DirectionalLight> _directionalLight;
	std::unique_ptr<SpotLight> _spotLight;

	// gameMode

	void initPhongShader();
	void initSphereShader();
	void initDancerShader();

	void screenShot();

	void setPanCamera(Model* model);

	bool isInBoundingBoxGlobal(Camera *camera);
};