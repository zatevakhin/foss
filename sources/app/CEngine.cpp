#include "CEngine.hpp"

#include "app/auxiliary/imgui.hpp"
#include "app/auxiliary/trace.hpp"
#include "app/geometry/CCubeSphere.hpp"
#include "app/input/CCameraListener.hpp"
#include "app/input/CEngineListener.hpp"

#include "components/C3dObjectComponent.hpp"
#include "components/CCameraComponent.hpp"
#include "components/CEditableComponent.hpp"
#include "components/CMeshObjectComponent.hpp"
#include "components/CModelComponent.hpp"
#include "components/CParticleSystemComponent.hpp"
#include "components/CSkyboxComponent.hpp"
#include "components/CTransform3DComponent.hpp"
#include "components/CWindowComponent.hpp"

#include "app/scene/Mesh.hpp"

#include "resources/CRegistry.hpp"
#include "resources/resources.hpp"

#include "entities/windows/CEngineDebugWindow.hpp"
#include "entities/windows/CEntityManagerWindow.hpp"
#include "entities/windows/CFboDebugWindow.hpp"
#include "entities/windows/CShaderManagerWindow.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace
{
void checkOpenGLErrors()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::string message;
        switch (error)
        {
        case GL_INVALID_ENUM:
            message = "invalid enum passed to GL function (GL_INVALID_ENUM)";
            break;
        case GL_INVALID_VALUE:
            message = "invalid parameter passed to GL function (GL_INVALID_VALUE)";
            break;
        case GL_INVALID_OPERATION:
            message = "cannot execute some of GL functions in current state "
                      "(GL_INVALID_OPERATION)";
            break;
        case GL_STACK_OVERFLOW:
            message = "matrix stack overflow occurred inside GL (GL_STACK_OVERFLOW)";
            break;
        case GL_STACK_UNDERFLOW:
            message = "matrix stack underflow occurred inside GL "
                      "(GL_STACK_UNDERFLOW)";
            break;
        case GL_OUT_OF_MEMORY:
            message = "no enough memory to execute GL function (GL_OUT_OF_MEMORY)";
            break;
        default:
            message = "error in some GL extension (framebuffers, shaders, etc)";
            break;
        }

        spdlog::error("OpenGL error: {}", message.c_str());
    }
}

void GLAPIENTRY DebugOutputCallback(GLenum /*source*/, GLenum type, GLuint id, GLenum /*severity*/,
                                    GLsizei /*length*/, const GLchar* message,
                                    const void* /*userParam*/)
{
    // errors only
    if (type != GL_DEBUG_TYPE_ERROR)
    {
        return;
    }

    spdlog::error("OpenGL error #{}: {}", id, message);
}

void setupOpenGlDebug()
{
    gl::enable(GL_DEBUG_OUTPUT);
    gl::enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugOutputCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

} // namespace


CEngine::CEngine()
    : mMainWindow(nullptr)
    , mInputManager(nullptr)
    , m2dRenderSystem(nullptr)
    , m3dRenderSystem(nullptr)
    , mRotationUpdateSystem(nullptr)
    , mParticleUpdateSystem(nullptr)
    , mCullingSystem(nullptr)
    , mPickingSystem(nullptr)
    , mIsRunning(false)
    , mIsDebugMode(false)
    , mResourceManager(nullptr)
    , m_camera(new CFreeCamera())
{
    mSettings.mWindowSize = glm::ivec2(1920, 1080);
}

CEngine::~CEngine()
{
}

void CEngine::initialize()
{
    spdlog::debug("Engine.initialize()");

    initializeVideo();

    initializeInput();

    // Resource manager
    mResourceManager.reset(new CResourceManager());
    mResourceManager->initialize();

    // Textures

    // Engine settings
    mSettings.mVersion = {0, 4};

    mSettings.mBlend = false;
    mSettings.mCullFace = false;
    mSettings.mDepthMask = false;
    mSettings.mDepthTest = true;
    mSettings.mPolygonOffsetFill = false;

    mSettings.mPolygonMode.mIndex = 2;
    mSettings.mPolygonMode.mItems = {GL_POINT, GL_LINE, GL_FILL};
    mSettings.mPolygonMode.mItemsNames = {"GL_POINT", "GL_LINE", "GL_FILL"};

    spdlog::debug("settings.version = {}.{}", mSettings.mVersion.x, mSettings.mVersion.y);


    CRegistry::set("settings", &mSettings);
    CRegistry::set("mouse/position", mSettings.mWindowSize / 2);
}

