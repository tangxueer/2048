#include "CardSprite.h"


//��ʼ����Ϸ��Ƭ�ķ���
CardSprite* CardSprite::createCardSprite(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
	//����һ�п�Ƭ���͵���·
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

//��������
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

//��ȡ����
int  CardSprite::getNumber()
{
	return number;
}

//������ʾ�ڽ��������
void CardSprite::enemyInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY)
{
	//��ʼ������
	int number = numbers;

	//������Ϸ��Ƭ�ı�����ɫ(RBG + Alpha)
	layerColorBG = cocos2d::LayerColor::create(cocos2d::Color4B(200, 190, 180, 255), width-15, height-15);
	layerColorBG->setPosition(Point(CardSpriteX, CardSpriteY));

	//�ж������Ƭ�ϵ����ֲ�Ϊ������ʾ������Ϊ����ʧ
	if (number > 0)
	{
		//�����м�����,����1:�ַ����ݣ�����2���������ͣ�����3:�����С
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
