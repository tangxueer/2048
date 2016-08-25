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
		glview = GLViewImpl::createWithRect("HiCocos", Rect(0, 0, 640, 960));
		director->setOpenGLView(glview);
	}

	glview->setDesignResolutionSize(640, 960, ResolutionPolicy::SHOW_ALL);

	director->setDisplayStats(false);
 
	director->setAnimationInterval(1.0 / 60);
 
	auto scene = MainScene::createScene();

	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}