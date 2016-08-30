#include "MainScene.h"  
#include "CardSprite.h"  

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{ 
	if (!Layer::init())
	{
		return false;
	}
	lastScore = 0;
	maxScore = 0;

	//��ȡstudio ui����
	LoadBackground();
	//���÷���
	SetScore();
	//��������ʶ����¼�������,����¼����� 
	SetTouchListener();
	// ���ɷ���Ƭ
	createCardSprite(GetVisibleSize());
	// �����������Ƭ
	autoCreateCardNumber();
	autoCreateCardNumber();
	
	return true;
}

//����ϴ���Ϸ���ݣ��������¿�ʼ
void MainScene::ClearData()
{
	UserDefault::getInstance()->setIntegerForKey("lastScore", 0);
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			auto key = String::createWithFormat("tile_%i%i", x, y)->getCString();
			UserDefault::getInstance()->setIntegerForKey(key, 0);
		}
	}
	UserDefault::getInstance()->flush();
}

//��ȡstudio ui����
void MainScene::LoadBackground()
{
	auto rootNode = CSLoader::createNode("MainScene.csb");
	root = (Layout*)rootNode->getChildByName("root");
	imgIcon = (ImageView*)Helper::seekWidgetByName(root, "imgIcon");
	imgScorelast = (ImageView*)Helper::seekWidgetByName(root, "imgScorelast");
	imgScoremax = (ImageView*)Helper::seekWidgetByName(root, "imgScoremax");
	btnMenu = (Button*)Helper::seekWidgetByName(root, "btnMenu");

	btnMenu->addTouchEventListener(CC_CALLBACK_2(MainScene::MenuTouch, this));

	addChild(rootNode);
}

// �����Ļ���Ӵ�С  
Size MainScene::GetVisibleSize()
{	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	return visibleSize;
}

//���÷���
void MainScene::SetScore()
{	
	//�������
	labelTTFlastScore = LabelTTF::create("0", "HelveticaNeue-Bold", 40);
	labelTTFlastScore->setPosition(GetVisibleSize().width / 2 + 17, GetVisibleSize().height - 110);
	addChild(labelTTFlastScore);
	//��ʷ��߷���
	labelTTFmaxScore = LabelTTF::create("0", "HelveticaNeue-Bold", 40);
	labelTTFmaxScore->setPosition(GetVisibleSize().width / 2 + 212, GetVisibleSize().height - 110);
	addChild(labelTTFmaxScore);
}

//��������ʶ����¼�������,����¼����� 
void MainScene::SetTouchListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

//�����˵�����
void MainScene::MenuTouch(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		MenuLayer::playEffect("Sound/move.wav", false);
		auto layer = MenuLayer::create();
		this->addChild(layer);
		break;
	}
}

// ��Ϸ�Ƿ��ܼ���������ȥ  
void MainScene::doCheckGameOver() {
	bool isGameOver = true;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (cardArr[x][y]->getNumber() == 0
				|| (x>0 && (cardArr[x][y]->getNumber() == cardArr[x - 1][y]->getNumber()))
				|| (x<3 && (cardArr[x][y]->getNumber() == cardArr[x + 1][y]->getNumber()))
				|| (y<0 && (cardArr[x][y]->getNumber() == cardArr[x][y - 1]->getNumber()))
				|| (x<3 && (cardArr[x][y]->getNumber() == cardArr[x][y + 1]->getNumber()))) {
				isGameOver = false;
			}
		}
	}
	if (isGameOver) {
		// ������Ϸ  
		Director::getInstance()->replaceScene(TransitionFade::create(1, MainScene::createScene()));
	}

}

// �Զ����ɿ�Ƭ  
void MainScene::autoCreateCardNumber() {
	int i = CCRANDOM_0_1() * 4;
	int j = CCRANDOM_0_1() * 4;

	// �ж��Ƿ��Ѿ����ڵ�λ��  
	if (cardArr[i][j]->getNumber() > 0) {
		// �Ѵ��ڣ��ݹ鴴��  
		autoCreateCardNumber();
	}
	else {
		cardArr[i][j]->setAnchorPoint(Point(0.5, 0.5));
		cardArr[i][j]->setScale(0);//����һ��ʼ��СΪ0����С���󽥱�
		// ����2��4�ı�����1:9�ĸ���  
		cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
		
		cardArr[i][j]->runAction(ScaleTo::create(0.05, 1.0));//�������ֶ��� 0.2s�𽥷Ŵ�1��	
	}
}

// ������Ƭ��sizeΪ��Ļ��С  
void MainScene::createCardSprite(cocos2d::Size size) {
	// �����Ԫ��Ŀ�Ⱥ͸߶ȣ�28Ϊ���Ҿ���  
	int lon = (size.width - 28) / 4;

	// 4*4�ĵ�Ԫ��  
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			
			// ����0�������ͬΪlon,lon+j+20Ϊ��ƬX��λ�ã���lon+0+20Ϊ��һ����Ƭ��λ�ã�20��ÿ�ſ�Ƭ�ļ�϶��lon+i+20+size.height/6�������˼����Ļ��С������������ݣ���������ֻ��4��λ��  
			CardSprite *card = CardSprite::createCardSprite(0, lon, lon, lon * j + 20 , lon * i - 70 + size.height / 6);
			addChild(card);
			// ��ӿ�Ƭ����ά������  
			cardArr[j][i] = card;
		}
	}
}

