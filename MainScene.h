#ifndef __MAIN_SCENE_H__  
#define __MAIN_SCENE_H__  

#include "cocos2d.h"  
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "CardSprite.h"  
#include "MenuLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MainScene);
	static void ClearData();

private:
	void LoadBackground();
	void MenuTouch(cocos2d::Ref *pSender, Widget::TouchEventType type);
	void SetScore();
	Size GetVisibleSize();
	void SetTouchListener();
	void createCardSprite(cocos2d::Size size);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	bool doLeft();
	bool doRight();
	bool doUp();
	bool doDown();
	void autoCreateCardNumber();
	void doCheckGameOver();

	Layout* root;
	ImageView* imgIcon;
	ImageView* imgScorelast;
	ImageView* imgScoremax;
	Button* btnMenu;
	int firstX, firstY, endX, endY;
	CardSprite *cardArr[4][4]; 
	int lastScore;
	int maxScore;
	LabelTTF *labelTTFCardNumber;
	LabelTTF *labelTTFlastScore;
	LabelTTF *labelTTFmaxScore;

};

#endif // __MAIN_SCENE_H__  