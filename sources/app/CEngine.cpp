
#include "CEngine.hpp"

#include "app/auxiliary/imgui.hpp"
#include "resources/CRegistry.hpp"

#include "app/input/CCameraListener.hpp"
#include "app/input/CEngineListener.hpp"
#include "resources/CResourceLoader.hpp"

#include "components/C3DModelComponent.hpp"
#include "components/CWindowComponent.hpp"
#include "components/CSkyboxComponent.hpp"
#include "components/C3dObjectComponent.hpp"
#include "components/CTransform3DComponent.hpp"
#include "components/CMeshObjectComponent.hpp"
#include "components/CParticleSystemComponent.hpp"

#include "app/geometry/CCubeSphere.hpp"
#include "resources/CStaticModelLoader.hpp"

#include "entities/windows/CFboDebugWindow.hpp"
#include "entities/windows/CEngineDebugWindow.hpp"
#include "entities/windows/CShaderManagerWindow.hpp"
#include "entities/windows/CEngineSettingsWindow.hpp"

#include "app/auxiliary/trace.hpp"

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
                message = "cannot execute some of GL functions in current state (GL_INVALID_OPERATION)";
                break;
            case GL_STACK_OVERFLOW:
                message = "matrix stack overflow occurred inside GL (GL_STACK_OVERFLOW)";
                break;
            case GL_STACK_UNDERFLOW:
                message = "matrix stack underflow occurred inside GL (GL_STACK_UNDERFLOW)";
                break;
            case GL_OUT_OF_MEMORY:
                message = "no enough memory to execute GL function (GL_OUT_OF_MEMORY)";
                break;
            default:
                message = "error in some GL extension (framebuffers, shaders, etc)";
                break;
        }
        trc_error("OpenGL error: %s", message.c_str());
    }
}

void GLAPIENTRY DebugOutputCallback(
    GLenum /*source*/,
    GLenum type,
    GLuint id,
    GLenum /*severity*/,
    GLsizei /*length*/,
    const GLchar* message,
    const void* /*userParam*/)
{
    // errors only
    if (type != GL_DEBUG_TYPE_ERROR)
    {
        return;
    }
    std::string formatted =
        "OpenGL error #" + std::to_string(id) + ": " + message;
    std::cerr << formatted << std::endl;
}

void setupOpenGlDebug()
{
    gl::Enable(GL_DEBUG_OUTPUT);
    gl::Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugOutputCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

}


CEngine::CEngine()
    : mResourceLoader(nullptr)
    , mMainWindow(nullptr)
    , mInputManager(nullptr)
    , m2dRenderSystem(nullptr)
    , m3dRenderSystem(nullptr)
    , mRotationUpdateSystem(nullptr)
    , mParticleUpdateSystem(nullptr)
    , mCullingSystem(nullptr)
    , mPickingSystem(nullptr)
    , mIsRunning(false)
    , mIsDebugMode(false)
    , mShaderManager(nullptr)
{
    mSettings.mWindowSize = glm::ivec2(1920, 1080);
}

CEngine::~CEngine()
{
}

void CEngine::initialize()
{
    trc_debug("Initialize:");
    initializeVideo();
    initializeInput();

    mShaderManager.reset(new CShaderManager("resources/shaders"));

    mSettings.mBlend = false;
    mSettings.mCullFace = false;
    mSettings.mDepthMask = false;
    mSettings.mDepthTest = true;
    mSettings.mPolygonOffsetFill = false;

    mSettings.mPolygonMode.mIndex = 2;
    mSettings.mPolygonMode.mItems = {GL_POINT, GL_LINE, GL_FILL};
    mSettings.mPolygonMode.mItemsNames = {"GL_POINT", "GL_LINE", "GL_FILL"};


    CRegistry::set("settings", &mSettings);
    CRegistry::set("mouse/position", mSettings.mWindowSize / 2);

    // Load shaders
    mShaderManager->initialize();
}

