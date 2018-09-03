
#include "CEngine.hpp"

#include "core/auxiliary/imgui.hpp"
#include "core/resources/CRegistry.hpp"

#include "core/shading/CShaderProgram.hpp"

#include "app/input/CCameraListener.hpp"
#include "app/input/CEngineListener.hpp"
#include "core/resources/CResourceLoader.hpp"

#include "components/CMeshComponent.hpp"

#include "core/resources/CStaticModelLoader.hpp"

#include "core/scene/CArrayObject.hpp"

#include "entities/windows/CEngineDebugWindow.hpp"
#include "entities/windows/CEngineSettingsWindow.hpp"

#include "core/auxiliary/trace.hpp"

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
    : mMainWindow(nullptr)
    , mChronometer(nullptr)
    , mInputManager(nullptr)
    , mIsRunning(false)
    , mIsDebugMode(false)
    , m2dRenderSystem()
    , m3dRenderSystem()
    , mCamera()
{
}

CEngine::~CEngine()
{
    delete m2dRenderSystem;
    delete m3dRenderSystem;
    delete mChronometer;
    delete mMainWindow;
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
}

void CEngine::initializeVideo()
{
    trc_debug(" - video");
    mMainWindow = new CMainWindow(
        "F.O.S.S. (0.1.2)",
        {1366, 768},
        SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
    );

    CMainWindow::setGlAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    CMainWindow::setGlAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    CMainWindow::setGlAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    CMainWindow::setGlAttribute(SDL_GL_DOUBLEBUFFER, 1);
    CMainWindow::setGlAttribute(SDL_GL_DOUBLEBUFFER, 1);
    CMainWindow::setGlAttribute(SDL_GL_STENCIL_SIZE, 8);
    CMainWindow::setGlAttribute(SDL_GL_DEPTH_SIZE, 24);

    CMainWindow::setGlAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    CMainWindow::setGlAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    mMainWindow->createGlContext();
}

void CEngine::initializeInput()
{
    trc_debug(" - input");
    mInputManager = new CInputEventManager;

    mInputManager->addListener(new CEngineListener(*this));
    mInputManager->addListener(new CCameraListener(&mCamera));
}

void CEngine::run()
{
    auto * vao = new CArrayObject();
    vao->bind();
    prepare();
    loop();
    finalize();
    vao->unbind();
    delete vao;
}

void CEngine::stop()
{
    mIsRunning = false;
}

void CEngine::prepare()
{
    trc_debug("prepare: ");
    auto skyboxTexture = mResourceLoader->getCubeMap("resources/skybox/purple-nebula", 4096);

    CRegistry::set("camera", &mCamera);

    CRegistry::set("texture/purple-nebula/4096", skyboxTexture);
    CRegistry::set("texture/skybox", skyboxTexture);    

    ImGui_ImplSdlGL3_Init(SDL_GetWindowFromID(mMainWindow->getId()));

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);

    m2dRenderSystem = new C2DRenderSystem();
    m3dRenderSystem = new C3DRenderSystem();

    mWorld.addSystem(*m2dRenderSystem);
    mWorld.addSystem(*m3dRenderSystem);

    CStaticModelLoader modelLoader(*mResourceLoader);
    auto rockModel = modelLoader.load("resources/models/rock/rock.obj");

    anax::Entity sbx = mWorld.createEntity();
    auto &mc1 = sbx.addComponent<CMeshComponent>();
    auto &tc1 = sbx.addComponent<CTransform3DComponent>();
    mc1.mCategory = CMeshComponent::ECategory::eEnvironment;
    mc1.mModel = modelLoader.load("resources/models/cube/cube.obj");

    sbx.activate();

    anax::Entity rock = mWorld.createEntity();
    auto &mc2 = rock.addComponent<CMeshComponent>();
    auto &tc2 = rock.addComponent<CTransform3DComponent>();

    tc2.mScale = glm::vec3(0.1f);
    tc2.mPosition = glm::vec3(1.f, 1.f, 1.f);
    tc2.mOrientation = glm::quat(glm::vec3(0.f, 0.f, 0.f));

    mc2.mCategory = CMeshComponent::ECategory::eForeground;
    mc2.mModel = rockModel;
    rock.activate();


    anax::Entity debugWindow = mWorld.createEntity();
    auto & dw = debugWindow.addComponent<CWindowComponent>();
    dw.mWindow = std::make_shared<CEngineDebugWindow>(mCamera);
    debugWindow.activate();

    anax::Entity settingsWindow = mWorld.createEntity();
    auto & sw = settingsWindow.addComponent<CWindowComponent>();
    sw.mWindow = std::make_shared<CEngineSettingsWindow>(mSettings);
    settingsWindow.activate();


    unsigned int amount = 1000;

    glm::mat4* modelMatrices;

    modelMatrices = new glm::mat4[amount];

    srand(SDL_GetTicks()); 

    float radius = 30.0;
    float offset = 20.f;

    for (unsigned int i = 0; i < amount; i++)  {
        anax::Entity entity = mWorld.createEntity();

        auto &m = entity.addComponent<CMeshComponent>();
        m.mCategory = CMeshComponent::ECategory::eForeground;
        m.mModel = rockModel;

        auto &t = entity.addComponent<CTransform3DComponent>();

        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; 
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;


        t.mPosition = glm::vec3(x, y, z);
        
        float scale = (rand() % 8) / 100.0f + 0.005;
        t.mScale = glm::vec3(scale);

        float rotAngle = (rand() % 360);
        t.mOrientation = glm::angleAxis(rotAngle, glm::vec3(0, 1, 0));

        entity.activate();
    }

    mWorld.refresh();
}

void CEngine::loop()
{
    mIsRunning = true;
    mIsDebugMode = true;

    if (mIsDebugMode)
    {
        setupOpenGlDebug();
    }

    mChronometer = new CChronometer;

    while (mIsRunning)
    {
        // EVENTS
         onEvent();

        // UPDATE
        onUpdate(mChronometer->getDelta());

        // CLEAR
        onClear();

        // DRAW
        onDraw();

        // CHECK
        checkOpenGLErrors();

        // SWAP BUFFERS
        mMainWindow->swapBuffers();

        // WAIT
        mChronometer->wait(16UL /* 1000 / 16 ≈ 60 FPS */);
    }
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
