#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "final_project.h"

const std::string spherePath = "../media/sphere.obj";
const std::string cubePath = "../media/shape/cube.obj";
const std::string cylinderPath = "../media/shape/cylinder.obj";
const std::string conePath = "../media/shape/cone.obj";
const std::string prismPath = "../media/shape/prism_6.obj";
const std::string prismaticTable6Path = "../media/shape/prismatic_table_6_0.5.obj";
const std::string prismaticTable4Path = "../media/shape/prismatic_table_4_0.3.obj";
const std::string knotPath = "../media/shape/knot.obj";

const std::string woodTexturePath = "../media/texture/wood.jpg";
const std::string brickTexturePath = "../media/texture/brick.jpg";
const std::string marbleBrownTexturePath = "../media/texture/marble_brown.jpg";
const std::string marblePurpleTexturePath = "../media/texture/marble_purple.jpg";
const std::string metalBareTexturePath = "../media/texture/metal_bare.jpg";
const std::string metalPaintedTexturePath = "../media/texture/metal_painted.jpg";
const std::string plasticTexturePath = "../media/texture/plastic.jpg";
const std::string tableTexturePath = "../media/resources/wood2.jpg";

const string objTexturePaths[] = {
	"../media/resources/purple.png",
	"../media/resources/purple_normal.png",
	"../media/resources/stone.jpg",
	"../media/resources/stone2.jpg",
	"../media/resources/wood1.jpg",
	"../media/resources/wood2.jpg"};

const string objSideTexturePaths[] = {
	"../media/resources/texture/side1.jpg",
	"../media/resources/texture/side2.jpg",
	"../media/resources/texture/top1.jpg",
	"../media/resources/texture/top2.jpg",
	"../media/resources/texture/fu.jpg",
	"../media/resources/texture/fu2.jpg"};

const std::vector<std::string>
	skyboxTexturePaths = {"../media/starfield/Right_Tex.jpg", "../media/starfield/Left_Tex.jpg", "../media/starfield/Up_Tex.jpg", "../media/starfield/Down_Tex.jpg", "../media/starfield/Front_Tex.jpg", "../media/starfield/Back_Tex.jpg"};

const std::vector<std::string> gameSkyboxTexturePaths = {
	"../media/resources/skybox/right.jpg",
	"../media/resources/skybox/left.jpg",
	"../media/resources/skybox/top.jpg",
	"../media/resources/skybox/bottom.jpg",
	"../media/resources/skybox/front.jpg",
	"../media/resources/skybox/back.jpg"};

FinalProject::FinalProject(const Options &options) : Application(options)
{
	_skybox.reset(new SkyBox(skyboxTexturePaths));
	gameSkybox.reset(new SkyBox(gameSkyboxTexturePaths));

	// init cameras
	_cameras.resize(2);

	const float aspect = 1.0f * _windowWidth / _windowHeight;
	constexpr float znear = 0.1f;
	constexpr float zfar = 10000.0f;

	// perspective camera
	_cameras[0] = new PerspectiveCamera(
		glm::radians(60.0f), aspect, 0.1f, 10000.0f);
	_cameras[0]->position = glm::vec3(0.0f, 0.0f, 15.0f);

	// orthographic camera
	_cameras[1] = new OrthographicCamera(
		-4.0f * aspect, 4.0f * aspect, -4.0f, 4.0f, znear, zfar);
	_cameras[1]->position = glm::vec3(0.0f, 0.0f, 15.0f);

	// init postures
	std::string posturePath;
	_postures.resize(101);
	for (int i = 0; i <= 100; i++)
	{
		posturePath = "../media/postures/pose0" + std::to_string(i) + ".obj";
		_postures[i].reset(new Model(posturePath));
		_postures[i]->position = glm::vec3{0.0f, 0.0f, -2.5f};
		_postures[i]->scale = glm::vec3{0.02f, 0.02f, 0.02f};
	}

	// init model
	_wall.floor = new Model(cubePath);
	_wall.floor->position = glm::vec3{0.0f, -0.25f, 0.0f};
	_wall.floor->scale = glm::vec3{5.0f, 0.25f, 5.0f};

	_wall.back = new Model(cubePath);
	_wall.back->position = glm::vec3{0.0f, 2.5f, -5.25f};
	_wall.back->scale = glm::vec3{5.5f, 3.0f, 0.25f};

	_wall.left = new Model(cubePath);
	_wall.left->position = glm::vec3{-5.25f, 2.5f, 0.0f};
	_wall.left->scale = glm::vec3{0.25f, 3.0f, 5.0f};

	_wall.right = new Model(cubePath);
	_wall.right->position = glm::vec3{5.25f, 2.5f, 0.0f};
	_wall.right->scale = glm::vec3{0.25f, 3.0f, 5.0f};

	_geometry.cylinder = new Model(cylinderPath);
	_geometry.cylinder->position = glm::vec3{-3.0f, 1.0f, -3.0f};

	_geometry.cone = new Model(conePath);
	_geometry.cone->position = glm::vec3{3.0f, 1.0f, -3.0f};

	_geometry.prism = new Model(prismPath);
	_geometry.prism->position = glm::vec3{-3.0f, 1.0f, 0.0f};

	_geometry.cube = new Model(cubePath);
	_geometry.cube->position = glm::vec3{3.0f, 1.0f, 0.0f};
	_geometry.cube->scale = glm::vec3{1.0f, 1.0f, 1.0f};

	_geometry.prismaticTable4 = new Model(prismaticTable4Path);
	_geometry.prismaticTable4->position = glm::vec3{-3.0f, 1.0f, 3.0f};

	_geometry.prismaticTable6 = new Model(prismaticTable6Path);
	_geometry.prismaticTable6->position = glm::vec3{3.0f, 1.0f, 3.0f};

	_geometry.gameObject = new DIYmodel();
	_geometry.gameObject->remake();
	_geometry.gameObject->position = glm::vec3{0.0f, 0.0f, 0.0f};
	_geometry.gameObject->scale = glm::vec3{0.2f, 0.2f, 0.2f};

	_geometry.knot = new Model(knotPath);
	_geometry.knot->position = glm::vec3{0.0f, 0.7f, 3.0f};
	_geometry.knot->scale = glm::vec3{0.2f, 0.2f, 0.2f};

	_spotLightSphere.reset(new Model(spherePath));
	_spotLightSphere->position = glm::vec3{0.0f, 10.0f, 10.0f};
	_spotLightSphere->scale = glm::vec3{0.5f, 0.5f, 0.5f};

	_texture.wood.reset(new Texture2D(woodTexturePath));
	_texture.brick.reset(new Texture2D(brickTexturePath));
	_texture.marbleBrown.reset(new Texture2D(marbleBrownTexturePath));
	_texture.marblePurple.reset(new Texture2D(marblePurpleTexturePath));
	_texture.metalBare.reset(new Texture2D(metalBareTexturePath));
	_texture.metalPainted.reset(new Texture2D(metalPaintedTexturePath));
	_texture.plastic.reset(new Texture2D(plasticTexturePath));
	_texture.table.reset(new Texture2D(tableTexturePath));

	for (int i = 0; i < 6; i++)
		_texture.objTex[i].reset(new Texture2D(objTexturePaths[i]));
	for (int i = 0; i < 6; i++)
		_texture.objSideTex[i].reset(new Texture2D(objSideTexturePaths[i]));

	for (int i = 0; i < 2; i++)
		flowerSystem.texture[i].reset(new Texture2D(flowerSystem.flowerpath[i]));

	glActiveTexture(GL_TEXTURE20);
	flowerSystem.texture[0]->bind();
	glActiveTexture(GL_TEXTURE21);
	flowerSystem.texture[1]->bind();

	// init shader
	initPhongShader();
	initSphereShader();
	initDancerShader();
	initFrameShader();
	initDepthShader();
	initTexturePhoneShader();

	initDepthPeelingShaders();
	initDepthPeelingResources();

	initAlphaBlendingShader();

	// init lights
	_directionalLight.reset(new DirectionalLight);
	_spotLight.reset(new SpotLight);

	// init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init();
}

