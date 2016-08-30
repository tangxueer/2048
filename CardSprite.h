#ifndef __CARD_SPRITE_H__ 
#define __CARD_SPRITE_H__ 

#include "cocos2d.h"  

class CardSprite : public cocos2d::Sprite {

public: 
	static CardSprite *createCardSprite(int numbers, int width, int height, float CardSpriteX, float CardSpriteY);
	virtual bool init();
	CREATE_FUNC(CardSprite);
	void setNumber(int num); 
	int getNumber();

private:
	void enemyInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY);

	int number;	 
	cocos2d::LabelTTF *labTTFCardNumber;
	cocos2d::LayerColor *layerColorBG;
};

#endif // __CARD_SPRITE_H__  