void CEngine::initializeVideo()
{
    spdlog::debug("Engine.initializeVideo()");

    mMainWindow.reset(new CMainWindow());

    mMainWindow->setSize(mSettings.mWindowSize);
    mMainWindow->setTitle("wtf.e");
    mMainWindow->setFlags(SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

    std::map<SDL_GLattr, int> attributes = {
        {SDL_GL_CONTEXT_MAJOR_VERSION, 4},
        {SDL_GL_CONTEXT_MINOR_VERSION, 5},
        {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE},
        {SDL_GL_DOUBLEBUFFER, 1},
        {SDL_GL_ACCELERATED_VISUAL, 1},
        {SDL_GL_STENCIL_SIZE, 8},
        {SDL_GL_DEPTH_SIZE, 24},
        {SDL_GL_MULTISAMPLEBUFFERS, 1},
        {SDL_GL_MULTISAMPLESAMPLES, 8},
    };

    mMainWindow->setAttributes(attributes);
    mMainWindow->create();
    glEnable(GL_MULTISAMPLE);
}

void CEngine::initializeInput()
{
    spdlog::debug(" - input");
    mInputManager.reset(new CInputEventManager);

    mInputManager->addListener(new CEngineListener(*this));
    mInputManager->addListener(new CCameraListener(m_camera));
}

void CEngine::run()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Engine.run()");

    initialize();
    prepare();
    loop();
    finalize();
}

void CEngine::stop()
{
    mIsRunning = false;
}

void CEngine::prepare()
{
    srand(SDL_GetTicks());

    spdlog::debug("prepare: ");
    // auto skyboxTexture = resources::get_texture("resources/skybox/purple-nebula/4096",
    //                                             resources::ETextureType::CUBE_MAP_TEXTURE);
    auto skyboxTexture = resources::get_texture("resources/skybox/green-nebula/4096",
                                                resources::ETextureType::CUBE_MAP_TEXTURE);

    auto orange = resources::get_texture("resources/textures/orange.png",
                                         resources::ETextureType::TEXTURE_2D);

    CRegistry::set("camera", m_camera.get());
    CRegistry::set("texture/skybox", skyboxTexture);
    CRegistry::set("texture/orange", orange);

    CRegistry::set("dbg.tex.fbo.rgb", 0);
    CRegistry::set("dbg.tex.fbo.depth", 0);

    constexpr auto nbEntities = std::size_t(10000);
    mEntityManager.reserve(nbEntities);

    ImGui_ImplSdlGL3_Init(SDL_GetWindowFromID(mMainWindow->getId()));

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);

    m2dRenderSystem.reset(new C2DRenderSystem(mEntityManager));
    auto shader_manager = mResourceManager->get_shader_manager();
    m3dRenderSystem.reset(new C3DRenderSystem(mEntityManager, shader_manager));
    mCullingSystem.reset(new CCullingSystem(mEntityManager));
    mPickingSystem.reset(new CPickingSystem(mEntityManager));
    mRotationUpdateSystem.reset(new CRotationUpdateSystem(mEntityManager));
    mParticleUpdateSystem.reset(new CParticleUpdateSystem(mEntityManager));


    {
        auto model = resources::get_model("resources/models/cube/cube.obj", EModelType::STATIC);

        auto e = mEntityManager.createEntity();
        mEntityManager.addComponent<CEditableComponent>(e, "Skybox");
        mEntityManager.addComponent<CSkyboxComponent>(e);
        mEntityManager.addComponent<CModelComponent>(e, model);
    }

    {
        auto e = mEntityManager.createEntity();
        mEntityManager.addComponent<CEditableComponent>(e, "Sphere");

        auto& m = mEntityManager.addComponent<CMeshObjectComponent>(e);
        auto& o = mEntityManager.addComponent<C3dObjectComponent>(e);
        auto& t = mEntityManager.addComponent<CTransform3DComponent>(e);

        t.mScale = glm::vec3(5);
        t.mPosition = glm::vec3(0.f, 0.f, -50.f);
        t.mOrientation = glm::quat(glm::vec3(90.f, 0.f, 0.f));

        m.mMeshObject.reset(new CCubeSphere(40));
        o.isInCameraView = true;
    }


    {
        auto e = mEntityManager.createEntity();
        mEntityManager.addComponent<CEditableComponent>(e, "Camera");
        mEntityManager.addComponent<CCameraComponent>(e, m_camera);
    }


    {
        auto e = mEntityManager.createEntity();
        mEntityManager.addComponent<CEditableComponent>(e, "Model");
        auto& t = mEntityManager.addComponent<CTransform3DComponent>(e);
        auto& o = mEntityManager.addComponent<C3dObjectComponent>(e);

        o.isInCameraView = true;

        t.mScale = glm::vec3(1);
        t.mPosition = glm::vec3(10.f, 0.f, -20.f);
        // t.mOrientation = glm::quat(glm::vec3(90.f, 0.f, 0.f));

        // auto model = resources::get_model("resources/models/rock/rock.obj", EModelType::STATIC);
        auto model = resources::get_model("trash/nanosuit/nanosuit.obj", EModelType::STATIC);
        mEntityManager.addComponent<CModelComponent>(e, model);
    }

    {
        auto e = mEntityManager.createEntity();
        auto& w = mEntityManager.addComponent<CWindowComponent>(e);
        w.mWindow = std::make_shared<CEngineDebugWindow>(m_camera);
    }

    {
        auto e = mEntityManager.createEntity();
        auto& w = mEntityManager.addComponent<CWindowComponent>(e);
        w.mWindow = std::make_shared<CShaderManagerWindow>(shader_manager);
    }

    {
        auto e = mEntityManager.createEntity();
        auto& w = mEntityManager.addComponent<CWindowComponent>(e);
        w.mWindow = std::make_shared<CEntityManagerWindow>(mEntityManager);
    }


    {
        auto e = mEntityManager.createEntity();
        mEntityManager.addComponent<CEditableComponent>(e, "ParticleSystem");

        auto& p = mEntityManager.addComponent<CParticleSystemComponent>(e);
        auto& t = mEntityManager.addComponent<CTransform3DComponent>(e);

        t.mScale = glm::vec3(1);
        t.mPosition = glm::vec3(0.f, 0.f, -50.f);
        t.mOrientation = glm::quat(glm::vec3(0));

        auto system = std::make_shared<CParticleSystem>();
        p.mParticleSystem = system;

        system->setGravity(glm::vec3(0));
        system->setParticleTexture(orange);

        auto createEmitter = []() -> std::shared_ptr<CParticleEmitter> {
            auto emitter = std::make_shared<CParticleEmitter>();
            emitter->setPosition(glm::vec3(0, 0, 0));
            emitter->setDirection(glm::vec3(0, -1, 0));
            emitter->setMaxDeviationAngle(3.14f);
            emitter->setDistanceRange(5, 5.1);
            emitter->setEmitIntervalRange(0.0003, 0.0004);
            emitter->setLifetimeRange(0.3, 0.4);
            emitter->setSpeedRange(1.0, 1.1);

            return emitter;
        };

        p.m_particle_emitter = createEmitter();

        system->set_emitter(p.m_particle_emitter);
    }
}