FinalProject::~FinalProject()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void FinalProject::handleInput()
{
	if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(_window, true);
		return;
	}

	if (!gameMode)
	{
		if (_keyboardInput.keyStates[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS && panIndex == 0)
		{
			showCursor = false;
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (firstPressControl == true)
			{
				firstPressControl = false;
				_mouseInput.move.xOld = _mouseInput.move.xCurrent = 0.5 * _windowWidth;
				_mouseInput.move.yOld = _mouseInput.move.yCurrent = 0.5 * _windowHeight;
				glfwSetCursorPos(_window, _mouseInput.move.xCurrent, _mouseInput.move.yCurrent);
			}

			constexpr float cameraMoveSpeed = 0.02f;
			constexpr float cameraRotateSpeed = 0.015f;

			if (_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_PRESS)
			{
				// std::cout << "switch camera" << std::endl;
				//  switch camera
				activeCameraIndex = (activeCameraIndex + 1) % _cameras.size();
				_keyboardInput.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
				return;
			}

			Camera *camera = _cameras[activeCameraIndex];

			if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE)
			{
				// std::cout << "W" << std::endl;
				camera->position += 0.1f * camera->getFront();
				if (opencd == true && isInBoundingBoxGlobal(camera))
				{
					camera->position -= 0.1f * camera->getFront();
				}
			}

			if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE)
			{
				// std::cout << "A" << std::endl;
				camera->position -= 0.1f * camera->getRight();
				if (opencd == true && isInBoundingBoxGlobal(camera))
				{
					camera->position += 0.1f * camera->getRight();
				}
			}

			if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE)
			{
				// std::cout << "S" << std::endl;
				camera->position -= 0.1f * camera->getFront();
				if (opencd == true && isInBoundingBoxGlobal(camera))
				{
					camera->position += 0.1f * camera->getFront();
				}
			}

			if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE)
			{
				// std::cout << "D" << std::endl;
				camera->position += 0.1f * camera->getRight();
				if (opencd == true && isInBoundingBoxGlobal(camera))
				{
					camera->position -= 0.1f * camera->getRight();
				}
			}

			if (_keyboardInput.keyStates[GLFW_KEY_I] != GLFW_RELEASE)
			{
				// std::cout << "I" << std::endl;
				if (camera->fovy >= glm::radians(30.0f))
				{
					camera->fovy -= glm::radians(0.5f);
					camera->znear += 0.001;
					camera->zfar += 100.0;
				}
			}

			if (_keyboardInput.keyStates[GLFW_KEY_O] != GLFW_RELEASE)
			{
				// std::cout << "O" << std::endl;
				if (camera->fovy <= glm::radians(89.5f))
				{
					camera->fovy += glm::radians(0.5f);
					camera->znear -= 0.001;
					camera->zfar -= 100.0;
				}
			}

			if (_keyboardInput.keyStates[GLFW_KEY_F] != GLFW_RELEASE)
			{
				// std::cout << "F" << std::endl;
				camera->fovy = glm::radians(60.0f);
				camera->znear = 0.1;
				camera->zfar = 10000.0;
			}

			if (_mouseInput.move.xCurrent != _mouseInput.move.xOld)
			{
				// std::cout << "mouse move in x direction" << std::endl;
				float mouse_movement_in_x_direction = cameraRotateSpeed * (_mouseInput.move.xCurrent - _mouseInput.move.xOld);
				glm::quat q = glm::angleAxis(glm::radians(-mouse_movement_in_x_direction), glm::vec3(0.0f, 1.0f, 0.0f));
				camera->rotation = q * camera->rotation;
				_mouseInput.move.xOld = _mouseInput.move.xCurrent;
			}

			if (_mouseInput.move.yCurrent != _mouseInput.move.yOld)
			{
				// std::cout << "mouse move in y direction" << std::endl;
				float mouse_movement_in_y_direction = cameraRotateSpeed * (_mouseInput.move.yCurrent - _mouseInput.move.yOld);
				glm::quat q = glm::angleAxis(glm::radians(-mouse_movement_in_y_direction), glm::vec3(camera->getRight().x, camera->getRight().y, camera->getRight().z));
				camera->rotation = q * camera->rotation;
				_mouseInput.move.yOld = _mouseInput.move.yCurrent;
			}
		}
		else
		{
			if (firstPressControl == false)
			{
				firstPressControl = true;
			}
			showCursor = true;
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		sunOrder += 1 * sunOrderSign;
		_spotLightSphere->position = glm::vec3(0.005f * sunOrder, 10.0f, 7.0f);
		if (sunOrder == 2000)
			sunOrderSign = -1;
		if (sunOrder == -2000)
			sunOrderSign = 1;
	}
	else
	{
		int point, circle;
		bool lor;
		bool lorSelection;
		if (_mouseInput.click.left)
		{
			if (_phongMode == PhongMode::ModelEdit)
			{
				point = _geometry.gameObject->get_point(_mouseInput.move.xOld, _mouseInput.move.yOld, _cameras[activeCameraIndex]);
				if (point >= 0)
					PointSelect = true;
				else
					PointSelect = false;
			}
			else
			{
				if (circle >= 0)
				{
					PointSelect = true;
					lorSelection = lor;
				}
				else
				{
					PointSelect = false;
				}
			}
		}
		else if (_mouseInput.click.middle)
		{
			if (_phongMode == PhongMode::ModelEdit)
			{
				point = _geometry.gameObject->get_line_start_point(_mouseInput.move.xOld, _mouseInput.move.yOld, _cameras[activeCameraIndex]);
				if (point >= 0)
				{
					_geometry.gameObject->split_point(point);
					notChange = false;
				}
			}
		}
		else if (_mouseInput.click.right)
		{
			if (_phongMode == PhongMode::ModelEdit)
			{
				point = _geometry.gameObject->get_point(_mouseInput.move.xOld, _mouseInput.move.yOld, _cameras[activeCameraIndex]);
				if (point >= 0)
				{
					_geometry.gameObject->remove_point(point);
					notChange = false;
				}
			}
		}
		else
			PointSelect = false;

		float xoffset = _mouseInput.move.xCurrent - _mouseInput.move.xOld;
		float yoffset = _mouseInput.move.yOld - _mouseInput.move.yCurrent;
		// reversed since y-coordinates go from bottom to top

		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
		_mouseInput.move.yOld = _mouseInput.move.yCurrent;

		if (PointSelect)
		{
			if (_phongMode == PhongMode::ModelEdit)
				_geometry.gameObject->modify_point(xoffset, yoffset, _cameras[activeCameraIndex]);
			notChange = false;
			_geometry.gameObject->remake();
		}

		if (_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_PRESS)
		{
			// std::cout << "switch camera" << std::endl;
			//  switch camera
			activeCameraIndex = (activeCameraIndex + 1) % _cameras.size();
			_keyboardInput.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
			return;
		}

		Camera *camera = _cameras[activeCameraIndex];

		if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE)
		{
			// std::cout << "W" << std::endl;
			camera->position += 0.1f * camera->getFront();
			if (opencd == true && isInBoundingBoxGlobal(camera))
			{
				camera->position -= 0.1f * camera->getFront();
			}
		}

		if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE)
		{
			// std::cout << "A" << std::endl;
			camera->position -= 0.1f * camera->getRight();
			if (opencd == true && isInBoundingBoxGlobal(camera))
			{
				camera->position += 0.1f * camera->getRight();
			}
		}

		if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE)
		{
			// std::cout << "S" << std::endl;
			camera->position -= 0.1f * camera->getFront();
			if (opencd == true && isInBoundingBoxGlobal(camera))
			{
				camera->position += 0.1f * camera->getFront();
			}
		}

		if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE)
		{
			// std::cout << "D" << std::endl;
			camera->position += 0.1f * camera->getRight();
			if (opencd == true && isInBoundingBoxGlobal(camera))
			{
				camera->position -= 0.1f * camera->getRight();
			}
		}

		if (_keyboardInput.keyStates[GLFW_KEY_I] != GLFW_RELEASE)
		{
			// std::cout << "I" << std::endl;
			if (camera->fovy >= glm::radians(30.0f))
			{
				camera->fovy -= glm::radians(0.5f);
				camera->znear += 0.001;
				camera->zfar += 100.0;
			}
		}

		if (_keyboardInput.keyStates[GLFW_KEY_O] != GLFW_RELEASE)
		{
			// std::cout << "O" << std::endl;
			if (camera->fovy <= glm::radians(89.5f))
			{
				camera->fovy += glm::radians(0.5f);
				camera->znear -= 0.001;
				camera->zfar -= 100.0;
			}
		}

		if (_keyboardInput.keyStates[GLFW_KEY_F] != GLFW_RELEASE)
		{
			// std::cout << "F" << std::endl;
			camera->fovy = glm::radians(60.0f);
			camera->znear = 0.1;
			camera->zfar = 10000.0;
		}

		_spotLightSphere->position = glm::vec3(5.5f, 10.0f, 7.0f);
	}
}

