
#include "CPickingSystem.hpp"
#include "app/auxiliary/trace.hpp"

#include "app/components/CModelComponent.hpp"
#include "app/components/CPickingComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"

#include "app/resources/CRegistry.hpp"
#include "app/scene/CFreeCamera.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


void ScreenPosToWorldRay(
    int mouseX, int mouseY, // Mouse position, in pixels, from bottom-left corner of the window
    int screenWidth, int screenHeight, // Window size, in pixels
    glm::mat4 ViewMatrix,              // Camera position and orientation
    glm::mat4 ProjectionMatrix, // Camera parameters (ratio, field of view, near and far planes)
    glm::vec3& out_origin,   // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you
                             // want the ray to start at the camera's position instead, ignore this.
    glm::vec3& out_direction // Ouput : Direction, in world space, of the ray that goes "through"
                             // the mouse.
)
{

    // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4
    // ?)
    glm::vec4 lRayStart_NDC(
        ((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,  // [0,1024] -> [-1,1]
        ((float)mouseY / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
        -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
        1.0f);
    glm::vec4 lRayEnd_NDC(((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
                          ((float)mouseY / (float)screenHeight - 0.5f) * 2.0f, 0.0, 1.0f);


    // // The Projection matrix goes from Camera Space to NDC.
    // // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
    // glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

    // // The View Matrix goes from World Space to Camera Space.
    // // So inverse(ViewMatrix) goes from Camera Space to World Space.
    // glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

    // glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;
    // lRayStart_camera/=lRayStart_camera.w; glm::vec4 lRayStart_world  = InverseViewMatrix       *
    // lRayStart_camera; lRayStart_world /=lRayStart_world .w; glm::vec4 lRayEnd_camera   =
    // InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera  .w; glm::vec4
    // lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world /=lRayEnd_world
    // .w;


    // Faster way (just one inverse)
    glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
    glm::vec4 lRayStart_world = M * lRayStart_NDC;
    lRayStart_world /= lRayStart_world.w;
    glm::vec4 lRayEnd_world = M * lRayEnd_NDC;
    lRayEnd_world /= lRayEnd_world.w;

    glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
    lRayDir_world = glm::normalize(lRayDir_world);

    out_origin = glm::vec3(lRayStart_world);
    out_direction = glm::normalize(lRayDir_world);
}


bool testRayIntersect(glm::vec3 rayOriginWorld, glm::vec3 rayDirectionWorld, glm::vec3 min_bb,
                      glm::vec3 max_bb, const glm::mat4 transformation, float& intersection)
{
    const auto rayOrigin = glm::inverse(transformation) * glm::vec4(rayOriginWorld, 1.f);
    const auto rayDirection = glm::mat3(glm::inverse(transformation)) * rayDirectionWorld;

    float t1 = (min_bb.x - rayOrigin.x) / rayDirection.x;
    float t2 = (max_bb.x - rayOrigin.x) / rayDirection.x;
    float t3 = (min_bb.y - rayOrigin.y) / rayDirection.y;
    float t4 = (max_bb.y - rayOrigin.y) / rayDirection.y;
    float t5 = (min_bb.z - rayOrigin.z) / rayDirection.z;
    float t6 = (max_bb.z - rayOrigin.z) / rayDirection.z;

    float t_min = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
    float t_max = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

    if (t_max < 0)
    {
        intersection = t_max;
        return false;
    }

    if (t_min > t_max)
    {
        intersection = t_max;
        return false;
    }

    intersection = t_min;
    return true;
}


CPickingSystem::CPickingSystem(ecs::EntityManager& entityManager)
    : mEntityManager(entityManager)
{
}

void CPickingSystem::update(double& delta)
{
    auto camera = CRegistry::get<CFreeCamera*>("camera");
    auto mouse = CRegistry::get<glm::ivec2>("mouse/position");

    glm::mat4 projection = camera->get_projection();
    glm::mat4 view = camera->get_view();

    const int screen_w = 1920;
    const int screen_h = 1080;

    glm::vec3 ray_origin;
    glm::vec3 ray_direction;
    // ScreenPosToWorldRay(mouse.x, mouse.y, screen_w, screen_h, view, projection, ray_origin,
    // ray_direction);
    ScreenPosToWorldRay(screen_w / 2, screen_h / 2, screen_w, screen_h, view, projection,
                        ray_origin, ray_direction);


    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, CPickingComponent, CTransform3DComponent>())
    {
        auto [model, picking, transform] = components;

        if (model.mIsInView)
        {
            if (const auto aabb = model.mModel->getBoundingBox())
            {
                float intersection = 0.f;
                const auto& bounds = aabb->getBounds<glm::vec3>();

                picking.isPicked =
                    testRayIntersect(ray_origin, ray_direction, bounds.mMin, bounds.mMax,
                                     transform.toMat4(), picking.intersection);
            }
        }
    }
}
