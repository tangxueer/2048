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

	//获取studio ui场景
	LoadBackground();
	//设置分数
	SetScore();
	//调用手势识别的事件监听器,添加事件监听 
	SetTouchListener();
	// 生成方格卡片
	createCardSprite(GetVisibleSize());
	// 生成随机数卡片
	autoCreateCardNumber();
	autoCreateCardNumber();
	
	return true;
}

//清除上次游戏数据，用于重新开始
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

//获取studio ui场景
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

// 获得屏幕可视大小  
Size MainScene::GetVisibleSize()
{	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	return visibleSize;
}

//设置分数
void MainScene::SetScore()
{	
	//最近分数
	labelTTFlastScore = LabelTTF::create("0", "HelveticaNeue-Bold", 40);
	labelTTFlastScore->setPosition(GetVisibleSize().width / 2 + 17, GetVisibleSize().height - 110);
	addChild(labelTTFlastScore);
	//历史最高分数
	labelTTFmaxScore = LabelTTF::create("0", "HelveticaNeue-Bold", 40);
	labelTTFmaxScore->setPosition(GetVisibleSize().width / 2 + 212, GetVisibleSize().height - 110);
	addChild(labelTTFmaxScore);
}

//调用手势识别的事件监听器,添加事件监听 
void MainScene::SetTouchListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

//弹出菜单界面
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

// 游戏是否还能继续运行下去  
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
		// 结束游戏  
		Director::getInstance()->replaceScene(TransitionFade::create(1, MainScene::createScene()));
	}

}

// 自动生成卡片  
void MainScene::autoCreateCardNumber() {
	int i = CCRANDOM_0_1() * 4;
	int j = CCRANDOM_0_1() * 4;

	// 判断是否已经存在的位置  
	if (cardArr[i][j]->getNumber() > 0) {
		// 已存在，递归创建  
		autoCreateCardNumber();
	}
	else {
		cardArr[i][j]->setAnchorPoint(Point(0.5, 0.5));
		cardArr[i][j]->setScale(0);//弹窗一开始大小为0，从小到大渐变
		// 生成2和4的比例是1:9的概率  
		cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
		
		cardArr[i][j]->runAction(ScaleTo::create(0.05, 1.0));//弹窗出现动画 0.2s逐渐放大1倍	
	}
}

// 创建卡片，size为屏幕大小  
void MainScene::createCardSprite(cocos2d::Size size) {
	// 求出单元格的宽度和高度，28为左右距离  
	int lon = (size.width - 28) / 4;

	// 4*4的单元格  
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			
			// 数字0，宽高相同为lon,lon+j+20为卡片X轴位置，如lon+0+20为第一个卡片的位置，20是每张卡片的间隙，lon+i+20+size.height/6代表的意思是屏幕大小竖方向分了六份，我们这里只放4个位置  
			CardSprite *card = CardSprite::createCardSprite(0, lon, lon, lon * j + 20 , lon * i - 70 + size.height / 6);
			addChild(card);
			// 添加卡片到二维数组中  
			cardArr[j][i] = card;
		}
	}
}

// 加入手势识别的事件  
bool MainScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// 触摸点  
	Point touchP0 = touch->getLocation();

	firstX = touchP0.x;
	firstY = touchP0.y;


	return true;
}

