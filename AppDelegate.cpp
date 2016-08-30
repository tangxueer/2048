#include "AppDelegate.h"  

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}


bool AppDelegate::applicationDidFinishLaunching() {
 
	auto director = Director::getInstance();

	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::createWithRect("2048", Rect(0, 0, 640, 960));
		director->setOpenGLView(glview);
	}

	glview->setDesignResolutionSize(640, 960, ResolutionPolicy::NO_BORDER);

	director->setDisplayStats(false);
 
	director->setAnimationInterval(1.0 / 60);

	FileUtils::getInstance()->addSearchPath("res");

	auto scene = MainScene::createScene();

	////Ô¤¼ÓÔØÒôÐ§
	SimpleAudioEngine::getInstance()->preloadEffect("Sound/move.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Sound/merge.wav");

	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}