void FinalProject::renderFrame()
{
	_geometry.cylinder->computeRotationQuat();
	_geometry.cone->computeRotationQuat();
	_geometry.prism->computeRotationQuat();
	_geometry.cube->computeRotationQuat();
	_geometry.prismaticTable4->computeRotationQuat();
	_geometry.prismaticTable6->computeRotationQuat();
	_geometry.gameObject->computeRotationQuat();
	_geometry.knot->computeRotationQuat();

	// some options related to imGUI
	static bool wireframe = false;

	showFpsInWindowTitle();

	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glm::mat4 projection = _cameras[activeCameraIndex]->getProjectionMatrix();
	glm::mat4 view = _cameras[activeCameraIndex]->getViewMatrix();

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 20.0f;
	lightProjection = glm::ortho(-18.0f, 18.0f, -18.0f, 18.0f, near_plane, far_plane);
	lightView = glm::lookAt(_spotLight->position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	_sphereShader->use();
	_sphereShader->setMat4("projection", projection);
	_sphereShader->setMat4("view", view);
	_sphereShader->setMat4("model", _spotLightSphere->getModelMatrix());

	_phongTextureShader->use();
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	_phongTextureShader->setInt("shadowMap", 10);

	_phongTextureShader->setVec3("directionalLight.direction", _directionalLight->direction);
	_phongTextureShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
	_phongTextureShader->setVec3("directionalLight.color", _directionalLight->color);
	_phongTextureShader->setMat4("projection", projection);
	_phongTextureShader->setMat4("view", view);
	_phongTextureShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	_spotLight->position = _spotLightSphere->position;
	_phongTextureShader->setVec3("spotLight.position", _spotLight->position);
	_phongTextureShader->setVec3("spotLight.direction", _spotLight->direction);
	_phongTextureShader->setFloat("spotLight.intensity", _spotLight->intensity);
	_phongTextureShader->setVec3("spotLight.color", _spotLight->color);
	_phongTextureShader->setFloat("spotLight.angle", _spotLight->angle);
	_phongTextureShader->setFloat("spotLight.kc", _spotLight->kc);
	_phongTextureShader->setFloat("spotLight.kl", _spotLight->kl);
	_phongTextureShader->setFloat("spotLight.kq", _spotLight->kq);

	_phongTextureShader->setVec3("viewPosition", _cameras[activeCameraIndex]->position);

	glActiveTexture(GL_TEXTURE1);
	_texture.wood->bind();
	glActiveTexture(GL_TEXTURE2);
	_texture.brick->bind();
	glActiveTexture(GL_TEXTURE3);
	_texture.marbleBrown->bind();
	glActiveTexture(GL_TEXTURE4);
	_texture.marblePurple->bind();
	glActiveTexture(GL_TEXTURE5);
	_texture.metalBare->bind();
	glActiveTexture(GL_TEXTURE6);
	_texture.metalPainted->bind();
	glActiveTexture(GL_TEXTURE7);
	_texture.plastic->bind();
	glActiveTexture(GL_TEXTURE8);
	_texture.objTex[activeBaceTexture]->bind();
	for (int i = 0; i < 6; i++)
	{
		glActiveTexture(GL_TEXTURE11 + i);
		_texture.objSideTex[i]->bind();
	}

	if (!gameMode)
	{
		switch (panIndex)
		{
		case 1:
			setPanCamera(_geometry.cylinder);
			break;
		case 2:
			setPanCamera(_geometry.cone);
			break;
		case 3:
			setPanCamera(_geometry.prism);
			break;
		case 4:
			setPanCamera(_geometry.cube);
			break;
		case 5:
			setPanCamera(_geometry.prismaticTable4);
			break;
		case 6:
			setPanCamera(_geometry.prismaticTable6);
			break;
		case 7:
			setPanCamera(_geometry.gameObject);
			break;
		case 8:
			setPanCamera(_postures[postureFrameNumber / 5].get());
			break;
		case 9:
			setPanCamera(_geometry.knot);
		default:
			break;
		}

		_dancerShader->use();
		_dancerShader->setMat4("projection", projection);
		_dancerShader->setMat4("view", view);

		_dancerShader->setVec3("directionalLight.direction", _directionalLight->direction);
		_dancerShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
		_dancerShader->setVec3("directionalLight.color", _directionalLight->color);

		_spotLight->position = _spotLightSphere->position;
		_dancerShader->setVec3("spotLight.position", _spotLight->position);
		_dancerShader->setVec3("spotLight.direction", _spotLight->direction);
		_dancerShader->setFloat("spotLight.intensity", _spotLight->intensity);
		_dancerShader->setVec3("spotLight.color", _spotLight->color);
		_dancerShader->setFloat("spotLight.angle", _spotLight->angle);
		_dancerShader->setFloat("spotLight.kc", _spotLight->kc);
		_dancerShader->setFloat("spotLight.kl", _spotLight->kl);
		_dancerShader->setFloat("spotLight.kq", _spotLight->kq);

		_dancerShader->setVec3("viewPosition", _cameras[activeCameraIndex]->position);

		_dancerShader->setMat4("model", _postures[postureFrameNumber / 5]->getModelMatrix());
		_dancerShader->setVec3("material.ka", _postures[postureFrameNumber / 5]->_phongMaterial->ka);
		_dancerShader->setVec3("material.kd", _postures[postureFrameNumber / 5]->_phongMaterial->kd);
		_dancerShader->setVec3("material.ks", _postures[postureFrameNumber / 5]->_phongMaterial->ks);
		_dancerShader->setFloat("material.ns", _postures[postureFrameNumber / 5]->_phongMaterial->ns);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glCullFace(GL_FRONT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		_depthShader->use();
		_depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		_depthShader->setMat4("model", _postures[postureFrameNumber / 5]->getModelMatrix());
		_postures[postureFrameNumber / 5]->draw();

		_depthShader->setMat4("model", _wall.floor->getModelMatrix());
		_wall.floor->draw();

		_depthShader->setMat4("model", _wall.back->getModelMatrix());
		_wall.back->draw();

		_depthShader->setMat4("model", _wall.left->getModelMatrix());
		_wall.left->draw();

		_depthShader->setMat4("model", _wall.right->getModelMatrix());
		_wall.right->draw();

		_depthShader->setMat4("model", _geometry.prism->getModelMatrix());
		_geometry.prism->draw();

		_depthShader->setMat4("model", _geometry.cone->getModelMatrix());
		_geometry.cone->draw();

		_depthShader->setMat4("model", _geometry.cylinder->getModelMatrix());
		_geometry.cylinder->draw();

		_depthShader->setMat4("model", _geometry.prismaticTable4->getModelMatrix());
		_geometry.prismaticTable4->draw();

		_depthShader->setMat4("model", _geometry.cube->getModelMatrix());
		_geometry.cube->draw();

		_depthShader->setMat4("model", _geometry.prismaticTable6->getModelMatrix());
		_geometry.prismaticTable4->draw();

		if (_renderMode == RenderMode::PhongTexture)
		{
			_depthShader->setMat4("model", _geometry.gameObject->getModelMatrix());
			_geometry.gameObject->draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);

		// reset viewport
		glViewport(0, 0, _windowWidth, _windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_sphereShader->use();
		_spotLightSphere->draw();

		_dancerShader->use();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		_dancerShader->setInt("shadowMap", 10);
		_postures[(postureFrameNumber += (1 * playOrder)) / 5]->draw();
		if (postureFrameNumber == 500)
			playOrder = -1;
		if (postureFrameNumber == 0)
			playOrder = 1;

		_phongShader->use();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		_phongShader->setInt("shadowMap", 10);

		_phongShader->setVec3("directionalLight.direction", _directionalLight->direction);
		_phongShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
		_phongShader->setVec3("directionalLight.color", _directionalLight->color);
		_phongShader->setMat4("projection", projection);
		_phongShader->setMat4("view", view);
		_phongShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		_spotLight->position = _spotLightSphere->position;
		_phongShader->setVec3("spotLight.position", _spotLight->position);
		_phongShader->setVec3("spotLight.direction", _spotLight->direction);
		_phongShader->setFloat("spotLight.intensity", _spotLight->intensity);
		_phongShader->setVec3("spotLight.color", _spotLight->color);
		_phongShader->setFloat("spotLight.angle", _spotLight->angle);
		_phongShader->setFloat("spotLight.kc", _spotLight->kc);
		_phongShader->setFloat("spotLight.kl", _spotLight->kl);
		_phongShader->setFloat("spotLight.kq", _spotLight->kq);

		_phongShader->setVec3("viewPosition", _cameras[activeCameraIndex]->position);

		_phongShader->use();

		_phongShader->setInt("mapKd", 1);
		_wall.floor->setPhongShader(_phongShader);
		_wall.floor->draw();

		_wall.back->setPhongShader(_phongShader);
		_wall.back->draw();

		_wall.left->setPhongShader(_phongShader);
		_wall.left->draw();

		_wall.right->setPhongShader(_phongShader);
		_wall.right->draw();

		_phongShader->setInt("mapKd", 2);
		_geometry.prism->setPhongShader(_phongShader);
		_geometry.prism->draw();

		_phongShader->setInt("mapKd", 3);
		_geometry.cone->setPhongShader(_phongShader);
		_geometry.cone->draw();

		_phongShader->setInt("mapKd", 4);
		_geometry.cylinder->setPhongShader(_phongShader);
		_geometry.cylinder->draw();

		_phongShader->setInt("mapKd", 5);
		_geometry.prismaticTable4->setPhongShader(_phongShader);
		_geometry.prismaticTable4->draw();

		_phongShader->setInt("mapKd", 6);
		_geometry.cube->setPhongShader(_phongShader);
		_geometry.cube->draw();

		_phongShader->setInt("mapKd", 7);
		_geometry.prismaticTable6->setPhongShader(_phongShader);
		_geometry.prismaticTable6->draw();

		if (_renderMode == RenderMode::PhongTexture)
		{
			_phongTextureShader->use();

			_phongTextureShader->setInt("mapKd", 8);
			int texnum = _geometry.gameObject->textures.size();

			_phongTextureShader->setMat4("model", _geometry.gameObject->getModelMatrix());

			_phongTextureShader->setInt("texnum", texnum);
			for (int i = 0; i < texnum; i++)
			{
				_phongTextureShader->setFloat("trange_l[" + to_string(i) + "]", _geometry.gameObject->textures[i].l);
				_phongTextureShader->setFloat("trange_r[" + to_string(i) + "]", _geometry.gameObject->textures[i].r);
				_phongTextureShader->setInt("texs[" + to_string(i) + "]", _geometry.gameObject->textures[i].map);
				_phongTextureShader->setInt("repeat[" + to_string(i) + "]", _geometry.gameObject->textures[i].repeat);
				_phongTextureShader->setInt("reverse[" + to_string(i) + "]", _geometry.gameObject->textures[i].reverse);
				_phongTextureShader->setInt("type[" + to_string(i) + "]", _geometry.gameObject->textures[i].type);
			}
			_geometry.gameObject->setPhongShader(_phongTextureShader);
			_geometry.gameObject->draw();
		}

		_skybox->draw(projection, view);

		if (_renderMode == RenderMode::AlphaBlending)
			renderWithAlphaBlending(_geometry.gameObject);
		renderWithAlphaBlending(_geometry.knot);
	}

	if (gameMode)
	{

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glCullFace(GL_FRONT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		_depthShader->use();
		_depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		_depthShader->setMat4("model", _wall.floor->getModelMatrix());
		_wall.floor->draw();

		if (_renderMode == RenderMode::PhongTexture)
		{
			_depthShader->setMat4("model", _geometry.gameObject->getModelMatrix());
			_geometry.gameObject->draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);

		// reset viewport
		glViewport(0, 0, _windowWidth, _windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (flowerShowen)
		{
			static int i = 0;
			i = (i + 1) % 5;
			if (i == 0)
				flowerSystem.update(_deltaTime);
			flowerSystem.draw(_cameras[activeCameraIndex]);
		}

		_sphereShader->use();
		_spotLightSphere->draw();

		glActiveTexture(GL_TEXTURE1);
		_texture.table->bind();
		glActiveTexture(GL_TEXTURE2);
		_texture.objTex[activeBaceTexture]->bind();

		_phongShader->use();

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		_phongShader->setInt("shadowMap", 10);

		_phongShader->setVec3("directionalLight.direction", _directionalLight->direction);
		_phongShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
		_phongShader->setVec3("directionalLight.color", _directionalLight->color);
		_phongShader->setMat4("projection", projection);
		_phongShader->setMat4("view", view);
		_phongShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		_spotLight->position = _spotLightSphere->position;
		_phongShader->setVec3("spotLight.position", _spotLight->position);
		_phongShader->setVec3("spotLight.direction", _spotLight->direction);
		_phongShader->setFloat("spotLight.intensity", _spotLight->intensity);
		_phongShader->setVec3("spotLight.color", _spotLight->color);
		_phongShader->setFloat("spotLight.angle", _spotLight->angle);
		_phongShader->setFloat("spotLight.kc", _spotLight->kc);
		_phongShader->setFloat("spotLight.kl", _spotLight->kl);
		_phongShader->setFloat("spotLight.kq", _spotLight->kq);

		_phongShader->setVec3("viewPosition", _cameras[activeCameraIndex]->position);

		_phongShader->setInt("mapKd", 1);
		_wall.floor->setPhongShader(_phongShader);
		_wall.floor->draw();

		if (_renderMode == RenderMode::PhongTexture)
		{
			_phongTextureShader->use();

			int texnum = _geometry.gameObject->textures.size();

			_phongTextureShader->setInt("texnum", texnum);
			for (int i = 0; i < texnum; i++)
			{
				_phongTextureShader->setFloat("trange_l[" + to_string(i) + "]", _geometry.gameObject->textures[i].l);
				_phongTextureShader->setFloat("trange_r[" + to_string(i) + "]", _geometry.gameObject->textures[i].r);
				_phongTextureShader->setInt("texs[" + to_string(i) + "]", _geometry.gameObject->textures[i].map);
				_phongTextureShader->setInt("repeat[" + to_string(i) + "]", _geometry.gameObject->textures[i].repeat);
				_phongTextureShader->setInt("reverse[" + to_string(i) + "]", _geometry.gameObject->textures[i].reverse);
				_phongTextureShader->setInt("type[" + to_string(i) + "]", _geometry.gameObject->textures[i].type);
			}
			_geometry.gameObject->setPhongShader(_phongTextureShader);

			_geometry.gameObject->draw();
		}

		gameSkybox->draw(projection, view);

		if (_renderMode == RenderMode::AlphaBlending)
			renderWithAlphaBlending(_geometry.gameObject);

		_frameShader->use();
		_frameShader->setMat4("projection", projection);
		_frameShader->setMat4("view", view);
		_frameShader->setMat4("model", _geometry.gameObject->getModelMatrix());
		if (_phongMode == PhongMode::ModelEdit)
			_geometry.gameObject->drawFrame(_frameShader);
	}

	// draw ui elements
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const auto flags =
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_AlwaysVerticalScrollbar;

	if (!ImGui::Begin("Control Panel", nullptr, flags))
	{
		ImGui::End();
	}
	else if (!gameMode)
	{
		if (ImGui::TreeNode("directional light"))
		{
			ImGui::SliderFloat("x direction##1", (float *)&_directionalLight->direction.x, -1.0f, 1.0f);
			ImGui::SliderFloat("z direction##1", (float *)&_directionalLight->direction.z, -1.0f, 1.0f);
			ImGui::SliderFloat("intensity##1", (float *)&_directionalLight->intensity, 0.0f, 1.0f);
			ImGui::ColorEdit3("color##1", (float *)&_directionalLight->color);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("spot light"))
		{
			ImGui::SliderFloat("x position##2", (float *)&_spotLightSphere->position.x, -5.0f, 5.0f);
			ImGui::SliderFloat("y position##2", (float *)&_spotLightSphere->position.y, 5.0f, 15.0f);
			ImGui::SliderFloat("z position##2", (float *)&_spotLightSphere->position.z, 5.0f, 15.0f);
			ImGui::SliderFloat("x direction##2", (float *)&_spotLight->direction.x, -1.0f, 1.0f);
			ImGui::SliderFloat("z direction##2", (float *)&_spotLight->direction.z, -1.0f, 1.0f);
			ImGui::SliderFloat("intensity##2", (float *)&_spotLight->intensity, 0.0f, 2.0f);
			ImGui::SliderFloat("angle##2", (float *)&_spotLight->angle, glm::radians(0.0f), glm::radians(120.0f));
			ImGui::ColorEdit3("color##2", (float *)&_spotLight->color);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("cylinder"))
		{
			ImGui::SliderFloat3("position##1", (float *)&_geometry.cylinder->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##1", (float *)&_geometry.cylinder->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##1", (float *)&_geometry.cylinder->scale, 0.5f, 2.0f);
			ImGui::ColorEdit3("ka##1", (float *)&_geometry.cylinder->_phongMaterial->ka);
			ImGui::ColorEdit3("kd##1", (float *)&_geometry.cylinder->_phongMaterial->kd);
			ImGui::ColorEdit3("ks##1", (float *)&_geometry.cylinder->_phongMaterial->ks);
			ImGui::SliderFloat("ns##1", (float *)&_geometry.cylinder->_phongMaterial->ns, 1.0f, 20.0f);
			if (ImGui::Button("save as .obj file##1") == true)
				_geometry.cylinder->saveAsObjFile("cylinder");
			if (ImGui::Button("pan to it##1") == true)
				panIndex = ((panIndex == 0) ? 1 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("cone"))
		{
			ImGui::SliderFloat3("position##2", (float *)&_geometry.cone->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##2", (float *)&_geometry.cone->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##2", (float *)&_geometry.cone->scale, 0.5f, 2.0f);
			ImGui::ColorEdit3("ka##2", (float *)&_geometry.cone->_phongMaterial->ka);
			ImGui::ColorEdit3("kd##2", (float *)&_geometry.cone->_phongMaterial->kd);
			ImGui::ColorEdit3("ks##2", (float *)&_geometry.cone->_phongMaterial->ks);
			ImGui::SliderFloat("ns##2", (float *)&_geometry.cone->_phongMaterial->ns, 1.0f, 20.0f);
			if (ImGui::Button("save as .obj file##2") == true)
				_geometry.cone->saveAsObjFile("cone");
			if (ImGui::Button("pan to it##2") == true)
				panIndex = ((panIndex == 0) ? 2 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("prism"))
		{
			ImGui::SliderFloat3("position##3", (float *)&_geometry.prism->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##3", (float *)&_geometry.prism->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##3", (float *)&_geometry.prism->scale, 0.5f, 2.0f);
			ImGui::ColorEdit3("ka##3", (float *)&_geometry.prism->_phongMaterial->ka);
			ImGui::ColorEdit3("kd##3", (float *)&_geometry.prism->_phongMaterial->kd);
			ImGui::ColorEdit3("ks##3", (float *)&_geometry.prism->_phongMaterial->ks);
			ImGui::SliderFloat("ns##3", (float *)&_geometry.prism->_phongMaterial->ns, 1.0f, 20.0f);
			if (ImGui::Button("save as .obj file##3") == true)
				_geometry.prism->saveAsObjFile("prism");
			if (ImGui::Button("pan to it##3") == true)
				panIndex = ((panIndex == 0) ? 3 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("cube"))
		{
			ImGui::SliderFloat3("position##4", (float *)&_geometry.cube->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##4", (float *)&_geometry.cube->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##4", (float *)&_geometry.cube->scale, 0.5f, 2.0f);
			ImGui::ColorEdit3("ka##4", (float *)&_geometry.cube->_phongMaterial->ka);
			ImGui::ColorEdit3("kd##4", (float *)&_geometry.cube->_phongMaterial->kd);
			ImGui::ColorEdit3("ks##4", (float *)&_geometry.cube->_phongMaterial->ks);
			ImGui::SliderFloat("ns##4", (float *)&_geometry.cube->_phongMaterial->ns, 1.0f, 20.0f);
			if (ImGui::Button("save as .obj file##4") == true)
				_geometry.cube->saveAsObjFile("cube");
			if (ImGui::Button("pan to it##4") == true)
				panIndex = ((panIndex == 0) ? 4 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("prismatic table (4 side faces)"))
		{
			ImGui::SliderFloat3("position##5", (float *)&_geometry.prismaticTable4->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##5", (float *)&_geometry.prismaticTable4->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##5", (float *)&_geometry.prismaticTable4->scale, 0.5f, 2.0f);
			ImGui::ColorEdit3("ka##5", (float *)&_geometry.prismaticTable4->_phongMaterial->ka);
			ImGui::ColorEdit3("kd##5", (float *)&_geometry.prismaticTable4->_phongMaterial->kd);
			ImGui::ColorEdit3("ks##5", (float *)&_geometry.prismaticTable4->_phongMaterial->ks);
			ImGui::SliderFloat("ns##5", (float *)&_geometry.prismaticTable4->_phongMaterial->ns, 1.0f, 20.0f);
			if (ImGui::Button("save as .obj file##5") == true)
				_geometry.prismaticTable4->saveAsObjFile("prismaticTable4");
			if (ImGui::Button("pan to it##5") == true)
				panIndex = ((panIndex == 0) ? 5 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("prismatic table (6 side faces)"))
		{
			ImGui::SliderFloat3("position##6", (float *)&_geometry.prismaticTable6->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##6", (float *)&_geometry.prismaticTable6->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##6", (float *)&_geometry.prismaticTable6->scale, 0.5f, 2.0f);
			ImGui::ColorEdit3("ka##6", (float *)&_geometry.prismaticTable6->_phongMaterial->ka);
			ImGui::ColorEdit3("kd##6", (float *)&_geometry.prismaticTable6->_phongMaterial->kd);
			ImGui::ColorEdit3("ks##6", (float *)&_geometry.prismaticTable6->_phongMaterial->ks);
			ImGui::SliderFloat("ns##6", (float *)&_geometry.prismaticTable6->_phongMaterial->ns, 1.0f, 20.0f);
			if (ImGui::Button("save as .obj file##6") == true)
				_geometry.prismaticTable6->saveAsObjFile("prismaticTable6");
			if (ImGui::Button("pan to it##6") == true)
				panIndex = ((panIndex == 0) ? 6 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("dancer"))
		{
			if (ImGui::Button("pan to it##8") == true)
				panIndex = ((panIndex == 0) ? 8 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("knot"))
		{
			ImGui::SliderFloat3("position##9", (float *)&_geometry.knot->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##9", (float *)&_geometry.knot->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##9", (float *)&_geometry.knot->scale, 0.1f, 2.0f);
			ImGui::SliderFloat3("albedo##9", (float *)&_knotMaterial->albedo, 0.0f, 1.0f);
			ImGui::SliderFloat("ka##9", (float *)&_knotMaterial->ka, 0.0f, 1.0f);
			ImGui::ColorEdit3("kd##9", (float *)&_knotMaterial->kd);
			ImGui::SliderFloat("transparent##9", &_knotMaterial->transparent, 0.0f, 1.0f);
			if (ImGui::Button("save as .obj file##9") == true)
				_geometry.knot->saveAsObjFile("knot");
			if (ImGui::Button("pan to it##9") == true)
				panIndex = ((panIndex == 0) ? 9 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("diy model"))
		{
			ImGui::RadioButton("Phong Texture", (int *)&_renderMode, (int)(RenderMode::PhongTexture));
			ImGui::RadioButton("Alpha Blending", (int *)&_renderMode, (int)(RenderMode::AlphaBlending));
			ImGui::SliderFloat3("position##7", (float *)&_geometry.gameObject->position, -4.0f, 4.0f);
			ImGui::SliderFloat3("rotation##7", (float *)&_geometry.gameObject->rotateangle, -4.0f, 4.0f);
			ImGui::SliderFloat3("scale##7", (float *)&_geometry.gameObject->scale, 0.1f, 2.0f);
			if (_renderMode == RenderMode::PhongTexture)
			{
				ImGui::ColorEdit3("ka##7", (float *)&_geometry.gameObject->_phongMaterial->ka);
				ImGui::ColorEdit3("kd##7", (float *)&_geometry.gameObject->_phongMaterial->kd);
				ImGui::ColorEdit3("ks##7", (float *)&_geometry.gameObject->_phongMaterial->ks);
				ImGui::SliderFloat("ns##7", (float *)&_geometry.gameObject->_phongMaterial->ns, 1.0f, 20.0f);
			}
			else
			{
				ImGui::SliderFloat3("albedo##9", (float *)&_diyMaterial->albedo, 0.0f, 1.0f);
				ImGui::SliderFloat("ka##9", (float *)&_diyMaterial->ka, 0.0f, 1.0f);
				ImGui::ColorEdit3("kd##9", (float *)&_diyMaterial->kd);
				ImGui::SliderFloat("transparent##9", &_diyMaterial->transparent, 0.0f, 1.0f);
			}
			if (ImGui::Button("save to file") == true)
				_geometry.gameObject->save_file("../output/DIYmodel.txt");
			if (ImGui::Button("load from file") == true)
			{
				_geometry.gameObject->load_from_file("../output/DIYmodel.txt");
				_geometry.gameObject->remake();
			}
			if (ImGui::Button("pan to it##7") == true)
				panIndex = ((panIndex == 0) ? 7 : 0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		ImGui::Checkbox("open collision detection", &opencd);
		if (ImGui::Button("screenshot##1") == true)
			screenShot();
		if (ImGui::Button("start to make your own china##2") == true)
		{
			_geometry.gameObject->position = glm::vec3{0.0f, 0.0f, 0.0f};
			_geometry.gameObject->scale = glm::vec3{0.5f, 0.5f, 0.5f};
			_geometry.gameObject->rotation = {1.0f, 0.0f, 0.0f, 0.0f};
			_geometry.gameObject->rotateangle = glm::vec3{0.0f, 0.0f, 0.0f};
			gameMode = true;
			setPanCamera(_geometry.gameObject);
		}
		ImGui::End();
	}
	else if (gameMode)
	{

		ImGui::RadioButton("Phong Texture", (int *)&_renderMode, (int)(RenderMode::PhongTexture));
		ImGui::RadioButton("Alpha Blending", (int *)&_renderMode, (int)(RenderMode::AlphaBlending));
		if (_renderMode == RenderMode::PhongTexture)
		{
			ImGui::RadioButton("Model Edit", (int *)&_phongMode, (int)(PhongMode::ModelEdit));
			ImGui::RadioButton("Texture Edit", (int *)&_phongMode, (int)(PhongMode::TextureEdit));

			ImGui::ColorEdit3("ka##7", (float *)&_geometry.gameObject->_phongMaterial->ka);
			ImGui::ColorEdit3("kd##7", (float *)&_geometry.gameObject->_phongMaterial->kd);
			ImGui::ColorEdit3("ks##7", (float *)&_geometry.gameObject->_phongMaterial->ks);
			ImGui::SliderFloat("ns##7", (float *)&_geometry.gameObject->_phongMaterial->ns, 1.0f, 20.0f);

			if (_phongMode == PhongMode::TextureEdit)
			{

				if (ImGui::TreeNode("base matrial"))
				{
					ImGui::RadioButton("purple1", &activeBaceTexture, 0);
					ImGui::RadioButton("purple2", &activeBaceTexture, 1);
					ImGui::RadioButton("stone1", &activeBaceTexture, 2);
					ImGui::RadioButton("stone2", &activeBaceTexture, 3);
					ImGui::RadioButton("wood1", &activeBaceTexture, 4);
					ImGui::RadioButton("wood2", &activeBaceTexture, 5);
					ImGui::TreePop();
					ImGui::Separator();
				}
				if (ImGui::TreeNode("add texture"))
				{
					ImGui::RadioButton("side1", &addTextureIndex, 0);
					ImGui::RadioButton("side2", &addTextureIndex, 1);
					ImGui::RadioButton("top1", &addTextureIndex, 2);
					ImGui::RadioButton("top2", &addTextureIndex, 3);
					ImGui::RadioButton("fu1", &addTextureIndex, 4);
					ImGui::RadioButton("fu2", &addTextureIndex, 5);
					if (_geometry.gameObject->textures.size() < 10)
						if (ImGui::Button("add") == true)
							_geometry.gameObject->add_texture("texture " + to_string(_geometry.gameObject->textures.size()), 11 + addTextureIndex);
					ImGui::TreePop();
					ImGui::Separator();
				}

				for (int i = 0; i < _geometry.gameObject->textures.size(); i++)
				{

					if (ImGui::TreeNode(_geometry.gameObject->textures[i].name.c_str()))
					{
						// if (ImGui::Button(("active##" + to_string(i)).c_str()) == true)
						// 	_geometry.gameObject->active_texture(i);
						ImGui::SliderInt(("repeat##" + to_string(i)).c_str(), (int *)&_geometry.gameObject->textures[i].repeat, 1, 15);
						ImGui::Checkbox(("is top##" + to_string(i)).c_str(), &_geometry.gameObject->textures[i].isTop);
						ImGui::Checkbox(("is reverse##" + to_string(i)).c_str(), &_geometry.gameObject->textures[i].isRev);

						if (_geometry.gameObject->textures[i].isTop)
							_geometry.gameObject->textures[i].type = 1;
						else
							_geometry.gameObject->textures[i].type = 0;

						if (_geometry.gameObject->textures[i].isRev)
							_geometry.gameObject->textures[i].reverse = 1;
						else
							_geometry.gameObject->textures[i].reverse = 0;

						ImGui::SliderFloat(("position##" + to_string(i)).c_str(), (float *)&_geometry.gameObject->textures[i].pos, -1.0f, 1.0f);
						ImGui::SliderFloat(("width##" + to_string(i)).c_str(), (float *)&_geometry.gameObject->textures[i].width, 0.0f, 2.0f);

						_geometry.gameObject->textures[i].l = std::max(-1.0f, _geometry.gameObject->textures[i].pos - _geometry.gameObject->textures[i].width / 2);
						_geometry.gameObject->textures[i].r = std::min(1.0f, _geometry.gameObject->textures[i].pos + _geometry.gameObject->textures[i].width / 2);

						if (ImGui::Button(("remove##" + to_string(i)).c_str()) == true)
							_geometry.gameObject->remove_texture(i);
						ImGui::TreePop();
						ImGui::Separator();
					}
				}
			}
		}
		else
		{
			ImGui::SliderFloat3("albedo##9", (float *)&_diyMaterial->albedo, 0.0f, 1.0f);
			ImGui::SliderFloat("ka##9", (float *)&_diyMaterial->ka, 0.0f, 1.0f);
			ImGui::ColorEdit3("kd##9", (float *)&_diyMaterial->kd);
			ImGui::SliderFloat("transparent##9", &_diyMaterial->transparent, 0.0f, 1.0f);
		}
		ImGui::Checkbox("open collision detection", &opencd);
		ImGui::Checkbox("open particle system", &flowerShowen);
		ImGui::RadioButton("flower", &flowerSystem.type, 0);
		ImGui::RadioButton("maple", &flowerSystem.type, 1);

		if (ImGui::Button("save to file") == true)
			_geometry.gameObject->save_file("../output/DIYmodel.txt");
		if (ImGui::Button("load from file") == true)
		{
			_geometry.gameObject->load_from_file("../output/DIYmodel.txt");
			_geometry.gameObject->remake();
		}
		if (ImGui::Button("front view##2") == true)
			setPanCamera(_geometry.gameObject);
		if (ImGui::Button("top view##2") == true)
		{
			_cameras[activeCameraIndex]->position = glm::vec3{0.0f, 7.0f, 5.0f};
			_cameras[activeCameraIndex]->rotation = glm::angleAxis(glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (ImGui::Button("screenshot##1") == true)
			screenShot();
		if (ImGui::Button("finish edit##3") == true)
		{
			_geometry.gameObject->position = glm::vec3{0.0f, 0.0f, 0.0f};
			_geometry.gameObject->scale = glm::vec3{0.2f, 0.2f, 0.2f};
			_geometry.gameObject->rotation = {1.0f, 0.0f, 0.0f, 0.0f};
			_geometry.gameObject->rotateangle = glm::vec3{0.0f, 0.0f, 0.0f};
			gameMode = false;
			const float aspect = 1.0f * _windowWidth / _windowHeight;
			constexpr float znear = 0.1f;
			constexpr float zfar = 10000.0f;
			_cameras[0] = new PerspectiveCamera(
				glm::radians(60.0f), aspect, 0.1f, 10000.0f);
			_cameras[0]->position = glm::vec3(0.0f, 0.0f, 15.0f);
		}

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FinalProject::initPhongShader()
{
	const char *vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"out vec3 fPosition;\n"
		"out vec3 fNormal;\n"
		"out vec2 fTexCoord;\n"
		"out vec3 fFragPos;\n"
		"out vec4 fFragPosLightSpace;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 lightSpaceMatrix;\n"

		"void main() {\n"
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	fTexCoord = aTexCoord;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"	fFragPos = vec3(model * vec4(aPosition, 1.0));\n"
		"	fFragPosLightSpace = lightSpaceMatrix * vec4(fFragPos, 1.0);\n"
		"}\n";

	const char *fsCode =
		"#version 330 core\n"
		"in vec3 fPosition;\n"
		"in vec3 fNormal;\n"
		"in vec2 fTexCoord;\n"
		"in vec3 fFragPos;\n"
		"in vec4 fFragPosLightSpace;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 ka;\n"
		"	vec3 kd;\n"
		"	vec3 ks;\n"
		"	float ns;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"struct SpotLight {\n"
		"	vec3 position;\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"	float angle;\n"
		"	float kc;\n"
		"	float kl;\n"
		"	float kq;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"
		"uniform SpotLight spotLight;\n"
		"uniform vec3 viewPosition;\n"
		"uniform sampler2D mapKd;\n"
		"uniform sampler2D shadowMap;\n"

		"float ShadowCalculation(vec4 fragPosLightSpace, float bias)\n"
		"{"
		"	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
		"	projCoords = projCoords * 0.5 + 0.5;\n"
		"	float closestDepth = texture(shadowMap, projCoords.xy).r;\n"
		"	float currentDepth = projCoords.z;\n"

		"	float shadow = 0.0;\n"
		"	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);\n"
		"	for (int x = -1; x <= 1; ++x)\n"
		"	{\n"
		"		for (int y = -1; y <= 1; ++y)\n"
		"		{\n"
		"			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
		"			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;\n"
		"		}\n"
		"	}\n"
		"	shadow /= 9.0;\n"

		"	if (projCoords.z > 1.0)\n"
		"		shadow = 0.0;\n"

		"	return shadow;\n"
		"}\n"

		"vec3 calcDirectionalLight(vec3 normal, vec3 viewDir) {\n"
		"	vec3 ambient = directionalLight.intensity * directionalLight.color * material.ka * texture(mapKd, fTexCoord).rgb;\n"

		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	float diff = max(dot(lightDir, normal), 0.0f);\n"
		"	vec3 diffuse = directionalLight.intensity * directionalLight.color * diff * material.kd * texture(mapKd, fTexCoord).rgb;\n"

		"	vec3 reflectDir = reflect(-lightDir, normal);\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);\n"
		"	vec3 specular = directionalLight.intensity * directionalLight.color * spec * material.ks;\n"
		"	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  "
		"	float shadow = 0;"
		"	shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"	return (ambient + (1.0 - shadow) * (diffuse + specular));\n"
		"}\n"

		"vec3 calcSpotLight(vec3 normal, vec3 viewDir) {\n"
		"	vec3 lightDir = normalize(spotLight.position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
		"	float distance = length(spotLight.position - fPosition);\n"
		"	float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);\n"

		"	if (theta <= spotLight.angle) {\n"
		"		vec3 ambient = spotLight.color *  material.ka * spotLight.intensity * attenuation * texture(mapKd, fTexCoord).rgb;\n"

		"		float diff = max(dot(lightDir, normal), 0.0f);\n"
		"		vec3 diffuse = spotLight.color * diff * material.kd * spotLight.intensity * attenuation * texture(mapKd, fTexCoord).rgb;\n"

		"		vec3 reflectDir = reflect(-lightDir, normal);\n"
		"		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);\n"
		"		vec3 specular = spotLight.color * spec * material.ks * spotLight.intensity * attenuation;\n"
		"		float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  "
		"		float shadow = 0;"
		"		shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"		return (ambient + (1.0 - shadow) * (diffuse + specular));\n"
		"	}\n"
		"	else {\n"
		"		vec3 ambient = spotLight.color *  material.ka * spotLight.intensity * attenuation * texture(mapKd, fTexCoord).rgb;\n"
		"		float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  "
		"		float shadow = 0;"
		"		shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"		return ambient;\n"
		"	}\n"
		"}\n"

		"void main() {\n"
		"	vec3 Normal = normalize(fNormal);\n"
		"	vec3 viewDir = normalize(viewPosition - fPosition);\n"
		"	vec3 result = calcDirectionalLight(Normal, viewDir) + calcSpotLight(Normal, viewDir);\n"
		"	color = vec4(result, 1.0f);\n"
		"}\n";

	_phongShader = new GLSLProgram;
	_phongShader->attachVertexShader(vsCode);
	_phongShader->attachFragmentShader(fsCode);
	_phongShader->link();

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FinalProject::initTexturePhoneShader()
{
	const char *vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"out vec3 fPosition;\n"
		"out vec3 fNormal;\n"
		"out vec2 fTexCoord;\n"
		"out vec3 fFragPos;\n"
		"out vec4 fFragPosLightSpace;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 lightSpaceMatrix;\n"

		"void main() {\n"
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	fTexCoord = aTexCoord;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"	fFragPos = vec3(model * vec4(aPosition, 1.0));\n"
		"	fFragPosLightSpace = lightSpaceMatrix * vec4(fFragPos, 1.0);\n"
		"}\n";

	const char *fsCode =
		"#version 330 core\n"
		"in vec3 fPosition;\n"
		"in vec3 fNormal;\n"
		"in vec2 fTexCoord;\n"
		"in vec3 fFragPos;\n"
		"in vec4 fFragPosLightSpace;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 ka;\n"
		"	vec3 kd;\n"
		"	vec3 ks;\n"
		"	float ns;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"struct SpotLight {\n"
		"	vec3 position;\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"	float angle;\n"
		"	float kc;\n"
		"	float kl;\n"
		"	float kq;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"
		"uniform SpotLight spotLight;\n"
		"uniform vec3 viewPosition;\n"
		"uniform sampler2D mapKd;\n"
		"uniform sampler2D shadowMap;\n"
		"uniform sampler2D texs[10];\n"

		"uniform int texnum;\n"
		"uniform float trange_l[10];\n"
		"uniform  int repeat[10];\n"
		"uniform  int reverse[10];\n"
		"uniform float trange_r[10];\n"
		"uniform int type[10];\n"

		"float ShadowCalculation(vec4 fragPosLightSpace, float bias)\n"
		"{"
		"	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
		"	projCoords = projCoords * 0.5 + 0.5;\n"
		"	float closestDepth = texture(shadowMap, projCoords.xy).r;\n"
		"	float currentDepth = projCoords.z;\n"

		"	float shadow = 0.0;\n"
		"	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);\n"
		"	for (int x = -1; x <= 1; ++x)\n"
		"	{\n"
		"		for (int y = -1; y <= 1; ++y)\n"
		"		{\n"
		"			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
		"			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;\n"
		"		}\n"
		"	}\n"
		"	shadow /= 9.0;\n"

		"	if (projCoords.z > 1.0)\n"
		"		shadow = 0.0;\n"

		"	return shadow;\n"
		"}\n"

		"vec3 calTexture()\n"
		"{\n"
		"vec3 texcolor=vec3(texture(mapKd, fTexCoord));"
		"if(texnum>=1)\n"
		"	for(int i=0;i<texnum;i++){\n"
		"		vec2 TexCoordV=fTexCoord;\n"
		"		float len=trange_r[i]-trange_l[i];\n"
		"		if(TexCoordV.y>trange_l[i]&&TexCoordV.y<trange_r[i]){\n"
		"			TexCoordV.y= (TexCoordV.y-trange_l[i])/len;\n"
		"			if(type[i]==0){\n"
		"				TexCoordV.x=(repeat[i]*TexCoordV.x);\n"
		" 				if(reverse[i]==1){\n"
		"					TexCoordV.y=1-TexCoordV.y;\n"
		"					TexCoordV.x=1-TexCoordV.x;\n"
		"			}\n"
		"		}\n"
		"		else{\n"
		"			float theta= TexCoordV.x*(2*3.1415926);\n"
		"			float r=(TexCoordV.y)/2;\n"
		"			if(reverse[i]==1){\n"
		"				TexCoordV.x=r*cos(theta)+0.5;\n"
		"				TexCoordV.y=1-r*sin(theta)+0.5;\n"
		"			}\n"
		"			else{\n"
		"				r=(1-TexCoordV.y)/2;\n"
		"				TexCoordV.x=r*cos(theta)+0.5;\n"
		"				TexCoordV.y=r*sin(theta)+0.5;\n"
		"			}\n"
		"		}\n"
		"		if(i==0)"
		"		texcolor*=vec3(texture(texs[0], TexCoordV));\n"
		"		if(i==1)"
		"		texcolor*=vec3(texture(texs[1], TexCoordV));\n"
		"		if(i==2)"
		"		texcolor*=vec3(texture(texs[2], TexCoordV));\n"
		"		if(i==3)"
		"		texcolor*=vec3(texture(texs[3], TexCoordV));\n"
		"		if(i==4)"
		"		texcolor*=vec3(texture(texs[4], TexCoordV));\n"
		"		if(i==5)"
		"		texcolor*=vec3(texture(texs[5], TexCoordV));\n"
		"		if(i==6)"
		"		texcolor*=vec3(texture(texs[6], TexCoordV));\n"
		"		if(i==7)"
		"		texcolor*=vec3(texture(texs[7], TexCoordV));\n"
		"		if(i==8)"
		"		texcolor*=vec3(texture(texs[8], TexCoordV));\n"
		"		if(i==9)"
		"		texcolor*=vec3(texture(texs[9], TexCoordV));\n"
		"	}\n"
		"}\n"
		"	return texcolor;\n"
		"}\n"

		"vec3 calcDirectionalLight(vec3 normal, vec3 viewDir, vec3 texcolor) {\n"
		"	vec3 ambient = directionalLight.intensity * directionalLight.color * material.ka;\n"

		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	float diff = max(dot(lightDir, normal), 0.0f);\n"
		"	vec3 diffuse = directionalLight.intensity * directionalLight.color * diff * material.kd;\n"

		"	vec3 reflectDir = reflect(-lightDir, normal);\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);\n"
		"	vec3 specular = directionalLight.intensity * directionalLight.color * spec * material.ks;\n"
		"	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  \n"
		"	float shadow = 0;\n"
		"	shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"	return (ambient + (1.0 - shadow) * (diffuse + specular)) * texcolor.rgb;\n"
		"}\n"

		"vec3 calcSpotLight(vec3 normal, vec3 viewDir, vec3 texcolor) {\n"
		"	vec3 lightDir = normalize(spotLight.position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
		"	float distance = length(spotLight.position - fPosition);\n"
		"	float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);\n"

		"	if (theta <= spotLight.angle) {\n"
		"		vec3 ambient = spotLight.color *  material.ka * spotLight.intensity * attenuation;\n"

		"		float diff = max(dot(lightDir, normal), 0.0f);\n"
		"		vec3 diffuse = spotLight.color * diff * material.kd * spotLight.intensity * attenuation;\n"

		"		vec3 reflectDir = reflect(-lightDir, normal);\n"
		"		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);\n"
		"		vec3 specular = spotLight.color * spec * material.ks * spotLight.intensity * attenuation;\n"
		"		float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  "
		"		float shadow = 0;"
		"		shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"		return (ambient + (1.0 - shadow) * (diffuse + specular)) * texcolor.rgb;\n"
		"	}\n"
		"	else {\n"
		"		vec3 ambient = spotLight.color *  material.ka * spotLight.intensity * attenuation;\n"
		"		float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  "
		"		float shadow = 0;"
		"		shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"		return ambient * texcolor.rgb;\n"
		"	}\n"
		"}\n"

		"void main() {\n"
		"	vec3 texcolor = calTexture();"
		"	vec3 Normal = normalize(fNormal);\n"
		"	vec3 viewDir = normalize(viewPosition - fPosition);\n"
		"	vec3 result = calcDirectionalLight(Normal, viewDir, texcolor) + calcSpotLight(Normal, viewDir, texcolor);\n"
		"	color = vec4(result, 1.0f);\n"
		"}\n";

	_phongTextureShader = new GLSLProgram;
	_phongTextureShader->attachVertexShader(vsCode);
	_phongTextureShader->attachFragmentShader(fsCode);
	_phongTextureShader->link();
}

void FinalProject::initSphereShader()
{
	const char *vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main() {\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";

	const char *fsCode =
		"#version 330 core\n"
		"out vec4 color;\n"

		"void main() {\n"
		"	color = vec4(1.0f);\n"
		"}\n";

	_sphereShader = new GLSLProgram;
	_sphereShader->attachVertexShader(vsCode);
	_sphereShader->attachFragmentShader(fsCode);
	_sphereShader->link();
}

void FinalProject::initDancerShader()
{
	const char *vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"out vec3 fPosition;\n"
		"out vec3 fNormal;\n"
		"out vec3 fFragPos;\n"
		"out vec4 fFragPosLightSpace;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 lightSpaceMatrix;\n"

		"void main() {\n"
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"	fFragPos = vec3(model * vec4(aPosition, 1.0));\n"
		"	fFragPosLightSpace = lightSpaceMatrix * vec4(fFragPos, 1.0);\n"
		"}\n";

	const char *fsCode =
		"#version 330 core\n"
		"in vec3 fPosition;\n"
		"in vec3 fNormal;\n"
		"in vec3 fFragPos;\n"
		"in vec4 fFragPosLightSpace;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 ka;\n"
		"	vec3 kd;\n"
		"	vec3 ks;\n"
		"	float ns;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"struct SpotLight {\n"
		"	vec3 position;\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"	float angle;\n"
		"	float kc;\n"
		"	float kl;\n"
		"	float kq;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"
		"uniform SpotLight spotLight;\n"
		"uniform vec3 viewPosition;\n"
		"uniform sampler2D shadowMap;\n"

		"float ShadowCalculation(vec4 fragPosLightSpace, float bias)\n"
		"{"
		"	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
		"	projCoords = projCoords * 0.5 + 0.5;\n"
		"	float closestDepth = texture(shadowMap, projCoords.xy).r;\n"
		"	float currentDepth = projCoords.z;\n"

		"	float shadow = 0.0;\n"
		"	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);\n"
		"	for (int x = -1; x <= 1; ++x)\n"
		"	{\n"
		"		for (int y = -1; y <= 1; ++y)\n"
		"		{\n"
		"			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
		"			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;\n"
		"		}\n"
		"	}\n"
		"	shadow /= 9.0;\n"

		"	if (projCoords.z > 1.0)\n"
		"		shadow = 0.0;\n"

		"	return shadow;\n"
		"}\n"

		"vec3 calcDirectionalLight(vec3 normal, vec3 viewDir) {\n"
		"	vec3 ambient = directionalLight.intensity * directionalLight.color * material.ka;\n"

		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	float diff = max(dot(lightDir, normal), 0.0f);\n"
		"	vec3 diffuse = directionalLight.intensity * directionalLight.color * diff * material.kd;\n"

		"	vec3 reflectDir = reflect(-lightDir, normal);\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);\n"
		"	vec3 specular = directionalLight.intensity * directionalLight.color * spec * material.ks;\n"

		"	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  "
		"	float shadow = 0;"
		"	shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"	return (ambient + (1.0 - shadow) * (diffuse + specular));\n"
		"}\n"

		"vec3 calcSpotLight(vec3 normal, vec3 viewDir) {\n"
		"	vec3 lightDir = normalize(spotLight.position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
		"	float distance = length(spotLight.position - fPosition);\n"
		"	float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);\n"

		"	if (theta <= spotLight.angle) {\n"
		"		vec3 ambient = spotLight.color *  material.ka * spotLight.intensity * attenuation;\n"

		"		float diff = max(dot(lightDir, normal), 0.0f);\n"
		"		vec3 diffuse = spotLight.color * diff * material.kd * spotLight.intensity * attenuation;\n"

		"		vec3 reflectDir = reflect(-lightDir, normal);\n"
		"		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);\n"
		"		vec3 specular = spotLight.color * spec * material.ks * spotLight.intensity * attenuation;\n"

		"	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  "
		"	float shadow = 0;"
		"	shadow = ShadowCalculation(fFragPosLightSpace,bias); "
		"	return (ambient + (1.0 - shadow) * (diffuse + specular));\n"
		"	}\n"
		"	else {\n"
		"		vec3 ambient = spotLight.color *  material.ka * spotLight.intensity * attenuation;\n"

		"		return ambient;\n"
		"	}\n"
		"}\n"

		"void main() {\n"
		"	vec3 Normal = normalize(fNormal);\n"
		"	vec3 viewDir = normalize(viewPosition - fPosition);\n"
		"	vec3 result = calcDirectionalLight(Normal, viewDir) + calcSpotLight(Normal, viewDir);\n"
		"	color = vec4(result, 1.0f);\n"
		"}\n";

	_dancerShader = new GLSLProgram;
	_dancerShader->attachVertexShader(vsCode);
	_dancerShader->attachFragmentShader(fsCode);
	_dancerShader->link();
}

void FinalProject::initFrameShader()
{
	const char *vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * (vec4(aPos, 1.0));\n"
		"}\n";

	const char *fsCode =
		"#version 330 core\n"
		"out vec4 FragColor;\n"

		"uniform vec3 color;\n"

		"void main()\n"
		"{\n"
		"	FragColor = vec4(color, 1.0);\n"
		"}\n";

	_frameShader = new GLSLProgram;
	_frameShader->attachVertexShader(vsCode);
	_frameShader->attachFragmentShader(fsCode);
	_frameShader->link();
}

void FinalProject::initDepthShader()
{
	const char *vsCode =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"

		"uniform mat4 lightSpaceMatrix;\n"
		"uniform mat4 model;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);\n"
		"}\n";

	const char *fsCode =
		"#version 330 core\n"

		"void main()\n"
		"{\n"
		"}\n";

	_depthShader = new GLSLProgram;
	_depthShader->attachVertexShader(vsCode);
	_depthShader->attachFragmentShader(fsCode);
	_depthShader->link();
}

void FinalProject::screenShot()
{
	std::string savePath = "../output/output.png";
	GLubyte *pPixelData;
	GLint PixelDataLength;
	pPixelData = (GLubyte *)malloc(_windowWidth * _windowHeight * 3);
	if (pPixelData == 0)
		return;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, _windowWidth, _windowHeight, GL_RGB, GL_UNSIGNED_BYTE, pPixelData);
	stbi_write_png(savePath.c_str(), _windowWidth, _windowHeight, 3, pPixelData, 0);
	free(pPixelData);
	int iw = _windowWidth, ih = _windowHeight, n = 3;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *idata = stbi_load(savePath.c_str(), &iw, &ih, &n, 0);
	stbi_write_png(savePath.c_str(), _windowWidth, _windowHeight, 3, idata, 0);
	stbi_image_free(idata);
}

void FinalProject::setPanCamera(Model *model)
{
	_cameras[activeCameraIndex]->position = model->position + model->getFront() * -5.0f + model->getUp() * 5.0f;
	_cameras[activeCameraIndex]->rotation = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
	_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.x / 2.0f), model->oldright.x * sin(model->rotateangle.x / 2.0f), model->oldright.y * sin(model->rotateangle.x / 2.0f), model->oldright.z * sin(model->rotateangle.x / 2.0f)} * _cameras[activeCameraIndex]->rotation;
	_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.y / 2.0f), model->oldup.x * sin(model->rotateangle.y / 2.0f), model->oldup.y * sin(model->rotateangle.y / 2.0f), model->oldup.z * sin(model->rotateangle.y / 2.0f)} * _cameras[activeCameraIndex]->rotation;
	_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.z / 2.0f), -model->oldfront.x * sin(model->rotateangle.z / 2.0f), -model->oldfront.y * sin(model->rotateangle.z / 2.0f), -model->oldfront.z * sin(model->rotateangle.z / 2.0f)} * _cameras[activeCameraIndex]->rotation;
	_cameras[activeCameraIndex]->rotation = glm::quat{cos(-0.25f), model->getRight().x * sin(-0.25f), model->getRight().y * sin(-0.25f), model->getRight().z * sin(-0.25f)} * _cameras[activeCameraIndex]->rotation;
}

void FinalProject::setPanCamera(DIYmodel *model)
{
	if (!gameMode)
	{
		_cameras[activeCameraIndex]->position = model->position + model->getFront() * -5.0f + model->getUp() * 5.0f;
		_cameras[activeCameraIndex]->rotation = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
		_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.x / 2.0f), model->oldright.x * sin(model->rotateangle.x / 2.0f), model->oldright.y * sin(model->rotateangle.x / 2.0f), model->oldright.z * sin(model->rotateangle.x / 2.0f)} * _cameras[activeCameraIndex]->rotation;
		_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.y / 2.0f), model->oldup.x * sin(model->rotateangle.y / 2.0f), model->oldup.y * sin(model->rotateangle.y / 2.0f), model->oldup.z * sin(model->rotateangle.y / 2.0f)} * _cameras[activeCameraIndex]->rotation;
		_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.z / 2.0f), -model->oldfront.x * sin(model->rotateangle.z / 2.0f), -model->oldfront.y * sin(model->rotateangle.z / 2.0f), -model->oldfront.z * sin(model->rotateangle.z / 2.0f)} * _cameras[activeCameraIndex]->rotation;
		_cameras[activeCameraIndex]->rotation = glm::quat{cos(-0.25f), model->getRight().x * sin(-0.25f), model->getRight().y * sin(-0.25f), model->getRight().z * sin(-0.25f)} * _cameras[activeCameraIndex]->rotation;
	}
	if (gameMode)
	{
		_cameras[activeCameraIndex]->position = model->position + model->getFront() * -10.0f + model->getUp() * 3.0f;
		_cameras[activeCameraIndex]->rotation = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
		_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.x / 2.0f), model->oldright.x * sin(model->rotateangle.x / 2.0f), model->oldright.y * sin(model->rotateangle.x / 2.0f), model->oldright.z * sin(model->rotateangle.x / 2.0f)} * _cameras[activeCameraIndex]->rotation;
		_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.y / 2.0f), model->oldup.x * sin(model->rotateangle.y / 2.0f), model->oldup.y * sin(model->rotateangle.y / 2.0f), model->oldup.z * sin(model->rotateangle.y / 2.0f)} * _cameras[activeCameraIndex]->rotation;
		_cameras[activeCameraIndex]->rotation = glm::quat{cos(model->rotateangle.z / 2.0f), -model->oldfront.x * sin(model->rotateangle.z / 2.0f), -model->oldfront.y * sin(model->rotateangle.z / 2.0f), -model->oldfront.z * sin(model->rotateangle.z / 2.0f)} * _cameras[activeCameraIndex]->rotation;
	}
}

bool FinalProject::isInBoundingBoxGlobal(Camera *camera)
{
	if (!gameMode && (_wall.floor->isInBoundingBoxGlobal(camera) ||
					  _wall.back->isInBoundingBoxGlobal(camera) ||
					  _wall.left->isInBoundingBoxGlobal(camera) ||
					  _wall.right->isInBoundingBoxGlobal(camera) ||
					  _geometry.cylinder->isInBoundingBoxGlobal(camera) ||
					  _geometry.cone->isInBoundingBoxGlobal(camera) ||
					  _geometry.prism->isInBoundingBoxGlobal(camera) ||
					  _geometry.cube->isInBoundingBoxGlobal(camera) ||
					  _geometry.prismaticTable4->isInBoundingBoxGlobal(camera) ||
					  _geometry.prismaticTable6->isInBoundingBoxGlobal(camera) ||
					  _geometry.gameObject->isInBoundingBoxGlobal(camera) ||
					  _postures[postureFrameNumber / 5]->isInBoundingBoxGlobal(camera) ||
					  _geometry.knot->isInBoundingBoxGlobal(camera)))
		return true;
	else if (gameMode && (_wall.floor->isInBoundingBoxGlobal(camera) ||
						  _geometry.gameObject->isInBoundingBoxGlobal(camera)))
		return true;
	else
		return false;
}

void FinalProject::initDepthPeelingShaders()
{
	const char *shadeVsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"out vec3 fPos;\n"
		"out vec3 fNormal;\n"

		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"

		"void main() {\n"
		"	fPos = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";

	const char *blendVsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec2 aPosition;\n"
		"layout(location = 1) in vec2 aTexCoords;\n"
		"out vec2 fTexCoords;\n"
		"void main() {\n"
		"	fTexCoords = aTexCoords;\n"
		"	gl_Position = vec4(aPosition, 0.0f, 1.0f);\n"
		"}\n";

	const char *initFsCode =
		"#version 330 core\n"
		"in vec3 fPos;\n"
		"in vec3 fNormal;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 albedo;\n"
		"	float ka;\n"
		"	vec3 kd;\n"
		"	float transparent;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"

		"vec3 lambertShading() {\n"
		"	vec3 normal = normalize(fNormal);\n"
		"	if (!gl_FrontFacing) { \n"
		"		normal = -normal;\n"
		"	}\n"
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	vec3 ambient = material.ka * material.albedo ;\n"
		"	vec3 diffuse = material.kd * max(dot(lightDir, normal), 0.0f) * \n"
		"				   directionalLight.color * directionalLight.intensity;\n"
		"	return ambient + diffuse;\n"
		"}\n"

		"void main() {\n"
		"	vec3 premultiedColor = lambertShading() * material.transparent;\n"
		"	color = vec4(premultiedColor, 1.0f - material.transparent);\n"
		"}\n";

	const char *peelFsCode =
		"#version 330 core\n"
		"in vec3 fPos;\n"
		"in vec3 fNormal;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 albedo;\n"
		"	float ka;\n"
		"	vec3 kd;\n"
		"	float transparent;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"struct WindowExtent {\n"
		"	int width;\n"
		"	int height;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"
		"uniform sampler2D depthTexture;\n"
		"uniform WindowExtent windowExtent;\n"

		"float getPeelingDepth() {\n"
		"	float u = gl_FragCoord.x / windowExtent.width;\n"
		"	float v = gl_FragCoord.y / windowExtent.height;\n"
		"	return texture(depthTexture, vec2(u, v)).r;\n"
		"}\n"

		"vec3 lambertShading() {\n"
		"	vec3 normal = normalize(fNormal);\n"
		"	if (!gl_FrontFacing) { \n"
		"		normal = -normal;\n"
		"	}\n"
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	vec3 ambient = material.ka * material.albedo ;\n"
		"	vec3 diffuse = material.kd * max(dot(lightDir, normal), 0.0f) * \n"
		"				   directionalLight.color * directionalLight.intensity;\n"
		"	return ambient + diffuse;\n"
		"}\n"

		"void main() {\n"
		"	if (gl_FragCoord.z <= getPeelingDepth()) {\n"
		"		discard;\n"
		"	}\n"

		"	vec3 premultiedColor = lambertShading() * material.transparent;\n"
		"	color = vec4(premultiedColor, material.transparent);\n"
		"}\n";

	const char *blendFsCode =
		"#version 330 core\n"
		"out vec4 color;\n"

		"struct WindowExtent {\n"
		"	int width;\n"
		"	int height;\n"
		"};\n"

		"uniform WindowExtent windowExtent;\n"
		"uniform sampler2D blendTexture;\n"

		"void main() {\n"
		"	float u = gl_FragCoord.x / windowExtent.width;\n"
		"	float v = gl_FragCoord.y / windowExtent.height;\n"
		"	color = texture(blendTexture, vec2(u, v));\n"
		"}\n";

	const char *finalFsCode =
		"#version 330 core\n"
		"out vec4 color;\n"

		"struct WindowExtent {\n"
		"	int width;\n"
		"	int height;\n"
		"};\n"

		"uniform WindowExtent windowExtent;\n"
		"uniform sampler2D blendTexture;\n"
		"uniform vec4 backgroundColor;\n"

		"void main() {\n"
		"	float u = gl_FragCoord.x / windowExtent.width;\n"
		"	float v = gl_FragCoord.y / windowExtent.height;\n"
		"	vec4 frontColor = texture(blendTexture, vec2(u, v));\n"
		"	color = frontColor + backgroundColor * frontColor.a;\n"
		"}\n";

	_depthPeelingInitShader.reset(new GLSLProgram);
	_depthPeelingInitShader->attachVertexShader(shadeVsCode);
	_depthPeelingInitShader->attachFragmentShader(initFsCode);
	_depthPeelingInitShader->link();

	_depthPeelingShader.reset(new GLSLProgram);
	_depthPeelingShader->attachVertexShader(shadeVsCode);
	_depthPeelingShader->attachFragmentShader(peelFsCode);
	_depthPeelingShader->link();

	_depthPeelingBlendShader.reset(new GLSLProgram);
	_depthPeelingBlendShader->attachVertexShader(blendVsCode);
	_depthPeelingBlendShader->attachFragmentShader(blendFsCode);
	_depthPeelingBlendShader->link();

	_depthPeelingFinalShader.reset(new GLSLProgram);
	_depthPeelingFinalShader->attachVertexShader(blendVsCode);
	_depthPeelingFinalShader->attachFragmentShader(finalFsCode);
	_depthPeelingFinalShader->link();
}

void FinalProject::initDepthPeelingResources()
{
	// init materials
	_knotMaterial.reset(new TransparentMaterial());
	_knotMaterial->albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	_knotMaterial->ka = 0.03f;
	_knotMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);
	_knotMaterial->transparent = 0.6f;

	_diyMaterial.reset(new TransparentMaterial());
	_diyMaterial->albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	_diyMaterial->ka = 0.03f;
	_diyMaterial->kd = glm::vec3(1.0f, 1.0f, 1.0f);
	_diyMaterial->transparent = 0.6f;

	// init fullscreen quad
	_fullscreenQuad.reset(new FullscreenQuad);

	for (int i = 0; i < 2; ++i)
	{
		_fbos[i].reset(new Framebuffer);
		_colorTextures[i].reset(
			new DataTexture2D(GL_RGBA, _windowWidth, _windowHeight, GL_RGBA, GL_FLOAT));
		_depthTextures[i].reset(
			new DataTexture2D(GL_DEPTH_COMPONENT32F, _windowWidth, _windowHeight, GL_DEPTH_COMPONENT, GL_FLOAT));

		_fbos[i]->bind();
		_fbos[i]->attach(*_colorTextures[i], GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
		_fbos[i]->attach(*_depthTextures[i], GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);
		_fbos[i]->unbind();
	}

	_colorBlendTexture.reset(new DataTexture2D(GL_RGBA, _windowWidth, _windowHeight, GL_RGBA, GL_FLOAT));
	_colorBlendFbo.reset(new Framebuffer);
	_colorBlendFbo->bind();
	_colorBlendFbo->attach(*_colorBlendTexture, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	_colorBlendFbo->attach(*_depthTextures[0], GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);
	_colorBlendFbo->unbind();
}

void FinalProject::renderWithDepthPeeling(Model *model)
{
	const glm::mat4 projection = _cameras[activeCameraIndex]->getProjectionMatrix();
	const glm::mat4 view = _cameras[activeCameraIndex]->getViewMatrix();

	// 1. initialize min depth buffer
	_colorBlendFbo->bind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	_depthPeelingInitShader->use();
	// 1.1 set transformation matrices
	_depthPeelingInitShader->setMat4("projection", projection);
	_depthPeelingInitShader->setMat4("view", view);
	_depthPeelingInitShader->setMat4("model", model->getModelMatrix());
	// 1.2 set light
	_depthPeelingInitShader->setVec3("directionalLight.direction", _directionalLight->direction);
	_depthPeelingInitShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
	_depthPeelingInitShader->setVec3("directionalLight.color", _directionalLight->color);
	// 1.3 set material
	_depthPeelingInitShader->setVec3("material.albedo", _knotMaterial->albedo);
	_depthPeelingInitShader->setFloat("material.ka", _knotMaterial->ka);
	_depthPeelingInitShader->setVec3("material.kd", _knotMaterial->kd);
	_depthPeelingInitShader->setFloat("material.transparent", _knotMaterial->transparent);

	model->draw();

	for (int i = 1; i < 4; ++i)
	{

		_depthPeelingShader->use();
		_depthPeelingShader->setMat4("projection", projection);
		_depthPeelingShader->setMat4("view", view);
		_depthPeelingShader->setMat4("model", model->getModelMatrix());
		_depthPeelingShader->setVec3("directionalLight.direction", _directionalLight->direction);
		_depthPeelingShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
		_depthPeelingShader->setVec3("directionalLight.color", _directionalLight->color);
		_depthPeelingShader->setVec3("material.albedo", _knotMaterial->albedo);
		_depthPeelingShader->setFloat("material.ka", _knotMaterial->ka);
		_depthPeelingShader->setVec3("material.kd", _knotMaterial->kd);
		_depthPeelingShader->setFloat("material.transparent", _knotMaterial->transparent);
		_depthPeelingShader->setInt("windowExtent.width", _windowWidth);
		_depthPeelingShader->setInt("windowExtent.height", _windowHeight);

		glEnable(GL_DEPTH_TEST);

		_fbos[i % 2]->bind();
		glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//_fbos[(i + 1) % 2]->bind();
		_depthTextures[(i + 1) % 2]->bind(0);
		model->draw();

		_colorBlendFbo->bind();
		_colorTextures[i % 2]->bind(0);
		_depthPeelingBlendShader->use();
		_depthPeelingBlendShader->setInt("blendTexture", 0);
		_depthPeelingBlendShader->setInt("windowExtent.width", _windowWidth);
		_depthPeelingBlendShader->setInt("windowExtent.height", _windowHeight);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

		//_knot->draw();

		_fullscreenQuad->draw();
		glDisable(GL_BLEND);
	}

	// 3. final pass: blend the peeling result with the background color
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	_depthPeelingFinalShader->use();
	// 3.1 set the window extent
	_depthPeelingFinalShader->setInt("windowExtent.width", _windowWidth);
	_depthPeelingFinalShader->setInt("windowExtent.height", _windowHeight);
	// 3.2 set the blend texture
	glActiveTexture(GL_TEXTURE0);
	_colorBlendTexture->bind();
	// 3.3 set the background color
	_depthPeelingFinalShader->setVec4("backgroundColor", _clearColor);

	_fullscreenQuad->draw();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void FinalProject::initAlphaBlendingShader()
{
	const char *vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"out vec3 fPos;\n"
		"out vec3 fNormal;\n"

		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"

		"void main() {\n"
		"	fPos = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"} \n";

	const char *fsCode =
		"#version 330 core\n"
		"in vec3 fPos;\n"
		"in vec3 fNormal;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 albedo;\n"
		"	float ka;\n"
		"	vec3 kd;\n"
		"	float transparent;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"

		"void main() {\n"
		"	vec3 normal = normalize(fNormal);\n"
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	vec3 ambient = material.ka * material.albedo ;\n"
		"	vec3 diffuse = material.kd * max(dot(lightDir, normal), 0.0f) * \n"
		"				   directionalLight.color * directionalLight.intensity;\n"
		"	color = vec4(ambient + diffuse, material.transparent);\n"
		"}\n";

	_alphaBlendingShader.reset(new GLSLProgram);
	_alphaBlendingShader->attachVertexShader(vsCode);
	_alphaBlendingShader->attachFragmentShader(fsCode);
	_alphaBlendingShader->link();
}

void FinalProject::renderWithAlphaBlending(Model *model)
{
	//  render transparent objects
	_alphaBlendingShader->use();
	// 1 set transformation matrices
	_alphaBlendingShader->setMat4("projection", _cameras[activeCameraIndex]->getProjectionMatrix());
	_alphaBlendingShader->setMat4("view", _cameras[activeCameraIndex]->getViewMatrix());
	_alphaBlendingShader->setMat4("model", model->getModelMatrix());
	// 2 set light
	_alphaBlendingShader->setVec3("directionalLight.direction", _directionalLight->direction);
	_alphaBlendingShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
	_alphaBlendingShader->setVec3("directionalLight.color", _directionalLight->color);
	// 3 set material
	_alphaBlendingShader->setVec3("material.albedo", _knotMaterial->albedo);
	_alphaBlendingShader->setFloat("material.ka", _knotMaterial->ka);
	_alphaBlendingShader->setVec3("material.kd", _knotMaterial->kd);
	_alphaBlendingShader->setFloat("material.transparent", _knotMaterial->transparent);

	glEnable(GL_DEPTH_TEST);
	glColorMask(false, false, false, false);

	model->draw();

	glColorMask(true, true, true, true);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	model->draw();

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

void FinalProject::renderWithAlphaBlending(DIYmodel *model)
{
	//  render transparent objects
	_alphaBlendingShader->use();
	// 1 set transformation matrices
	_alphaBlendingShader->setMat4("projection", _cameras[activeCameraIndex]->getProjectionMatrix());
	_alphaBlendingShader->setMat4("view", _cameras[activeCameraIndex]->getViewMatrix());
	_alphaBlendingShader->setMat4("model", model->getModelMatrix());
	// 2 set light
	_alphaBlendingShader->setVec3("directionalLight.direction", _directionalLight->direction);
	_alphaBlendingShader->setFloat("directionalLight.intensity", _directionalLight->intensity);
	_alphaBlendingShader->setVec3("directionalLight.color", _directionalLight->color);
	// 3 set material
	_alphaBlendingShader->setVec3("material.albedo", _diyMaterial->albedo);
	_alphaBlendingShader->setFloat("material.ka", _diyMaterial->ka);
	_alphaBlendingShader->setVec3("material.kd", _diyMaterial->kd);
	_alphaBlendingShader->setFloat("material.transparent", _diyMaterial->transparent);

	glEnable(GL_DEPTH_TEST);
	glColorMask(false, false, false, false);

	model->draw();

	glColorMask(true, true, true, true);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	model->draw();

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}
