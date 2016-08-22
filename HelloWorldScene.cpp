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

	//������Ϸ������ʶ������¼������а�
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	//������Ƭ
	return true;
}


//��������ʶ����¼�
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Point touchPO = touch->getLocation();

	firstX = touchPO.x;
	firstY = touchPO.y;

	return true;
}
void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	//��ȡx����y����ƶ���Χ
	Point touchPO = touch->getLocation();
	endX = firstX - touchPO.x;
	endY = firstY - touchPO.y;
	//�ж� x����y����ƶ����룬���x�ľ���ֵ��y��ľ���ֵ���������һ�������������
	if (abs(endX) > abs(endY))
	{//����		
		if (endX+5>0)
		{		
			doLeft();//���
		}
		else
		{			
			doRight();//�ұ�
		}
	}
	else
	{//����		
		if (endY + 5>0)
		{			
			doUp();//�ϱ�
		}
		else
		{			
			doDown();//�±�
		}
	}
}

//�������ҵķ���
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