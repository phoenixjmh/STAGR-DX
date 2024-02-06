#include "Renderer.h"
#include "Physics.h"
#include "Camera.h"

#define GFX_THROW_HR(hrcall) if( FAILED( hr = (hrcall) ) ) std::cerr<< __LINE__<<__FILE__<<hr <<"\n"
#define GFX_THROW () std::cerr<< __LINE__<<__FILE__<<GetLastError()<<"\n"

void Renderer::Init()
{


    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2;
    sd.OutputWindow =
        Window::Get().GetWindow();
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = 0;


    HRESULT hr;
    if (FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &m_Swap,
        &m_Device,
        nullptr,
        &m_Context

    )))
    {
        std::cout<<"Failed!";
    }
    Log("Created Swap");
    ComPtr<ID3D11RasterizerState> rasterizer_state;
    D3D11_RASTERIZER_DESC ras_desc = {};
    ras_desc.CullMode = D3D11_CULL_BACK;
    ras_desc.FillMode = D3D11_FILL_SOLID;
    ras_desc.FrontCounterClockwise = FALSE;
    m_Device->CreateRasterizerState(&ras_desc, &rasterizer_state);
    m_Context->RSSetState(rasterizer_state.Get());
    ComPtr<ID3D11Texture2D> back_buffer;
    m_Swap->GetBuffer(0, __uuidof(ID3D11Texture2D), &back_buffer);

    D3D11_TEXTURE2D_DESC back_buffer_desc;
    back_buffer->GetDesc(&back_buffer_desc);


   

    m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_Target);

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    ComPtr<ID3D11DepthStencilState> pDSState;
    m_Device->CreateDepthStencilState(&dsDesc, &pDSState);

    // bind depth state
    m_Context->OMSetDepthStencilState(pDSState.Get(), 1u);

    // create depth stensil texture
    ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = back_buffer_desc.Width;
    descDepth.Height = back_buffer_desc.Height;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_HR(m_Device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

    // create view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    GFX_THROW_HR(m_Device->CreateDepthStencilView(
        pDepthStencil.Get(), &descDSV, &m_DepthStencilView
    ));

    // bind depth stensil view to OM
    m_Context->OMSetRenderTargets(1u, m_Target.GetAddressOf(), m_DepthStencilView.Get());

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)back_buffer_desc.Width;  
    vp.Height = (float)back_buffer_desc.Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    //m_Context->RSSetViewports(1u, &vp);
    m_Context->RSSetViewports(1u, &vp);
    std::cout<<GetLastError();
    
}


void Renderer::ClearBuffer(float r, float g, float b)
{
    m_Context->OMSetRenderTargets(1u, m_Target.GetAddressOf(), m_DepthStencilView.Get());
    const float color[] = { r,g,b,1.f };
    m_Context->ClearRenderTargetView(m_Target.Get(), color);
    m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    

}



