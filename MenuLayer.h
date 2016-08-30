#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#include "MainScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;
using namespace CocosDenshion;

class MenuLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(MenuLayer);
	static void playEffect(const std::string &effectName, bool force);

private:
	void SetBackground();
	void SetButton();
	void ContinueTouch(cocos2d::Ref *pSender, Widget::TouchEventType type);
	void RestartTouch(cocos2d::Ref *pSender, Widget::TouchEventType type);
	void Close(Node* pSender);
	void loadSoundMenu();
	void soundMenuCallback(Ref *sender);

	Layout* root;
	ImageView* imgBack;
	ImageView* imgMenu;
	Button* btnContinue;
	Button* btnRestart;
	CheckBox* checkBox;

};

#endif // __MENU_LAYER_H__




