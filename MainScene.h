#ifndef __MAIN_SCENE_H__  
#define __MAIN_SCENE_H__  

#include "cocos2d.h"  
#include "CardSprite.h"  

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(MainScene);

	// ��������ʶ����¼�  
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	// �������ҵķ���  
	bool doLeft();
	bool doRight();
	bool doUp();
	bool doDown();
	// �Զ���Ƭ����  
	void autoCreateCardNumber();
	// �ж���Ϸ�Ƿ��ܼ���������ȥ  
	void doCheckGameOver();

private:

	// �����Ԫ��λ��  
	int firstX, firstY, endX, endY;

	// ����һ����ά����  
	CardSprite *cardArr[4][4];

	// ������Ƭ  
	void createCardSprite(cocos2d::Size size);

	// ������Ϸ�ķ���  
	int score;
	// ������ʾ���ݵĿؼ�  
	cocos2d::LabelTTF *labelTTFCardNumber;
};

#endif // __MAIN_SCENE_H__  