void Renderer::EndFrame()
{

    HRESULT hr;
    if (FAILED(hr = m_Swap->Present(1, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            m_Device->GetDeviceRemovedReason();
        }
        else
        {
            std::cout<<hr;
        }
    }
}

void Renderer::InitScene()
{
    m_sceneData.light_data.light_color = { 1, 1, 1 };
    m_sceneData.light_data.light_position = glm::vec3(0.5f, 2, 2);
    const unsigned int DM_WIDTH = 1024, DM_HEIGHT = 1024;
    createShaders();
    createModels();
    createDepthMap();
}

void Renderer::createShaders()
{
    Shader m_shader(L"VertexShader.cso", L"PixelShader.cso");
    Shader point_light(L"VSPointLight.cso", L"PSPointLight.cso");
 /*   Shader m_depth(L"shaders/depth.vert", L"shaders/depth.frag");
    Shader NOLIGHTING(L"shaders/shader.vert", L"shaders/NOLIGHTING.frag");*/

    m_Shaders = {
        m_shader,
       point_light
       // NOLIGHTING
    };

    PHONG_SHADERINDEX = 0;
    POINTLIGHT_SHADERINDEX = 1;
    //DEPTH_SHADERINDEX = 1;
    //NOLIGHTING_SHADERINDEX = 2;
}


void Renderer::createModels()
{
    //VAFlags model_flags = { false, NONE };

    Model m_sphere("models/sphere/sphere.obj"/*, model_flags*/);

   Model m_backpack("models/backpack/backpack.obj");

    /*Model m_floor("models/floor/floor.obj", model_flags);

    VAFlags primitive_flags = { false, CUBE };

    Model m_cube("wherever", primitive_flags);*/

    m_Models = {
        m_sphere,
        m_backpack,
        /* m_cube,
        m_floor*/
    };
    SPHERE_MODELINDEX = 0;
    BACKPACK_MODELINDEX = 1;
}

void Renderer::init_mvp()
{
    /*ModelMatrix = glm::mat4(1);
    ProjectionMatrix = glm::perspective(glm::radians(45.0f), 1000.0f / 1000.0f, 0.1f, 100.0f);
    ViewMatrix = glm::lookAt(camera.camera_position, camera.camera_position + camera.camera_front
        , camera.camera_up);*/
}

void Renderer::createDepthMap()
{
    //const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    //glGenFramebuffers(1, &depthMapFBO);
    //// create depth texture
    //glGenTextures(1, &depthMap);
    //glBindTexture(GL_TEXTURE_2D, depthMap);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0
    //    , GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //// attach depth texture as FBO's depth buffer
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::drawPointLight(float size,glm::vec3 pos) {
    //set up everything right here.
    Model activeModel = m_Models[SPHERE_MODELINDEX];
    Shader activeShader = m_Shaders[POINTLIGHT_SHADERINDEX];
    activeModel.Size = size;
    activeModel.Position = pos;
    activeModel.Draw(activeShader, depthMap);

}
void Renderer::DrawObject(float size, glm::vec3 position, unsigned int model_id, bool depth_pass)
{
    Model activeModel = m_Models[model_id];

    Shader activeShader = NO_LIGHTING ?
        m_Shaders[NOLIGHTING_SHADERINDEX] : m_Shaders[PHONG_SHADERINDEX];
    if (depth_pass)
        activeShader = m_Shaders[DEPTH_SHADERINDEX];

    init_mvp();

    ApplyEditorPositionsToModel(activeModel,size,position);
    

    activeModel.Draw(activeShader, depthMap);
}

void Renderer::ApplyEditorPositionsToModel(Model&  model,float size,glm::vec3 position)
{
    model.Position = position;
    model.Size = size;
    model.LightPosition = m_sceneData.light_data.light_position;
}

void Renderer::DrawScene(float alpha)
{
    DepthPass(alpha);
    drawPointLight(1,m_sceneData.light_data.light_position);

 
    for (auto& s : Physics::all_sand)
    {
        glm::vec2 interpolatedPosition = s.pos * alpha + s.prev_pos * (1.0f - alpha);
        glm::vec3 render_position = { interpolatedPosition.x, interpolatedPosition.y, 1 };

        DrawObject(s.editor_size, s.editor_pos, s.Model_ID);
    }
}

void Renderer::DepthPass(float alpha)
{
    //// Optional parameter for the DrawObject function
    //bool depth_pass = true;
    //Shader depth_shader = m_Shaders[DEPTH_SHADERINDEX];
    //depth_shader.use();
    //depth_shader.setMat4("LightSpaceMatrix", m_sceneData.GetLightSpaceMatrix());
    //glViewport(0, 0, m_width, m_height);
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glClear(GL_DEPTH_BUFFER_BIT);
    //glCullFace(GL_FRONT);
    //for (auto& s : Physics::all_sand)
    //{
    //    glm::vec2 interpolatedPosition = s.pos * alpha + s.prev_pos * (1.0f - alpha);
    //    glm::vec3 render_position = { interpolatedPosition.x, interpolatedPosition.y, 1 };

    //    DrawObject(1, s.editor_pos, s.Model_ID, depth_pass);
    //}
}

void Renderer::Clean()
{
    //m_Models.clear();
    m_Shaders.clear();

    //glfwTerminate();
}