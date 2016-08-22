#include "HelloWorldScene.h"


Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	//加入游戏的手势识别监听事件，进行绑定
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	//创建卡片
	return true;
}


//加入手势识别的事件
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Point touchPO = touch->getLocation();

	firstX = touchPO.x;
	firstY = touchPO.y;

	return true;
}
void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	//获取x轴与y轴的移动范围
	Point touchPO = touch->getLocation();
	endX = firstX - touchPO.x;
	endY = firstY - touchPO.y;
	//判断 x轴与y轴的移动距离，如果x的绝对值比y轴的绝对值大，则是左右滑动，否则上下
	if (abs(endX) > abs(endY))
	{//左右		
		if (endX+5>0)
		{		
			doLeft();//左边
		}
		else
		{			
			doRight();//右边
		}
	}
	else
	{//上下		
		if (endY + 5>0)
		{			
			doUp();//上边
		}
		else
		{			
			doDown();//下边
		}
	}
}

//上下左右的方法
bool HelloWorld::doLeft()
{
	CCLOG("left");
	return true;
}
bool HelloWorld::doRight()
{
	CCLOG("right");
	return true;
}
bool HelloWorld::doUp()
{
	CCLOG("up");
	return true;
}
bool HelloWorld::doDown()
{
	CCLOG("down");
	return true;
}