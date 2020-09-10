
#include "CEngine.hpp"

#include "base/auxiliary/imgui.hpp"
#include "base/resources/CRegistry.hpp"

#include "base/shading/CShaderProgram.hpp"

#include "app/input/CCameraListener.hpp"
#include "app/input/CEngineListener.hpp"
#include "base/resources/CResourceLoader.hpp"

#include "components/CMeshComponent.hpp"
#include "components/CWindowComponent.hpp"
#include "components/CSkyboxComponent.hpp"
#include "components/C3dObjectComponent.hpp"
#include "components/CTransform3DComponent.hpp"

#include "base/resources/CStaticModelLoader.hpp"

#include "base/scene/CArrayObject.hpp"

#include "entities/windows/CEngineDebugWindow.hpp"
#include "entities/windows/CEngineSettingsWindow.hpp"
#include "entities/3d_objects/CInstancedAsteroidField.hpp"

#include "base/auxiliary/trace.hpp"

#include "base/geometry/CSimpleGeometry.hpp"

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
        throw std::runtime_error("OpenGL error: " + message);
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
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
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
    , mCullingSystem(nullptr)
    , mPickingSystem(nullptr)
    , mIsRunning(false)
    , mIsDebugMode(false)
{
}

CEngine::~CEngine()
{
}

void CEngine::initialize()
{
    trc_debug("Initialize:");
    initializeVideo();
    initializeInput();

    mSettings.mBlend = false;
    mSettings.mCullFace = false;
    mSettings.mDepthMask = false;
    mSettings.mDepthTest = true;
    mSettings.mPolygonOffsetFill = false;

    mSettings.mPolygonMode.mIndex = 2;
    mSettings.mPolygonMode.mItems = {GL_POINT, GL_LINE, GL_FILL};
    mSettings.mPolygonMode.mItemsNames = {"GL_POINT", "GL_LINE", "GL_FILL"};


    CRegistry::set("settings", &mSettings);
    CRegistry::set("mouse/position", glm::ivec2(1920 / 2, 1080 / 2));
}