void CEngine::initializeVideo()
{
    trc_debug(" - video");

    mMainWindow.reset(new CMainWindow);

    mMainWindow->setSize(mSettings.mWindowSize);
    mMainWindow->setTitle("F.O.S.S (0.3.5)");
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
        {SDL_GL_MULTISAMPLESAMPLES, 8}
    };

    mMainWindow->setAttributes(attributes);
    mMainWindow->create();
    glEnable(GL_MULTISAMPLE);
}

void CEngine::initializeInput()
{
    trc_debug(" - input");
    mInputManager.reset(new CInputEventManager);

    mInputManager->addListener(new CEngineListener(*this));
    mInputManager->addListener(new CCameraListener(&mCamera));
}

void CEngine::run()
{
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

    trc_debug("prepare: ");
    auto skyboxTexture = mResourceLoader->getCubeMap("resources/skybox/purple-nebula", 1024 * 4);

    CRegistry::set("camera", &mCamera);
    CRegistry::set("texture/skybox", skyboxTexture);

    CRegistry::set("dbg.tex.fbo.rgb", 0);
    CRegistry::set("dbg.tex.fbo.depth", 0);

    constexpr auto nbEntities = std::size_t(10000);
    mEntityManager.reserve(nbEntities);

    ImGui_ImplSdlGL3_Init(SDL_GetWindowFromID(mMainWindow->getId()));

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);

    m2dRenderSystem.reset(new C2DRenderSystem(mEntityManager));
    m3dRenderSystem.reset(new C3DRenderSystem(mEntityManager, *mShaderManager.get()));
    mCullingSystem.reset(new CCullingSystem(mEntityManager));
    mPickingSystem.reset(new CPickingSystem(mEntityManager));
    mRotationUpdateSystem.reset(new CRotationUpdateSystem(mEntityManager));
    mParticleUpdateSystem.reset(new CParticleUpdateSystem(mEntityManager));


    CStaticModelLoader modelLoader(*mResourceLoader);
    auto rockModel = modelLoader.load("resources/models/rock/rock.obj");
    auto cubeModel = modelLoader.load("resources/models/cube/cube.obj");


    auto skybox = mEntityManager.createEntity();
    auto &mc1 = mEntityManager.addComponent<C3DModelComponent>(skybox);
    mEntityManager.addComponent<CSkyboxComponent>(skybox);

    mc1.mModel = cubeModel;

    auto rock = mEntityManager.createEntity();
    auto &mc2 = mEntityManager.addComponent<C3DModelComponent>(rock);
    auto &dc2 = mEntityManager.addComponent<C3dObjectComponent>(rock);
    auto &tc2 = mEntityManager.addComponent<CTransform3DComponent>(rock);

    tc2.mScale = glm::vec3(2);
    tc2.mPosition = glm::vec3(-10.f, 0.f, -10.f);
    tc2.mOrientation = glm::quat(glm::vec3(1.f, 2.f, 3.f));

    mc2.mModel = rockModel;
    dc2.isInCameraView = false;

    auto meshObject = mEntityManager.createEntity();
    auto &mc3 = mEntityManager.addComponent<CMeshObjectComponent>(meshObject);
    auto &dc3 = mEntityManager.addComponent<C3dObjectComponent>(meshObject);
    auto &tc3 = mEntityManager.addComponent<CTransform3DComponent>(meshObject);


    tc3.mScale = glm::vec3(5);
    tc3.mPosition = glm::vec3(0.f, 0.f, -20.f);
    tc3.mOrientation = glm::quat(glm::vec3(90.f, 0.f, 0.f));

    auto sphere = new CCubeSphere(40);

    mc3.mMeshObject.reset(sphere);
    dc3.isInCameraView = true;



    auto debugWindow = mEntityManager.createEntity();
    auto &window1 = mEntityManager.addComponent<CWindowComponent>(debugWindow);
    window1.mWindow = std::make_shared<CEngineDebugWindow>(mCamera);

    auto settingsWindow = mEntityManager.createEntity();
    auto &window2 = mEntityManager.addComponent<CWindowComponent>(settingsWindow);
    window2.mWindow = std::make_shared<CEngineSettingsWindow>(mSettings, mCamera);

    // auto fboDebug = mEntityManager.createEntity();
    // auto &window3 = mEntityManager.addComponent<CWindowComponent>(fboDebug);
    // window3.mWindow = std::make_shared<CFboDebugWindow>();

    auto shaderManager = mEntityManager.createEntity();
    auto &window4 = mEntityManager.addComponent<CWindowComponent>(shaderManager);
    window4.mWindow = std::make_shared<CShaderManagerWindow>(*mShaderManager.get());

    // auto asteroids = new CInstancedAsteroidField(mEntityManager);
    // asteroids->setupModel(rockModel);
    // asteroids->setupTransform(glm::vec3(1.f, 1.f, 10.f), glm::vec3(1.f), glm::quat(glm::vec3(0.f, 0.f, 0.f)));

    auto particleSystem = mEntityManager.createEntity();
    auto &psComponent = mEntityManager.addComponent<CParticleSystemComponent>(particleSystem);
    auto &psTransform = mEntityManager.addComponent<CTransform3DComponent>(particleSystem);

    auto pSystem = std::make_shared<CParticleSystem>();

    psTransform.mScale = glm::vec3(0.5);
    psTransform.mPosition = glm::vec3(0,0,0);
    psTransform.mOrientation = glm::quat(glm::vec3(0));
    pSystem->setGravity(-glm::vec3(0, 0, 0));

    // psTransform.mScale = glm::vec3(0.7);
    // psTransform.mPosition = glm::vec3(-12, -2, 0);
    // psTransform.mOrientation = glm::quat(glm::vec3(0));
    // pSystem->setGravity(glm::vec3(0, -0.2, 0));


    pSystem->setParticleTexture(CResourceLoader::getTexture("resources/textures/orange.png"));

    auto createEmitter = []() -> std::unique_ptr<CParticleEmitter> {
        auto pEmitter = std::make_unique<CParticleEmitter>();
        pEmitter->setPosition(glm::vec3(0, 0, 0));
        pEmitter->setDirection(glm::vec3(0, -1, 0));
        pEmitter->setMaxDeviationAngle(3.14f);
        pEmitter->setDistanceRange(10, 10.1);
        pEmitter->setEmitIntervalRange(0.0003, 0.0004);
        pEmitter->setLifetimeRange(0.8, 0.9);
        pEmitter->setSpeedRange(0.5, 0.6);

        // pEmitter->setPosition(glm::vec3(0, 0, 0));
        // pEmitter->setDirection(glm::vec3(1, -0.5, 0));
        // pEmitter->setMaxDeviationAngle(0.3);
        // pEmitter->setDistanceRange(0, 2);
        // pEmitter->setEmitIntervalRange(0.003, 0.005);
        // pEmitter->setLifetimeRange(2, 3);
        // pEmitter->setSpeedRange(8, 10);

        return pEmitter;
    };


    pSystem->setEmitter(createEmitter());


    psComponent.mParticleSystem = pSystem;


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
    const auto &projection = mCamera.getProjection();
    const auto &view = mCamera.getView();

    mCullingSystem->setProjectionMatrix(projection);
    mCullingSystem->setViewMatrix(view);

    mCullingSystem->update(delta);
    mPickingSystem->update(delta);
    mParticleUpdateSystem->update(delta);

    // add ignore rotation on invisible objects
    mRotationUpdateSystem->update(delta);

    mCamera.update(delta);
}

void CEngine::onClear()
{
    gl::ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CEngine::onDraw()
{
    const glm::mat4 view = mCamera.getView();
    const glm::mat4 projection = mCamera.getProjection();

    m3dRenderSystem->prepare(&mCamera);

    m3dRenderSystem->render(view, projection);

    ImGui_ImplSdlGL3_NewFrame(SDL_GetWindowFromID(mMainWindow->getId()));
    m2dRenderSystem->render(view, projection);
    ImGui::Render();
}
