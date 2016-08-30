#include "MenuLayer.h"

bool MenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//��ȡstudio ui����
	SetBackground();
	//���ò˵�ѡ�ť
	SetButton();
	//������Ч
	loadSoundMenu();

	return true;
}

//��ȡstudio ui����
void MenuLayer::SetBackground()
{
	auto rootNode = CSLoader::createNode("MenuLayer.csb");
	root = (Layout*)rootNode->getChildByName("root");
	imgBack = (ImageView*)Helper::seekWidgetByName(root, "imgBack");
	imgMenu = (ImageView*)Helper::seekWidgetByName(root, "imgMenu");
	btnContinue = (Button*)Helper::seekWidgetByName(root, "btnContinue");
	btnRestart = (Button*)Helper::seekWidgetByName(root, "btnRestart");

	imgBack->setScale(0);//����һ��ʼ��СΪ0����С���󽥱�

	addChild(rootNode);

	imgBack->runAction(ScaleTo::create(0.2, 1.0));//�������ֶ��� 0.2s�𽥷Ŵ�1��	
}

//���ò˵�ѡ�ť
void MenuLayer::SetButton()
{
	btnContinue->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ContinueTouch, this));
	btnRestart->addTouchEventListener(CC_CALLBACK_2(MenuLayer::RestartTouch, this));
}

//������Ч
void MenuLayer::playEffect(const std::string &effectName, bool force)
{
	//�����ǿ�Ʋ��ţ�Ҫȥ��ȡ��������״̬��δ�����Ͳ�����
	if (!force) {
		auto soundOff = UserDefault::getInstance()->getBoolForKey("soundOff");
		if (soundOff) return;
	}
	//������Ч
	SimpleAudioEngine::getInstance()->playEffect(effectName.c_str());
}

//���������˵�
void MenuLayer::loadSoundMenu()
{
	//����֡�����ȡplist  
	SpriteFrameCache *sfc = SpriteFrameCache::sharedSpriteFrameCache();
	sfc->addSpriteFramesWithFile("res/images.plist");
	//���ش�ͼ  
	auto sprites = SpriteBatchNode::create("res/images.png");
	sprites->setPosition(Point::ZERO);
	this->addChild(sprites, 1);

	//������ť
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

//�����˵��ص�����
void MenuLayer::soundMenuCallback(Ref *sender)
{
	auto index = ((MenuItemToggle *)sender)->getSelectedIndex();

	auto userDefault = UserDefault::getInstance();
	switch (index) {
	case 0:
		//log("��������...");
		MenuLayer::playEffect("Sound/move.wav", true);
		userDefault->setBoolForKey("soundOff", false);
		break;
	case 1:
		//log("�ر�����...");
		userDefault->setBoolForKey("soundOff", true);
		break;
	default:
		break;
	}
	userDefault->flush(); //�ύ
}

//�رղ˵���
void MenuLayer::Close(Node* pSender)
{
	this->removeFromParentAndCleanup(true);//�Ѹò�Ӹ��ռ����
}

//������ť
void MenuLayer::ContinueTouch(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		MenuLayer::playEffect("Sound/move.wav",false);
		auto seq = Sequence::create(ScaleTo::create(0.2, 0), CallFuncN::create(CC_CALLBACK_1(MenuLayer::Close, this)), NULL);
		//�����رն��� 0.2s����С����ʧ
		imgBack->runAction(seq);
		break;
	}
}

//���¿�ʼ��ť
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