void CEngine::initializeVideo()
{
    trc_debug(" - video");

    mMainWindow.reset(new CMainWindow);

    mMainWindow->setSize({1920, 1080});
    mMainWindow->setTitle("F.O.S.S (0.3.5)");
    mMainWindow->setFlags(SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

    std::map<SDL_GLattr, int> attributes = {
        {SDL_GL_CONTEXT_MAJOR_VERSION, 4},
        {SDL_GL_CONTEXT_MINOR_VERSION, 5},
        {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE},
        {SDL_GL_DOUBLEBUFFER, 1},
        {SDL_GL_STENCIL_SIZE, 8},
        {SDL_GL_DEPTH_SIZE, 24},
        {SDL_GL_MULTISAMPLEBUFFERS, 1},
        {SDL_GL_MULTISAMPLESAMPLES, 4}
    };

    mMainWindow->setAttributes(attributes);
    mMainWindow->create();
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
    trc_debug("prepare: ");
    auto skyboxTexture = mResourceLoader->getCubeMap("resources/skybox/purple-nebula", 1024 * 4);

    CRegistry::set("camera", &mCamera);
    CRegistry::set("texture/skybox", skyboxTexture);

    constexpr auto nbEntities = std::size_t(10000);
    mEntityManager.reserve(nbEntities);

    ImGui_ImplSdlGL3_Init(SDL_GetWindowFromID(mMainWindow->getId()));

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);

    m2dRenderSystem.reset(new C2DRenderSystem(mEntityManager));
    m3dRenderSystem.reset(new C3DRenderSystem(mEntityManager));
    mCullingSystem.reset(new CCullingSystem(mEntityManager));
    mPickingSystem.reset(new CPickingSystem(mEntityManager));
    mRotationUpdateSystem.reset(new CRotationUpdateSystem(mEntityManager));


    CStaticModelLoader modelLoader(*mResourceLoader);
    auto rockModel = modelLoader.load("resources/models/rock/rock.obj");
    auto cubeModel = modelLoader.load("resources/models/cube/cube.obj");;


    auto skybox = mEntityManager.createEntity();
    auto &mc1 = mEntityManager.addComponent<CMeshComponent>(skybox);
    mEntityManager.addComponent<CSkyboxComponent>(skybox);

    mc1.mModel = cubeModel;

    auto rock = mEntityManager.createEntity();
    auto &mc2 = mEntityManager.addComponent<CMeshComponent>(rock);
    auto &dc2 = mEntityManager.addComponent<C3dObjectComponent>(rock);
    auto &tc2 = mEntityManager.addComponent<CTransform3DComponent>(rock);

    tc2.mScale = glm::vec3(0.1);
    tc2.mPosition = glm::vec3(0.f, 0.f, 0.f);
    // tc2.mOrientation = glm::quat(glm::vec3(1.f, 2.f, 3.f));
    

    mc2.mModel = rockModel;
    dc2.isInCameraView = true;

    auto rock2 = mEntityManager.createEntity();
    auto &mc3 = mEntityManager.addComponent<CMeshComponent>(rock2);
    auto &dc3 = mEntityManager.addComponent<C3dObjectComponent>(rock2);
    auto &tc3 = mEntityManager.addComponent<CTransform3DComponent>(rock2);

    tc3.mScale = glm::vec3(0.1);
    tc3.mPosition = glm::vec3(3.f, 0.f, 0.f);
    tc3.mOrientation = glm::quat(glm::vec3(90.f, 0.f, 0.f));
    

    mc3.mModel = rockModel;
    dc3.isInCameraView = true;


    auto debugWindow = mEntityManager.createEntity();
    auto &window1 = mEntityManager.addComponent<CWindowComponent>(debugWindow);
    window1.mWindow = std::make_shared<CEngineDebugWindow>(mCamera);

    auto settingsWindow = mEntityManager.createEntity();
    auto &window2 = mEntityManager.addComponent<CWindowComponent>(settingsWindow);
    window2.mWindow = std::make_shared<CEngineSettingsWindow>(mSettings, mCamera);

    // auto asteroids = new CInstancedAsteroidField(mEntityManager);
    // asteroids->setupModel(rockModel);
    // asteroids->setupTransform(glm::vec3(1.f, 1.f, 10.f), glm::vec3(1.f), glm::quat(glm::vec3(0.f, 0.f, 0.f)));


    srand(SDL_GetTicks());

    unsigned int amount = 1000 /* * 2 */;
    float radius = 200.0;
    float offset = 50.f;
    
    glm::mat4* modelMatrices = new glm::mat4[amount];

    for (unsigned int i = 0; i < amount; i++)  {
        auto entity = mEntityManager.createEntity();

        auto &mesh = mEntityManager.addComponent<CMeshComponent>(entity);
        auto &object = mEntityManager.addComponent<C3dObjectComponent>(entity);
        auto &transform = mEntityManager.addComponent<CTransform3DComponent>(entity);

        mesh.mModel = rockModel;
        object.isInCameraView = true;

        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;

        transform.mPosition = glm::vec3(x, y, z);

        float scale = (rand() % 8) / 100.0f + 0.005;
        transform.mScale = glm::vec3(scale);

        transform.mOrientation = glm::angleAxis(static_cast<float>(rand() % 360), glm::vec3(0, 1, 0));
        transform.mOrientation = glm::rotate(transform.mOrientation, static_cast<float>(rand() % 360), glm::vec3(1, 0, 0));
        transform.mOrientation = glm::rotate(transform.mOrientation, static_cast<float>(rand() % 360), glm::vec3(0, 0, 1));
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
        onClear();

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

    // add ignore rotation on invisible objects
    mRotationUpdateSystem->update(delta);

    mCamera.update(delta);
}

void CEngine::onClear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CEngine::onDraw()
{
    const glm::mat4 view = mCamera.getView();
    const glm::mat4 projection = mCamera.getProjection();

    m3dRenderSystem->render(view, projection);

    ImGui_ImplSdlGL3_NewFrame(SDL_GetWindowFromID(mMainWindow->getId()));
    m2dRenderSystem->render(view, projection);
    ImGui::Render();
}