// 触摸结束触发  
void MainScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	// 获取触摸点位置  
	Point touchP0 = touch->getLocation();
	// 获取X轴和Y轴的移动距离  
	endX = firstX - touchP0.x;
	endY = firstY - touchP0.y;

	// 判断X轴和Y轴的移动距离，如果X轴的绝对值大于Y轴的绝对值就是左右否则是上下  
	if (abs(endX) > abs(endY)) {
		// 左右  
		if (endX + 5 > 0) {
			// 左边  
			if (doLeft()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			}

		}
		else {
			// 右边  
			if (doRight()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}

	}
	else {
		// 上下  
		if (endY + 5 > 0) {
			// 下边  
			if (doDown()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}
		else {
			// 上边  
			if (doUp()) {
				MenuLayer::playEffect("Sound/move.wav", false);
				autoCreateCardNumber();
				doCheckGameOver();
			};
		}

	}
}


// 向左  
bool MainScene::doLeft() {
	//log("doLeft");
	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {

			// 这一层循环为判断卡片是合并还是清空  
			for (int x1 = x + 1; x1 < 4; x1++) {
				if (cardArr[x1][y]->getNumber() > 0) {// 有数字  
					if (cardArr[x][y]->getNumber() <= 0) { // 为空  
														   // 设置为右边卡片的数值  
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						// 当前卡片的值与其比较卡片的值相等，设置为其的2倍  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));
						// 设置分数  
						lastScore += cardArr[x][y]->getNumber();
						if (lastScore > maxScore)
						{
							maxScore = lastScore;
						}

						labelTTFlastScore->setString(__String::createWithFormat("%i", lastScore)->getCString());
						labelTTFmaxScore->setString(__String::createWithFormat("%i", maxScore)->getCString());

						isdo = true;
					}
					break;// 跳出  
				}
			}
		}
	}

	return isdo;
}

// 向右  
bool MainScene::doRight() {
	//log("doRight");
	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int y = 0; y < 4; y++) {
		for (int x = 3; x >= 0; x--) {

			// 循环判断左边卡片往右是合并还是清空  
			for (int x1 = x - 1; x1 >= 0; x1--) {
				if (cardArr[x1][y]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() <= 0) {// 为空
														  // 设置为右边卡片的数值 
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);

						x++;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						// 当前卡片的值与其比较卡片的值相等，设置为其的2倍  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));

						// 设置分数  
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

// 向上  
bool MainScene::doUp() {
	//log("doUp");
	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int x = 0; x < 4; x++) {
		for (int y = 3; y >= 0; y--) {

			// 这一层循环为判断卡片是合并还是清空  
			for (int y1 = y - 1; y1 >= 0; y1--) {
				if (cardArr[x][y1]->getNumber() > 0) {// 有数字  
					if (cardArr[x][y]->getNumber() <= 0) { // 为空  
														   // 设置为右边卡片的数值  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y++;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// 当前卡片的值与其比较卡片的值相等，设置为其的2倍  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));

						// 设置分数  
						lastScore += cardArr[x][y]->getNumber();
						if (lastScore > maxScore)
						{
							maxScore = lastScore;
						}

						labelTTFlastScore->setString(__String::createWithFormat("%i", lastScore)->getCString());
						labelTTFmaxScore->setString(__String::createWithFormat("%i", maxScore)->getCString());

						isdo = true;
					}
					break;// 跳出  
				}
			}
		}
	}

	return isdo;

}

// 向下  
bool MainScene::doDown() {
	//log("doDown");
	bool isdo = false;
	// 最外层循环为4*4迭代  
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			// 这一层循环为判断卡片是合并还是清空  
			for (int y1 = y + 1; y1 < 4; y1++) {
				if (cardArr[x][y1]->getNumber() > 0) {// 有数字  
					if (cardArr[x][y]->getNumber() <= 0) { // 为空  
														   // 设置为右边卡片的数值  
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);

						y--;
						isdo = true;

					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						// 当前卡片的值与其比较卡片的值相等，设置为其的2倍  
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);
						MenuLayer::playEffect("Sound/merge.wav", false);
						cardArr[x][y]->runAction(Sequence::create(ScaleTo::create(0.05, 1.1), ScaleTo::create(0.05, 1.0), NULL));


						// 设置分数  
						lastScore += cardArr[x][y]->getNumber();
						if (lastScore > maxScore)
						{
							maxScore = lastScore;
						}

						labelTTFlastScore->setString(__String::createWithFormat("%i", lastScore)->getCString());
						labelTTFmaxScore->setString(__String::createWithFormat("%i", maxScore)->getCString());


						isdo = true;
					}
					break;// 跳出  
				}
			}
		}
	}

	return isdo;
}
