#include "dsMath.h"
#include "dsLog.h"
#include "dsOpenGlRenderer.h"
#include "dsShowTextureRenderPass.h"
#include "dsScene.h"
#include "dsSceneUpdater.h"
#include "dsCubeObject.h"
#include "dsMeshObject.h"
#include "dsPointLight.h"
#include "dsSpotLight.h"
#include "dsMesh.h"
#include "dsCamera.h"
#include "dsTransformController.h"
#include "dsSelectorRenderPass.h"
#include "dsGeometryRenderPass.h"
#include "dsShowTextureRenderPass.h"
#include "dsAmbientLightingRenderPass.h"
#include "dsPointLightingRenderPass.h"
#include "dsSpotLightingRenderPass.h"
#include "dsFrameBuffer.h"
#include "dsEngine.h"

using namespace ds;

const int c_viewportWidth = 1280;
const int c_viewportHeight = 800;

int main(int argc, char *argv[])
{
    ds::Log::setReportingLevel(ds::Log::Level_Debug);

    Engine *pEngine = Engine::instance();
    OpenGlRenderer *pRenderer = new OpenGlRenderer();
    pEngine->setRenderer(pRenderer);

    Scene *pScene = new Scene();
    SceneUpdater *pSceneUpdater = new SceneUpdater(pScene);
    pEngine->registerUpdatable(pSceneUpdater);

    //CubeObject *pCube = new CubeObject();
    //pScene->addNode(pCube);

    Mesh *pTeapot = Mesh::loadFromObj("teapot", "teapot.obj");
    ResourceHandle hTeapot(pTeapot);
    MeshObject *pTeapotObject = new MeshObject(hTeapot);
    pTeapotObject->scale(glm::vec3(0.25f, 0.25f, 0.25f));
    pTeapotObject->translate(glm::vec3(0.0f, 0.0f, 0.0f));
    pScene->addNode(pTeapotObject);

    CubeObject *pFloor = new CubeObject();
    pFloor->scale(glm::vec3(10.0f, 10.0f, 0.1f));
    pFloor->translate(glm::vec3(0.0f, 0.0f, -10.0f));
    pScene->addNode(pFloor);

    TransformController *pCtrl = new TransformController();
    pCtrl->setTransformStep(glm::rotate(DS_DEG(0.3f), glm::vec3(0.0f, 0.0f, 1.0f)));
    pCtrl->attachToObject(pTeapotObject);

    pCtrl = new TransformController();
    pCtrl->setTransformStep(glm::rotate(DS_DEG(0.43f), glm::vec3(1.0f, 0.0f, 0.0f)));
    pCtrl->attachToObject(pTeapotObject);

    pScene->setAmbientLightColor(glm::vec3(0.1f, 0.1f, 0.1f));


    PointLight *pLight = new PointLight();
    pLight->setColor(glm::vec3(0.6f, 0.0f, 0.0f));
    pLight->translate(glm::vec3(-1.5f, 0.0f, 4.0f));
    pScene->addNode(pLight);

    pLight = new PointLight();
    pLight->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
    pLight->translate(glm::vec3(4.0f, 0.0f, 0.0f));
    pScene->addNode(pLight);

    pLight = new PointLight();
    pLight->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
    pLight->translate(glm::vec3(0.0f, -4.0f, 0.0f));
    pScene->addNode(pLight);


    SpotLight *pSpotLight = new SpotLight();
    pSpotLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    pSpotLight->setCutOffAngle(DS_DEG(5.0f));
    pSpotLight->rotate(glm::vec3(1.0f, 0.0f, 0.0f), DS_DEG(180.0f));
    pSpotLight->translate(glm::vec3(0.0f, 0.0f, -10.0f));
    pScene->addNode(pSpotLight);

    Camera *pCamera = new Camera();
    pCamera->setPerspectiveProjection(DS_DEG(60.0f),
                                      (float)c_viewportWidth / (float)c_viewportHeight,
                                      1.0f, 100.0f);
    pCamera->lookAt(glm::vec3(2.0f, -2.0f, 2.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f));
    pScene->addNode(pCamera);
    pScene->setActiveCamera(pCamera);

    // Prepare render passes
    GeometryRenderPass *pGeometryRP = new GeometryRenderPass(pScene, pRenderer);
    pRenderer->addRenderPass(pGeometryRP);

    AmbientLightingRenderPass *pAmbientLightRP = new AmbientLightingRenderPass(pScene, pRenderer);
    pAmbientLightRP->setColorTexture(&pGeometryRP->colorTexture());
    pRenderer->addRenderPass(pAmbientLightRP);

    PointLightnigRenderPass *pPointLightRP = new PointLightnigRenderPass(pScene, pRenderer);
    pPointLightRP->setColorTexture(&pGeometryRP->colorTexture());
    pPointLightRP->setPositionTexture(&pGeometryRP->positionTexture());
    pPointLightRP->setNormalTexture(&pGeometryRP->normalTexture());
    pRenderer->addRenderPass(pPointLightRP);

    SpotLightingRenderPass *pSpotLightRP = new SpotLightingRenderPass(pScene, pRenderer);
    pSpotLightRP->setColorTexture(&pGeometryRP->colorTexture());
    pSpotLightRP->setPositionTexture(&pGeometryRP->positionTexture());
    pSpotLightRP->setNormalTexture(&pGeometryRP->normalTexture());
    pRenderer->addRenderPass(pSpotLightRP);

    ShowTextureRenderPass *pTextureRP = new ShowTextureRenderPass(pGeometryRP, pRenderer);
    pTextureRP->setBlending(true);
    pTextureRP->addTexture(&pAmbientLightRP->targetColorTexture());
    pTextureRP->addTexture(&pPointLightRP->targetColorTexture());
    pTextureRP->addTexture(&pSpotLightRP->targetColorTexture());

    // Show intermediate renders
    //pTextureRP->setTextureHandle(&pGeometryRP->colorTexture());
    //pTextureRP->setTextureHandle(&pGeometryRP->depthTexture());

    pRenderer->addRenderPass(pTextureRP);

    pEngine->initialize("Test", c_viewportWidth, c_viewportHeight);
    pEngine->setFixedFps(60);
    pEngine->exec();

    delete pSceneUpdater;
    delete pScene;

    Engine::destroy();

    return 0;
}