// ��������ʶ����¼�  
bool MainScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// ������  
	Point touchP0 = touch->getLocation();

	firstX = touchP0.x;
	firstY = touchP0.y;


	return true;
}

// ������������  
void MainScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// ��ȡ������λ��  
	Point touchP0 = touch->getLocation();
	// ��ȡX���Y����ƶ�����  
	endX = firstX - touchP0.x;
	endY = firstY - touchP0.y;

	// �ж�X���Y����ƶ����룬���X��ľ���ֵ����Y��ľ���ֵ�������ҷ���������  
	if (abs(endX) > abs(endY)) {
		// ����  
		if (endX + 5 > 0) {
			// ���  
			if (doLeft()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			}

		}
		else {
			// �ұ�  
			if (doRight()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}

	}
	else {
		// ����  
		if (endY + 5 > 0) {
			// �±�  
			if (doDown()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}
		else {
			// �ϱ�  
			if (doUp()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			};
		}

	}
}


// ����  
bool MainScene::doLeft() {
	//log("doLeft");
	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {

			// ��һ��ѭ��Ϊ�жϿ�Ƭ�Ǻϲ��������  
			for (int x1 = x + 1; x1 < 4; x1++) {
				if (cardArr[x1][y]->getNumber() > 0) {// ������  
					if (cardArr[x][y]->getNumber() <= 0) { // Ϊ��  
														   // ����Ϊ�ұ߿�Ƭ����ֵ  
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						// ��ǰ��Ƭ��ֵ����ȽϿ�Ƭ��ֵ��ȣ�����Ϊ���2��  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));
						// ���÷���  
						lastScore += cardArr[x][y]->getNumber();
						if (lastScore > maxScore)
						{
							maxScore = lastScore;
						}

						labelTTFlastScore->setString(__String::createWithFormat("%i", lastScore)->getCString());
						labelTTFmaxScore->setString(__String::createWithFormat("%i", maxScore)->getCString());

						isdo = true;
					}
					break;// ����  
				}
			}
		}
	}

	return isdo;
}

// ����  
bool MainScene::doRight() {
	//log("doRight");
	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int y = 0; y < 4; y++) {
		for (int x = 3; x >= 0; x--) {

			// ѭ���ж���߿�Ƭ�����Ǻϲ��������  
			for (int x1 = x - 1; x1 >= 0; x1--) {
				if (cardArr[x1][y]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() <= 0) {// Ϊ��
														  // ����Ϊ�ұ߿�Ƭ����ֵ 
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x++;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						// ��ǰ��Ƭ��ֵ����ȽϿ�Ƭ��ֵ��ȣ�����Ϊ���2��  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));

						// ���÷���  
						lastScore += cardArr[x][y]->getNumber();
						if (lastScore > maxScore)
						{
							maxScore = lastScore;
						}

						labelTTFlastScore->setString(__String::createWithFormat("%i", lastScore)->getCString());
						labelTTFmaxScore->setString(__String::createWithFormat("%i", maxScore)->getCString());

						isdo = true;

					}
					break;
				}

			}
		}

	}

	return isdo;

}

// ����  
bool MainScene::doUp() {
	//log("doUp");
	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int x = 0; x < 4; x++) {
		for (int y = 3; y >= 0; y--) {

			// ��һ��ѭ��Ϊ�жϿ�Ƭ�Ǻϲ��������  
			for (int y1 = y - 1; y1 >= 0; y1--) {
				if (cardArr[x][y1]->getNumber() > 0) {// ������  
					if (cardArr[x][y]->getNumber() <= 0) { // Ϊ��  
														   // ����Ϊ�ұ߿�Ƭ����ֵ  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y++;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// ��ǰ��Ƭ��ֵ����ȽϿ�Ƭ��ֵ��ȣ�����Ϊ���2��  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));

						// ���÷���  
						lastScore += cardArr[x][y]->getNumber();
						if (lastScore > maxScore)
						{
							maxScore = lastScore;
						}

						labelTTFlastScore->setString(__String::createWithFormat("%i", lastScore)->getCString());
						labelTTFmaxScore->setString(__String::createWithFormat("%i", maxScore)->getCString());

						isdo = true;
					}
					break;// ����  
				}
			}
		}
	}

	return isdo;

}

// ����  
bool MainScene::doDown() {
	//log("doDown");
	bool isdo = false;
	// �����ѭ��Ϊ4*4����  
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			// ��һ��ѭ��Ϊ�жϿ�Ƭ�Ǻϲ��������  
			for (int y1 = y + 1; y1 < 4; y1++) {
				if (cardArr[x][y1]->getNumber() > 0) {// ������  
					if (cardArr[x][y]->getNumber() <= 0) { // Ϊ��  
														   // ����Ϊ�ұ߿�Ƭ����ֵ  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// ��ǰ��Ƭ��ֵ����ȽϿ�Ƭ��ֵ��ȣ�����Ϊ���2��  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));


						// ���÷���  
						lastScore += cardArr[x][y]->getNumber();
						if (lastScore > maxScore)
						{
							maxScore = lastScore;
						}

						labelTTFlastScore->setString(__String::createWithFormat("%i", lastScore)->getCString());
						labelTTFmaxScore->setString(__String::createWithFormat("%i", maxScore)->getCString());


						isdo = true;
					}
					break;// ����  
				}
			}
		}
	}

	return isdo;
}
