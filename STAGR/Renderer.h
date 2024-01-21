#pragma once
#include "WinInclude.h"
#include "Window.h"
#include <d3dcompiler.h>
#include <vector>
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"d3d11.lib")

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"



struct LightData {
	glm::vec3 light_position;
	glm::vec3 light_color;
};


struct SceneData {
private:
	glm::mat4 LightSpaceMatrix;

	glm::mat4 light_projection;

	float light_constant = 1;
	float light_linear = 0.09f;
	float light_quadratic = 0.032f;


public:
	LightData light_data;

	glm::mat4 GetLightSpaceMatrix()
	{
		float near_plane = 1.0f, far_plane = 20.0f;
		light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		glm::mat4 light_view = glm::lookAt(light_data.light_position, glm::vec3(0.0f, 0.0f, 0.0f)
			, glm::vec3(0.0f, 1.0f, 0.0f));
		LightSpaceMatrix = light_projection * light_view;
		return LightSpaceMatrix;
	}
};








class Renderer {
public:
	Renderer() : NO_LIGHTING(false) { Init(); }

	Camera camera;

	inline Camera& GetCamera() { return camera; }

	void Init();

	void ClearBuffer(float r, float g, float b);

	void EndFrame();

	void DrawIndexed(UINT count);

	void Shutdown();

	void InitScene();

	void DrawScene(float alpha);

	void DepthPass(float alpha);

	void DrawObject(float size, glm::vec3 position, unsigned model_id, bool depth_pass = false);

	inline SceneData& GetSceneData() { return m_sceneData; }

	void Clean();

	bool NO_LIGHTING;
	static float lightPosX,lightPosY,lightPosZ;

private:
	SceneData m_sceneData;

	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ModelViewProjection;

	unsigned int depthMapFBO;
	unsigned int depthMap;

	
	unsigned int m_width;
	int m_height;
	void createDepthMap();
	
	void ApplyEditorPositionsToModel(Model& model ,float size,glm::vec3 positions);
	void drawPointLight(float size,glm::vec3 pos);
	void createModels();
	void createShaders();
	void init_mvp();
	std::vector<Model> m_Models;
	std::vector<Shader> m_Shaders;

	// INDEX VALUES

	unsigned int PHONG_SHADERINDEX;
	unsigned int POINTLIGHT_SHADERINDEX;
	unsigned int DEPTH_SHADERINDEX;
	unsigned int NOLIGHTING_SHADERINDEX;

	unsigned int SPHERE_MODELINDEX;
	unsigned int  BACKPACK_MODELINDEX;

	//DEBUG BULLSHIT









public:
	
	inline ComPtr<ID3D11Device>& GetDevice()
	{
		return m_Device;
	}

	inline ComPtr<ID3D11DeviceContext>& GetContext()
	{
		return m_Context;
	}
	
private:
	ComPtr<ID3D11Device> m_Device = nullptr;
	ComPtr<IDXGISwapChain> m_Swap = nullptr;
	ComPtr<ID3D11DeviceContext> m_Context = nullptr;
	ComPtr<ID3D11RenderTargetView> m_Target = nullptr;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator = (const Renderer&) = delete;

	inline static Renderer& Get()
	{
		static Renderer instance;
		return instance;
	}


};

