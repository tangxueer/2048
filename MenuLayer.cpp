#include "MenuLayer.h"

bool MenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//获取studio ui场景
	SetBackground();
	//设置菜单选项按钮
	SetButton();
	//加载音效
	loadSoundMenu();

	return true;
}

//获取studio ui场景
void MenuLayer::SetBackground()
{
	auto rootNode = CSLoader::createNode("MenuLayer.csb");
	root = (Layout*)rootNode->getChildByName("root");
	imgBack = (ImageView*)Helper::seekWidgetByName(root, "imgBack");
	imgMenu = (ImageView*)Helper::seekWidgetByName(root, "imgMenu");
	btnContinue = (Button*)Helper::seekWidgetByName(root, "btnContinue");
	btnRestart = (Button*)Helper::seekWidgetByName(root, "btnRestart");

	imgBack->setScale(0);//弹窗一开始大小为0，从小到大渐变

	addChild(rootNode);

	imgBack->runAction(ScaleTo::create(0.2, 1.0));//弹窗出现动画 0.2s逐渐放大1倍	
}

//设置菜单选项按钮
void MenuLayer::SetButton()
{
	btnContinue->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ContinueTouch, this));
	btnRestart->addTouchEventListener(CC_CALLBACK_2(MenuLayer::RestartTouch, this));
}

//播放音效
void MenuLayer::playEffect(const std::string &effectName, bool force)
{
	//如果非强制播放，要去获取声音开启状态，未开启就不播放
	if (!force) {
		auto soundOff = UserDefault::getInstance()->getBoolForKey("soundOff");
		if (soundOff) return;
	}
	//播放音效
	SimpleAudioEngine::getInstance()->playEffect(effectName.c_str());
}

//加载声音菜单
void MenuLayer::loadSoundMenu()
{
	//精灵帧缓存读取plist  
	SpriteFrameCache *sfc = SpriteFrameCache::sharedSpriteFrameCache();
	sfc->addSpriteFramesWithFile("res/images.plist");
	//加载大图  
	auto sprites = SpriteBatchNode::create("res/images.png");
	sprites->setPosition(Point::ZERO);
	this->addChild(sprites, 1);

	//声音按钮
	auto soundOnSprite = Sprite::createWithSpriteFrameName("btn_sound_on.png");
	auto soundOnSpriteItem = MenuItemSprite::create(soundOnSprite, soundOnSprite);

	auto soundOffSprite = Sprite::createWithSpriteFrameName("btn_sound_off.png");
	auto soundOffSpriteItem = MenuItemSprite::create(soundOffSprite, soundOffSprite);

	auto toggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuLayer::soundMenuCallback, this), soundOnSpriteItem, soundOffSpriteItem, NULL);
	toggleItem->setPosition(Point(240, 400));
	if (UserDefault::getInstance()->getBoolForKey("soundOff"))
	{
		toggleItem->setSelectedIndex(1);
	}
	auto soundMenu = Menu::create(toggleItem, NULL);
	this->addChild(soundMenu);
}

//声音菜单回调函数
void MenuLayer::soundMenuCallback(Ref *sender)
{
	auto index = ((MenuItemToggle *)sender)->getSelectedIndex();

	auto userDefault = UserDefault::getInstance();
	switch (index) {
	case 0:
		//log("开启声音...");
		MenuLayer::playEffect("Sound/move.wav", true);
		userDefault->setBoolForKey("soundOff", false);
		break;
	case 1:
		//log("关闭声音...");
		userDefault->setBoolForKey("soundOff", true);
		break;
	default:
		break;
	}
	userDefault->flush(); //提交
}

//关闭菜单层
void MenuLayer::Close(Node* pSender)
{
	this->removeFromParentAndCleanup(true);//把该层从父空间清空
}

//继续按钮
void MenuLayer::ContinueTouch(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		MenuLayer::playEffect("Sound/move.wav",false);
		auto seq = Sequence::create(ScaleTo::create(0.2, 0), CallFuncN::create(CC_CALLBACK_1(MenuLayer::Close, this)), NULL);
		//弹窗关闭动画 0.2s逐渐缩小到消失
		imgBack->runAction(seq);
		break;
	}
}

//重新开始按钮
void MenuLayer::RestartTouch(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		MenuLayer::playEffect("Sound/move.wav",false);
		MainScene::ClearData();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainScene::createScene()));
		break;
	}
}


