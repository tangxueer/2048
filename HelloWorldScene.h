#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

	//��������ʶ����¼�
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	//�������ҵķ���
	bool doLeft();
	bool doRight();
	bool doUp();
	bool doDown();

private:
	//�����Ԫ��
	int firstX;
	int firstY;
	int endX;
	int endY;

};

#endif // __HELLOWORLD_SCENE_H__