void CEngine::loop()
{
    mIsRunning = true;
    mIsDebugMode = true;

    if (mIsDebugMode)
    {
        setupOpenGlDebug();
    }

    while (mIsRunning)
    {
        // EVENTS
        onEvent();

        // UPDATE
        onUpdate(mChronometer.getDelta());

        // CLEAR
        // onClear();

        // DRAW
        onDraw();

        // CHECK
        checkOpenGLErrors();

        // SWAP BUFFERS
        onSwapBuffers();

        // WAIT
        mChronometer.wait(16UL /* 1000 / 16 ≈ 60 FPS */);
    }
}

void CEngine::onSwapBuffers()
{
    mMainWindow->swapBuffers();
}

void CEngine::finalize()
{
    ImGui_ImplSdlGL3_Shutdown();
}

void CEngine::onEvent()
{
    mInputManager->handle();
}

void CEngine::onUpdate(double delta)
{
    const auto& projection = m_camera->get_projection();
    const auto& view = m_camera->get_view();

    mCullingSystem->setProjectionMatrix(projection);
    mCullingSystem->setViewMatrix(view);

    mCullingSystem->update(delta);
    mPickingSystem->update(delta);
    mParticleUpdateSystem->update(delta);

    // add ignore rotation on invisible objects
    mRotationUpdateSystem->update(delta);

    m_camera->update(delta);
}

void CEngine::onClear()
{
    gl::clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CEngine::onDraw()
{
    const glm::mat4 view = m_camera->get_view();
    const glm::mat4 projection = m_camera->get_projection();

    m3dRenderSystem->prepare(m_camera.get());

    m3dRenderSystem->render(view, projection);

    ImGui_ImplSdlGL3_NewFrame(SDL_GetWindowFromID(mMainWindow->getId()));
    m2dRenderSystem->render(view, projection);
    ImGui::Render();
}
