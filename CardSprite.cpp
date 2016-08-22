#include "CardSprite.h"


//初始化游戏卡片的方法
CardSprite* CardSprite::createCardSprite(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
	//创建一切卡片类型的套路
	CardSprite *enemy = new CardSprite();
	if (enemy && enemy->init())
	{
		enemy->autorelease;
		enemy->enemyInit(numbers, width, height, CardSpriteX, CardSpriteY);
		return enemy;
	}

	CC_SAFE_DELETE(enemy);
	return NULL;

}

bool CardSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}

//设置数字
void CardSprite::setNumber(int num)
{
	int number = num;
	if (number > 0)
	{
		labTTFCardNumber->setString(__String::createWithFormat("%i", number)->getCString());
	}
	else
	{
		labTTFCardNumber->setString("");
	}
}

//获取数字
int  CardSprite::getNumber()
{
	return number;
}

//定义显示在界面的数字
void CardSprite::enemyInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
	//初始化数字
	int number = numbers;

	//加入游戏卡片的背景颜色(RBG + Alpha)
	layerColorBG = cocos2d::LayerColor::create(cocos2d::Color4B(200, 190, 180, 255), width-15, height-15);
	layerColorBG->setPosition(Point(CardSpriteX, CardSpriteY));

	//判断如果卡片上的数字不为零则显示，否则为空消失
	if (number > 0)
	{
		//加入中间字体,参数1:字符内容，参数2：字体类型，参数3:字体大小
		labTTFCardNumber = cocos2d::LabelTTF::create(__String::createWithFormat("%i", number)->getCString(),"HirakakuProN-W6",100);
		labTTFCardNumber->setPosition(Point(layerColorBG->getContentSize().width / 2, layerColorBG->getContentSize().height / 2));
		
		layerColorBG->addChild(labTTFCardNumber);
	}
	else
	{
		labTTFCardNumber = cocos2d::LabelTTF::create("", "HirakakuProN-W6", 100);
		labTTFCardNumber->setPosition(Point(layerColorBG->getContentSize().width / 2, layerColorBG->getContentSize().height / 2));
		layerColorBG->addChild(labTTFCardNumber);
	}

	this->addChild(layerColorBG);
}
