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

	//加入手势识别的事件
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	//上下左右的方法
	bool doLeft();
	bool doRight();
	bool doUp();
	bool doDown();

private:
	//点击的元素
	int firstX;
	int firstY;
	int endX;
	int endY;

};

#endif // __HELLOWORLD_SCENE_H__
