#ifndef __CARD_SPRITE_H__
#define __CARD_SPRITE_H__

#include "cocos2d.h"

USING_NS_CC;

class CardSprite : public cocos2d::Sprite
{
public:
	//��ʼ����Ϸ��Ƭ�ķ���
	static CardSprite *createCardSprite(int numbers,int width,int height,float CardSpriteX, float CardSpriteY);
	virtual bool init();
	CREATE_FUNC(CardSprite);

	//��ȡ����
	int getNumber();
	
	//��������
	void setNumber(int num);

private:
	//������ʾ�ڽ��������
	int numbers;
	void enemyInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY);

	//������ʾ���ֵĿؼ�
	cocos2d::LabelTTF *labTTFCardNumber;

	//��ʾ�ı���
	cocos2d::LayerColor *layerColorBG;

};

#endif // __CARD_SPRITE_H__

