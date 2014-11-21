/****************************************************************************

Match 3 Source Code

Copyright (c) 2014 GameAcademy.com

www.GameAcademy.com

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

****************************************************************************/

#include "FXLightning.h"
#include "GameLayer.h"
#include "GameConstants.h"
#include "FallingGem.h"
#include "GemSprite.h"
#include "GameUtils.h"
#include "SimpleAudioEngine.h"
#include "GameState.h"
#include "PlayMenu.h"
#include "PauseLayer.h"
#include "perlin.h"
#include "EndGameLayer.h"

using namespace cocos2d;

#include <android/log.h>
#define LOGI(TAG,...) __android_log_print(ANDROID_LOG_INFO   , TAG,__VA_ARGS__)

#define PROGRESS_BAR_MARK "GameLayer/img_progressbar_marks.png"
#define TAKEN_GEM_SPRITE "GameLayer/particles/taken-gem.png"
#define GAME_TARGET_SPRITE "GameLayer/bgr_gametarget.png"
#define GAME_TARGET_SCORE "GameLayer/bgr_score.png"
#define PROGRESS_IMAGE "GameLayer/img_progressbar.png"
#define PROGRESS_BAR "GameLayer/bgr_progressbar.png"
#define EMOJIS_PNG "GameLayer/Candies/%i/thumb.png"
#define TILE_SELECTED_IMAGE "GameLayer/piece2.png"
#define MOVE_SPRITE "GameLayer/bgr_moves.png"
#define GAME_PLAY_BG "GameLayer/bgr_game.png"
#define GAME_PLAY_PLANT_IMAGE "GameLayer/plant.png"
#define GAME_PLAY_BG_5 "GameLayer/bgr_game_5.png"
#define LIGHTNING_POWERUP "GameLayer/LightningPowerUP.png"
#define LIGHTNING_FLASH "GameLayer/flash.png"


#define TILE_IMAGE "GameLayer/piece1.png"
#define HINT_PNG "GameLayer/piece2.png"

#define SPLASH_PLIST "GameLayer/Splash/Splash.plist"
#define SPLASH_PNG "GameLayer/Splash/Splash.png"




#define SCORE_STRING "Score"
#define MOVE_STRING "Moves"

#define zIndexParticles 1
#define zIndexHints 2
#define zIndexMenu 27

#define zIndexEffects 2
#define zIndexBG -1
#define zIndexCandies 40
#define zIndexEmojiBG 3


CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("GameFieldMusic.mp3",true);
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile(SPLASH_PLIST);

    gameLevel_ = GAME_SHARED_HANDLER->getCurrentLevelNumber();
    movesAllowed_=  GAME_SHARED_HANDLER->getTotalMoves(gameLevel_);
    scoreTarget_ = GAME_SHARED_HANDLER->getScoreTarget(gameLevel_);
    

    
    
    
    
    
    this->setKeypadEnabled(true);
    winSize = GameUtils::getDesignResolution();
    windowSize = CCDirector::sharedDirector()->getWinSize();
    
//    kOffsetX =13.0f;
    kOffsetX =85.0f;
    kOffsetY =windowSize.height/2- 310; /// 340

    
    CCMenuItemImage *pauseButton = CCMenuItemImage::create(BUTTON_PAUSE, BUTTON_PAUSE_PRESSED, this, menu_selector(GameLayer::pasueButtonPressed));
    CCMenu *pauseMenu = CCMenu::create(pauseButton,NULL);
    
    
    /// junaid
    float scale = .6;

    
    CCSprite* bckg;
    
    CCLog("shauket:: windowSize height %f",windowSize.height);
    CCLog("shauket:: winSize height %f",winSize.height);
    if(windowSize.height==1136 || windowSize.height > 960){
        bckg = CCSprite::create(GAME_PLAY_BG_5);
    }
    else{
        bckg = CCSprite::create(GAME_PLAY_BG);
       // bckg->setScaleY(1.2);
    }
    bckg->setPosition(ccp(windowSize.width/2, windowSize.height/2) );
    this->addChild(bckg,zIndexBG);
    
    
    CCSprite * plant = CCSprite::create(GAME_PLAY_PLANT_IMAGE);
    plant->setPosition(ccp(plant->getContentSize().width/2.0,windowSize.height/2.0));
    this->addChild(plant,1000.0);
    

    
    this->addChild(pauseMenu);
    pauseMenu->setPosition(CCPointZero);
    pauseButton->setPosition(ccp(pauseButton->getContentSize().width/2.0, windowSize.height-pauseButton->getContentSize().height/1.65));
    
    
    for (int i=0; i<kBoardWidth; i++) {
        for (int j=0; i<kBoardHeight; i++) {
            gBoardSprites[i][j] = NULL;
            
        }
    }
    
    currentEmojiFramesList_ = CCArray::create();
    currentEmojiFramesList_->retain();
    
    currentEmojiFramesList_->addObject(CCString::createWithFormat(EMOJIS_PNG,1));
    currentEmojiFramesList_->addObject(CCString::createWithFormat(EMOJIS_PNG,2));
    currentEmojiFramesList_->addObject(CCString::createWithFormat(EMOJIS_PNG,3));
    currentEmojiFramesList_->addObject(CCString::createWithFormat(EMOJIS_PNG,4));
    currentEmojiFramesList_->addObject(CCString::createWithFormat(EMOJIS_PNG,5));
    currentEmojiFramesList_->addObject(CCString::createWithFormat(EMOJIS_PNG,6));
    currentEmojiFramesList_->addObject(CCString::createWithFormat(EMOJIS_PNG,7));
    
    
    this->setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    this->setupBoard();
    
    gHintLayer = CCNode::create();
    this->addChild(gHintLayer, zIndexHints);
    
    gBoardChangedSinceEvaluation = true;
	gPossibleMove = -1;
    
    CCSprite* fillerBg = CCSprite::create(PROGRESS_BAR);
    fillerBg->setPosition(ccp(windowSize.width-fillerBg->getContentSize().width/2.0-5.0, windowSize.height-fillerBg->getContentSize().height/2.0) );
    this->addChild(fillerBg,1005);
    
//    CCSprite* targerSprite = CCSprite::create(PROGRESS_BAR_MARK);
//    targerSprite->setPosition(ccp(fillerBg->getContentSize().width-fillerBg->getContentSize().width/4+60,fillerBg->getContentSize().height/2-4) );
//    fillerBg->addChild(targerSprite,26);
    
    
    gTimer = CCProgressTimer::create(CCSprite::create(PROGRESS_IMAGE));
    gTimer->setPosition(ccp(fillerBg->getContentSize().width/2+2.0,fillerBg->getContentSize().height/2+10.0));
    gTimer->setPercentage(100);
    gTimer->setType(kCCProgressTimerTypeBar);
    gTimer->setMidpoint(ccp(0, 0.5));
    gTimer->setBarChangeRate(ccp(1, 0));
    fillerBg->addChild(gTimer);
    
    
    
    CCSprite* coingBg = CCSprite::create(MOVE_SPRITE);
    coingBg->setAnchorPoint(ccp(0,0.5));
    coingBg->setPosition(ccp(5.0,coingBg->getContentSize().height/1.8) );
    this->addChild(coingBg,2400);
    {
        CCLabelBMFont* score = CCLabelBMFont::create(MOVE_STRING,cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("font.fnt").c_str(), 35);
        score->setPosition(ccp(coingBg->getContentSize().width/1.3+coingBg->getContentSize().width,coingBg->getContentSize().height/2));
//        score->setColor(ccORANGE);
        score->setScale(scale);
        coingBg->addChild(score,26);
        
    }
    movesLabel = CCLabelBMFont::create("", cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("font.fnt").c_str(), 35);
    movesLabel->setAlignment( kCCTextAlignmentCenter );
    movesLabel->setPosition(ccp(coingBg->getContentSize().width/2,coingBg->getContentSize().height/1.9));
//    movesLabel->setColor(ccORANGE);
    movesLabel->setScale(scale*0.9);
    
    coingBg->addChild(movesLabel,27);
    movesLabel->setString(CCString::createWithFormat("%i",movesAllowed_)->getCString());
        
    CCSprite* scoreTarget=CCSprite::create(GAME_TARGET_SPRITE);
    scoreTarget->setPosition(ccp(fillerBg->getPositionX()+30.0,fillerBg->getPositionY()));
    addChild(scoreTarget,1060);
    {
        CCLabelBMFont* score = CCLabelBMFont::create(SCORE_STRING,cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("font.fnt").c_str(), 35);
        score->setPosition(ccp(scoreTarget->getContentSize().width/2,scoreTarget->getContentSize().height/2+8.0));
//        score->setColor(ccORANGE);
        score->setAlignment( kCCTextAlignmentLeft );
        score->setScale(scale-.1);
        scoreTarget->addChild(score,26);
        
//        string integerString = static_cast<ostringstream*>( &(ostringstream() << scoreTarget_) )->str();
//        integerString="Target: "+integerString;
//        score->setString(integerString.c_str());
        score->setString(CCString::createWithFormat("Target:%d",scoreTarget_)->getCString());
        
    }
    
    
    CCSprite* scoreSp=CCSprite::create(GAME_TARGET_SCORE);
    scoreSp->setPosition(ccp(windowSize.width-scoreSp->getContentSize().width/2.0,scoreSp->getContentSize().height/1.8));
    addChild(scoreSp,2500);
    {
        CCLabelBMFont* score = CCLabelBMFont::create(SCORE_STRING,cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("font.fnt").c_str(), 35);
        score->setPosition(ccp(winSize.width/2+50.0,scoreSp->getContentSize().height/1.8));
        score->setScale(scale);
//        score->setColor(ccORANGE);
        this->addChild(score,2600);
        
    }
    coinLabel_ = CCLabelBMFont::create("0",cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("font.fnt").c_str(), 35);
    coinLabel_->setPosition(ccp(winSize.width-scoreSp->getContentSize().width+14.0,scoreSp->getContentSize().height/1.8));
    coinLabel_->setAlignment( kCCTextAlignmentLeft );
    coinLabel_->setAnchorPoint( ccp(0, 0.5) );
    coinLabel_->setScale(scale*0.9);
//    coinLabel_->setColor(ccORANGE);
    this->addChild(coinLabel_,2605);
    coinLabel_->setString(CCString::createWithFormat("%i",0)->getCString());
    
    localScore= 0;
    startTouchEnable=false;
    return true;
}
void GameLayer::pasueButtonPressed()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("ButtonClick.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    PauseLayer *pauseLayer = PauseLayer::create();
    this->addChild(pauseLayer,20000);
    
}
void GameLayer::StartTouch()
{
    startTouchEnable=true;
}
void GameLayer::GameStart()
{

    this->removeChildByTag(KTAG_PLAYMENU_BLACKbg, true);
    
    this->removeChildByTag(KTAG_SOCIALPOPUP_LAYER, true);
    double time = nowInSeconds();
	gLastMoveTime  = time;
    gStartTime = time + kIntroTime;
    gNumConsecutiveGems = 0;
    gEndTimerStarted = false;
    gScore = 0;
    finalGameOverScreen = false;
    
//    scheduleUpdate();
    schedule(schedule_selector(GameLayer::update), 0.0167);
    schedule(schedule_selector(GameLayer::shakeSpritesRandomly), 2.0);
//    schedule(schedule_selector(GameLayer::enablelightningPowerUp), 5.0);
    
    {
        CCDelayTime *stoneDelayer = CCDelayTime::create(2.5);
        CCCallFunc *updater = CCCallFunc::create(this, callfunc_selector(GameLayer::StartTouch));
        this->runAction( CCSequence::create( stoneDelayer,updater, NULL ) );
    }
    
}

void GameLayer::setupBoard(){
    
    for (int i=0; i<kBoardWidth;i++) {
        for (int j=0; j<kBoardWidth;j++) {
            gBoard[i][j] = -1;
        }
    }
	for (int i = 0; i < kBoardWidth; i++)
	{
		gNumGemsInColumn[i] = 0;
		gTimeSinceAddInColumn[i] = 0;
	}
    
	// Setup falling pieces
	for (int i = 0; i < kBoardWidth; i++)
	{
        CCArray * gems = CCArray::createWithCapacity(kBoardWidth);
        gems->retain();
 		gFallingGems[i] = gems;
	}
    
}

CCArray * GameLayer::findConnectedGems(int x, int y){
    CCArray * connected = CCArray::createWithCapacity(kBoardWidth);
    if (gBoard[x][y] <= -1) return connected;
	findConnectedGems_(x, y,connected,gBoard[x][y]);
    return connected;
}

void GameLayer::findConnectedGems_(int x, int y, CCArray * arr, int gemType){
    
    
    // Check for bounds
	if (x < 0 || x >= kBoardWidth) return;
	if (y < 0 || y >= kBoardHeight) return;
    
    int gemTypeTCheck = gemType;
    
    int idx = x + y*kBoardWidth;
    
    if(  gBoard[x][y] == kLightningPowerUpTag)
    {
        gemTypeTCheck = powerUpMatchedWithType;
    }
	// Make sure that the gems type match
	else if (gBoard[x][y] != gemType) return;
    
    
	// Check if idx is already visited
	for (int i = 0; i < arr->count(); i++)
	{
        CCInteger * ccValue = (CCInteger *)arr->objectAtIndex(i);
		if (ccValue->getValue() == idx) return;
	}
    
	// Add idx to array
	arr->addObject(CCInteger::create(idx));
    
	// Visit neighbours
	findConnectedGems_(x+1, y, arr, gemTypeTCheck);
	findConnectedGems_(x-1, y, arr, gemTypeTCheck);
	findConnectedGems_(x, y+1, arr, gemTypeTCheck);
	findConnectedGems_(x, y-1, arr, gemTypeTCheck);
}
void GameLayer::removeMarkedGems(){
    
    // Iterate through the board
	for (int x = 0; x < kBoardWidth; x++)
	{
		for (int y = 0; y < kBoardHeight; y++)
		{
            
			if (gBoard[x][y] < -1)
			{
				// Increase the count for negative crystal types
				gBoard[x][y]++;
				if (gBoard[x][y] == -1)
				{
					gNumGemsInColumn[x]--;
					gBoardChangedSinceEvaluation = true;
                    
					// Transform any gem above this to a falling gem
					for (int yAbove = y+1; yAbove < kBoardHeight; yAbove++)
					{
                        
						if (gBoard[x][yAbove] < -1)
						{
							gNumGemsInColumn[x]--;
							gBoard[x][yAbove] = -1;
						}
						if (gBoard[x][yAbove] == -1) continue;
                        
						// The gem is not connected, make it into a falling gem
						int gemType = gBoard[x][yAbove];
						GemSprite * gemSprite = (GemSprite *)gBoardSprites[x][yAbove];
                        
                        if(gemSprite==NULL){
                            CCLOG("umair issue:removeMarkedGems");
                        }
                        FallingGem *gem = FallingGem::createWithInfo(gemSprite, gemType, yAbove, 0);
						gFallingGems[x]->addObject(gem);
                        
						// Remove from board
						gBoard[x][yAbove] = -1;
						gBoardSprites[x][yAbove] = NULL;
						gNumGemsInColumn[x]--;
                        
					}
                    
				}
			}
		}
	}
}

//bool GameLayer::removeConnectedGems(int x, int y)
bool GameLayer::removeConnectedGems(CCArray * params)
{
 
    int x = ((CCInteger*)params->objectAtIndex(0))->getValue();
    int y = ((CCInteger*)params->objectAtIndex(1))->getValue();
	if (x < 0 || x >= kBoardWidth) return false;
	if (y < 0 || y >= kBoardHeight) return false;
    
	CCArray * connected = findConnectedGems(x,y);
	bool removedGems = false;
    float xOfPowerup;
    float yOfPowerUp;
	if (connected->count() >= 3)
	{
        gHintLayer->removeAllChildrenWithCleanup(true);
		gIsDisplayingHint = false;

		gBoardChangedSinceEvaluation = true;
		removedGems = true;
        int multiplier =1;
        localScore=localScore+((300+(connected->count()-3)*300))*multiplier;
        coinLabel_->setString(CCString::createWithFormat("%i",localScore)->getCString());
        GameState::sharedGameState()->score = localScore;

        bool isLightningPowerFound = false;
		for (int i = 0; i < connected->count(); i++)
		{
            CCInteger * ccValue = (CCInteger *)connected->objectAtIndex(i);
			int idx = ccValue->getValue();
			int gemX = idx % kBoardWidth;
			int gemY = floor(idx/kBoardWidth);
            
			gBoard[gemX][gemY] = -kNumRemovalFrames;
            if(gBoardSprites[gemX][gemY]==NULL){
                
            }
            else
            {
                CCSprite * tempSprite = gBoardSprites[gemX][gemY];
                if(tempSprite->getTag() == kLightningPowerUpTag)
                {
                    lightningPowerAdded = false;
                    isLightningPowerFound = true;
                    xOfPowerup = (gemX*(kGemSize+20.0))+kOffsetX+kGemSize/2;
                    yOfPowerUp = (gemY*kGemSize)+kOffsetY+kGemSize/2;
                }
                this->removeChild(gBoardSprites[gemX][gemY], true);
                gBoardSprites[gemX][gemY] = NULL;
            }
            
			// Add particle effect
            
            addSplashParticleEffect((gemX*(kGemSize+20.0))+kOffsetX+kGemSize/2,(gemY*kGemSize)+kOffsetY+kGemSize/2);
            //TODO:umair add splash
//			CCParticleSystemQuad *particle =  CCParticleSystemQuad::create(TAKEN_GEM_PLIST);
//			particle->setPosition((gemX*kGemSize)+kOffsetX+kGemSize/2, (gemY*kGemSize)+kOffsetY+kGemSize/2);
//			particle->setAutoRemoveOnFinish(true);
//			gParticleLayer->addChild(particle);
        }
        if(isLightningPowerFound)
        {
            lightningArray = CCArray::create();
            lightningArray->retain();
            flashesArray = CCArray::create();
            flashesArray->retain();
            CCSprite * flash = CCSprite::create(LIGHTNING_FLASH);
            flash->setPosition(ccp(xOfPowerup,yOfPowerUp));
            this->addChild(flash,10001);
            flashesArray->addObject(flash);
            CCActionInterval * scale1 = CCScaleTo::create(0.1, 0.8);
            CCActionInterval * scale2 = CCScaleTo::create(0.1, 1.2);
            flash->runAction(CCRepeatForever::create(CCSequence::create(scale1,scale2,NULL)));
            
            for (int ii=0;ii<3;ii++)
            {
                int x ;
                int y ;
                
                CCSprite * t = NULL;
                while(t == NULL)
                {
                    x = arc4random()% kBoardWidth;
                    y = arc4random()% kBoardHeight;
                    t = gBoardSprites[x][y];
                    if(t)
                    {
                        
                        gBoard[x][y] = -kNumRemovalFrames;
                        this->removeChild(t, true);
                        gBoardSprites[x][y] = NULL;
                        addSplashParticleEffect((x*(kGemSize+20.0))+kOffsetX+kGemSize/2,(y*kGemSize)+kOffsetY+kGemSize/2);
                        
                       FXLightning* lightning = new FXLightning();
                        //    lightning->initWithFile("lightning.png");
                        lightning->initialize();
                        lightning->setPosition(ccp(0, 0));
                        lightning->start = ccp(xOfPowerup , yOfPowerUp);
                        lightning->end = ccp(x*(kGemSize+20.0)+kOffsetX+kGemSize/2 , (y*kGemSize)+kOffsetY+kGemSize/2);
                        lightning->width = 25;
                        this->addChild(lightning,1000);
                        lightningArray->addObject(lightning);
                        
                        CCSprite * flash = CCSprite::create(LIGHTNING_FLASH);
                        flash->setPosition(ccp(lightning->end.x,lightning->end.y));
                        flash->setScale(.5);
                        this->addChild(flash,10001);
                        flashesArray->addObject(flash);
                        CCActionInterval * scale1 = CCScaleTo::create(0.1, 0.3);
                        CCActionInterval * scale2 = CCScaleTo::create(0.1, 0.6);
                        flash->runAction(CCRepeatForever::create(CCSequence::create(scale1,scale2,NULL)));
                    }
                }
            }
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("flashSound.mp3");
            powerUpMatchedWithType = -7;
            this->unschedule(schedule_selector(GameLayer::update));
            scheduleOnce(schedule_selector(GameLayer::removeLightningEffect), 1.0);

        }
	}
	else
	{
        
	}
    
    
    long time = nowInSeconds();
	gLastMoveTime  = time;
    return removedGems;
}


void GameLayer::addSplashParticleEffect(int x, int y)
{

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteBatchNode *spriteSheet = CCSpriteBatchNode::create(SPLASH_PNG);
    this->addChild(spriteSheet, 100);
    
    splashSprite_ = CCSprite::createWithSpriteFrameName("Splash_0001.png");
    splashSprite_->setPosition(CCPoint(x, y));
    splashSprite_->setAnchorPoint(ccp(0.5,0.5));
    splashSprite_->setRotation( CCRANDOM_MINUS1_1() * 45);
    
    spriteSheet->addChild(splashSprite_);
    
    
    CCArray* splashFrames = CCArray::createWithCapacity(16);
    for (int i=1; i<=23; i++) {

        const char* str = CCString::createWithFormat("Splash_%04d.png",i)->getCString();
        CCSpriteFrame* frame = cache->spriteFrameByName(str);
        splashFrames->addObject(frame);
        
    }
    CCAnimation *splashAnim = CCAnimation::create();

    splashAnim = CCAnimation::createWithSpriteFrames(splashFrames, 0.02f);
   
    CCCallFuncN *callback = CCCallFuncN::create(this, callfuncN_selector(GameLayer::removSplash));
    CCSequence *seq = CCSequence::create(CCAnimate::create(splashAnim),callback,NULL);
    splashSprite_->runAction(seq);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("eggSplash.mp3");
}
void GameLayer::removSplash(CCObject *sender)
{
    
    CCSprite *sp = (CCSprite *)sender;
    sp->stopAllActions();
    sp->removeFromParentAndCleanup(true);
}
void GameLayer::onRemoveFromParent(CCNode *value){
    
    CCSprite *sprite = (CCSprite *)value;
    if(!sprite) return;
    sprite->stopAllActions();
    sprite->unscheduleAllSelectors();
    sprite->removeFromParentAndCleanup(true);
}


void GameLayer::update(float fDelta){
    if (!gIsGameOver )
	{
		removeMarkedGems();
		for (int x = 0; x < kBoardWidth; x++)
		{
			if (gNumGemsInColumn[x] + gFallingGems[x]->count() < kBoardHeight
				&& gTimeSinceAddInColumn[x] >= kTimeBetweenGemAdds)
			{
				int gemType = floor(arc4random()%kGameUniqueCandies);
                
                CCString *emojiName = (CCString *) currentEmojiFramesList_->objectAtIndex(gemType);
				GemSprite * gemEmojiSprite;// = (GemSprite *)GemSprite::create(emojiName->getCString());
                /// junaid
                bool powerUpAdded = false;
                int tempRand = arc4random() %10;
                if(tempRand == 5 && ! lightningPowerAdded)
                {
                    gemEmojiSprite = (GemSprite *)GemSprite::create(LIGHTNING_POWERUP);
                    gemEmojiSprite->setTag(kLightningPowerUpTag);
                    lightningPowerAdded = true;
                    powerUpAdded = true;
                }
                else
                {
                    gemEmojiSprite = (GemSprite *)GemSprite::create(emojiName->getCString());
                    gemEmojiSprite->setTag(1);
                }

				GemSprite * gemSprite = (GemSprite *)GemSprite::create(TILE_IMAGE);
                gemSprite->addChild(gemEmojiSprite);
                gemSprite->retain();
                
                gemSprite->setOpacity(0.5);
                gemEmojiSprite->setPosition(ccp(gemSprite->getContentSize().width/2,gemSprite->getContentSize().height/2-10.0));
                gemSprite->setPosition(ccp((x*(kGemSize+20.0))+kOffsetX+kGemSize/2, (kBoardHeight * kGemSize)+kOffsetY+kGemSize/2));
                FallingGem *gem = FallingGem::createWithInfo(gemSprite, gemType, kBoardHeight, 0);
                if(powerUpAdded)
                {
                    gem->isPowerUpGem = true;
                }
				gFallingGems[x]->addObject(gem);
                
				this->addChild(gemSprite,zIndexCandies);
				gTimeSinceAddInColumn[x] = 0;
			}
                
			gTimeSinceAddInColumn[x]++;
		}
		bool gemLanded = false;
		for (int x = 0; x < kBoardWidth; x++)
		{
            
			CCArray * column = gFallingGems[x];
			int numFallingGems = gFallingGems[x]->count();
			for (int i = numFallingGems-1; i >= 0; i--)
			{
				FallingGem * gem = (FallingGem *)column->objectAtIndex(i);
                if(gem)
                {
                    gem->gemSpeedY_ += 0.03;
                    gem->gemSpeedY_ *= 0.9; //1.45; // 0.85
                    gem->gemPositionY_ -= gem->gemSpeedY_;
                    int y = gNumGemsInColumn[x];//+ (numFallingGems-1-i);

                    float mAxPositionY = (y*kGemSize)+kOffsetY+kGemSize/2;
                    float newPosition = (gem->gemPositionY_*kGemSize)+kOffsetY+kGemSize/2;
                    
                    if (newPosition <= mAxPositionY)
                    {
                        // The gem hit the ground or a fixed gem
                        if (!gemLanded)
                        {
                            gemLanded = true;
                        }
                        
                        // Insert into board
                        
                        
                        if (gBoard[x][y] != -1)
                        {
                        }
                        gBoard[x][y] = gem->gemType_;
                        
                        gBoardSprites[x][y] = gem->gemSprite_;
                        /// junaid
                        if(gem->isPowerUpGem)
                        {
                            gBoard[x][y] = kLightningPowerUpTag;
                            gBoardSprites[x][y]->setTag(kLightningPowerUpTag);
                            
                        }
                        
                        if(gem->gemSprite_==NULL){
                            continue;
                        }
                        // Update fixed position
                        gem->gemSprite_->setPosition(ccp((x*(kGemSize+20.0))+kOffsetX+kGemSize/2, mAxPositionY));
                        column->removeObject(gem);
                        gNumGemsInColumn[x]++;
                        gBoardChangedSinceEvaluation = true;
                        this->reorderChild(gBoardSprites[x][y], zIndexCandies-y);
                    }
                    else
                    {
                        // Update the falling gems position
                        if(gem->gemSprite_){
                            gem->gemSprite_->setPosition(ccp((x*(kGemSize+20.0))+kOffsetX+kGemSize/2,newPosition));
                            gem->gemSprite_->setRotation( noise1(newPosition*0.01) * 45 );
                            this->reorderChild(gem->gemSprite_, zIndexCandies-y);
                        }
                    }

                    //interesting things is going here // i hope interesting thing has gone now :P
//                    this->reorderChild(gem->gemSprite_, zIndexCandies-y);

                }
                else{
                    
                }
			}
		}
		// Update timer
		double currentTime = nowInSeconds();
        float percentage;
        
        percentage=(float)localScore/(float)scoreTarget_*75;
        gTimer->setPercentage(percentage);
        
		if (currentTime - gLastMoveTime > kMaxTimeBetweenConsecutiveMoves)
		{
			gNumConsecutiveGems = 0;
		}
		// Check for game over
		if (movesAllowed_ == 0)
		{
            createGameOver();
            gIsGameOver = true;
        }
		else
        {
            if (currentTime - gLastMoveTime > kDelayBeforeHint && (!gIsDisplayingHint))
            {
                    displayHint();
            }
            if(currentTime - gLastMoveTime > kDelayBeforeHint){
                CCPoint hintPoint = getHintPoint();
                
                if (hintPoint.x == -1 && hintPoint.y == -1)
                {
                    createRandomMove();
                }
            }
        }
        fallingGem();
        
	}
	else
	{
		// It's game over
		updateGameOver();
        this->unschedule(schedule_selector(GameLayer::enablelightningPowerUp));
        this->unschedule(schedule_selector(GameLayer::shakeSpritesRandomly));
        
        if (!finalGameOverScreen){
            finalGameOverScreen=true;
            CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Gameover.mp3");
            this->schedule(schedule_selector(GameLayer::GameOverScreen),2.2);
        }
        
    }

}
void GameLayer::fallingGem()
{
    bool isFallingGems = false;
    for (int x = 0; x < kBoardWidth; x++)
    {
        if (gNumGemsInColumn[x] != kBoardHeight)
        {
            isFallingGems = true;
            break;
        }
    }
    
    if (!isFallingGems)
    {
        isMakingMove_ = false;
        
        
        if(!isMakingMove_){
            isMakingMove_ = true;
            bool isBreak = false;
            for (int i=0;(!isBreak)&&(i<kBoardWidth); i++) {
                for (int j=0;j<kBoardHeight; j++) {
                    
                    bool moved1 = checkGridPositionForValidMove(ccp(i,j));
                    if(moved1){
                        CCCallFuncO  *updater = CCCallFuncO::create(this, callfuncO_selector(GameLayer::removeConnectedGems),CCArray::create(CCInteger::create(i), CCInteger::create(j),NULL));
                        this->runAction(CCSequence::create(CCDelayTime::create(0.5),updater,NULL));
                        gLastMoveTime  = nowInSeconds();
                        isBreak = true;
                        return;
                    }
                }
            }
            isMakingMove_ = false;
        }
        
    }
    
}
double GameLayer::nowInSeconds()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    double retVal = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));
    
    return retVal;
    
}

int GameLayer::findMove(){
    
	// Iterate through all places on the board
	for (int y = 0; y < kBoardHeight; y++)
	{
		for (int x = 0; x < kBoardWidth; x++)
		{
			int idx = x + y*kBoardWidth;
			int gemType = gBoard[x][y];
            
			// Make sure that it is a gem
			if (gemType < 0 || gemType >= kGameUniqueCandies) continue;
            
			// Check surrounding tiles
			int numSimilar = 0;
            
			if (getGemType(x-1, y) == gemType) numSimilar++;
			if (getGemType(x+1, y) == gemType) numSimilar++;
			if (getGemType(x, y-1) == gemType) numSimilar++;
			if (getGemType(x, y+1) == gemType) numSimilar++;
            
			if (numSimilar >= 2)
			{
				gPossibleMove = idx;
				return idx;
			}
		}
	}
	gBoardChangedSinceEvaluation = false;
	gPossibleMove = -1;
	return -1;
    
}

int GameLayer::getGemType(int x, int y){
    if (x < 0 || x >= kBoardWidth) return -1;
	if (y < 0 || y >= kBoardHeight) return -1;
	return gBoard[x][y];
}
void GameLayer::setGemType(int x, int y, int newType){
    // Check bounds
	if (x < 0 || x >= kBoardWidth) return;
	if (y < 0 || y >= kBoardHeight) return;
    
	// Get the type of the gem
	int gemType = gBoard[x][y];
    
	// Make sure that it is a gem
	if (gemType < 0 || gemType >= kGameUniqueCandies) return;
    
	gBoard[x][y] = newType;
    
	// Remove old gem and insert a new one
    if(gBoardSprites[x][y]==NULL){
        CCLOG("umair issue:setGemType");
    }
    else{
        this->removeChild(gBoardSprites[x][y], true);
    }
    
    
    
    
    
    CCString *emojiName = (CCString *) currentEmojiFramesList_->objectAtIndex(newType);
    GemSprite * gemEmojiSprite = (GemSprite *)GemSprite::create(emojiName->getCString());
    GemSprite * gemSprite = (GemSprite *)GemSprite::create(TILE_IMAGE);
    
    gemSprite->addChild(gemEmojiSprite);
    gemEmojiSprite->setTag(1);
    gemSprite->setOpacity(0.5);

    gemEmojiSprite->setPosition(ccp(gemSprite->getContentSize().width/2,gemSprite->getContentSize().height/2));
    
    
    gemSprite->setPosition(ccp((x*(kGemSize+20.0))+kOffsetX+kGemSize/2, (y * kGemSize)+kOffsetY+kGemSize/2));
    
	this->addChild(gemSprite,zIndexCandies-y);
	gBoardSprites[x][y] = gemSprite;
    
	gBoardChangedSinceEvaluation = true;
}
void GameLayer::displayHint(){
    
    gIsDisplayingHint = true;
    
    CCPoint hintPoint = getHintPoint();
    
    if (hintPoint.x != -1) {
        
        CCScaleTo *scaleAction = CCScaleTo::create(0.1,0.75);
        
        CCFadeIn  *actionFadeIn = CCFadeIn::create(0.5);
		CCFadeOut *actionFadeOut = CCFadeOut::create(0.5);
		CCSequence *actionSeq = CCSequence::create(actionFadeIn, actionFadeOut,NULL);
		CCRepeatForever *action = CCRepeatForever::create(actionSeq);
        
        
        gHintLayer->removeAllChildren();
		CCSprite * hintSprite = CCSprite::create(TILE_SELECTED_IMAGE);
        hintSprite->setScale(0.5);
        
        hintSprite->setOpacity(0);
		hintSprite->setPosition(ccp((hintPoint.x*(kGemSize+20.0))+kOffsetX+kGemSize/2, (hintPoint.y*kGemSize)+kOffsetY+kGemSize/2));
		gHintLayer->addChild(hintSprite);
		hintSprite->runAction(action);
		hintSprite->runAction(scaleAction);
        
    }
    
}
void GameLayer::createRandomMove()
{
	// Find a random place in the lower part of the board
	int x = floor(arc4random()%(kBoardWidth-2));
    int y = floor(arc4random()%(kBoardHeight-1));
    y+=1;
    
    
	// Make sure it is a gem that we found
	int gemType = gBoard[x][y];
    int retry = 0;
    while(gemType <= -1 || gemType >= kGameUniqueCandies){
        x = floor(arc4random()%(kBoardWidth-2));
        y = floor(arc4random()%(kBoardHeight-1));
        y+=1;
        retry++;
        gemType = gBoard[x][y];
        
        //FIXED freeze
        if(retry>10)
            return;
    }
	if (gemType <= -1 || gemType >= kGameUniqueCandies) return;
    
	// Change the color of two surrounding gems
	setGemType(x+1, y, gemType);
	setGemType(x+2, y+1, gemType);
    
	gBoardChangedSinceEvaluation = true;
}


void GameLayer::createGameOver()
{
	gGameOverGems = CCArray::create();
    gGameOverGems->retain();
	for (int x = 0; x < kBoardWidth; x++)
	{
		CCArray * column = gFallingGems[x];
		for (int i = 0; i < column->count(); i++)
		{
			FallingGem * gem = (FallingGem *)column->objectAtIndex(i);
            
            float randomfloat = (arc4random()%10)/10.0f;
			float ySpeed = (randomfloat*2-1)*kGameOverGemSpeed;
            randomfloat = (arc4random()%10)/10.0f;
			float xSpeed = (randomfloat*2-1)*kGameOverGemSpeed;
            FallingGem *gameOverGem = FallingGem::createWithInfo(gem->gemSprite_, gem->gemType_,gem->gemPositionX_, gem->gemPositionY_,xSpeed,ySpeed);
            gGameOverGems->addObject(gameOverGem);
		}
        
		for (int y = 0; y < kBoardHeight; y++)
		{
			if (gBoardSprites[x][y])
			{
                float randomfloat = (arc4random()%10)/10.0f;
				float ySpeed = (randomfloat*2-1)*kGameOverGemSpeed;
                randomfloat = (arc4random()%10)/10.0f;
				float xSpeed = (randomfloat*2-1)*kGameOverGemSpeed;
                GemSprite *gem = (GemSprite *)gBoardSprites[x][y];
                FallingGem *gameOverGem = FallingGem::createWithInfo(gem, gBoard[x][y],x,y,xSpeed,ySpeed);
                gGameOverGems->addObject(gameOverGem);
			}
		}
	}
    
	gHintLayer->removeAllChildrenWithCleanup(true);
    
}
void GameLayer::updateGameOver()
{
    
	for (int i = 0; i < gGameOverGems->count(); i++)
	{
		FallingGem * gem = (FallingGem *)gGameOverGems->objectAtIndex(i);
        
		gem->gemPositionX_ = gem->gemPositionX_  + gem->gemSpeedX_;
        gem->gemPositionY_ = gem->gemPositionY_  + gem->gemSpeedY_;
		gem->gemSpeedY_ -= kGameOverGemAcceleration;
        
        
        float positionX = kOffsetX + gem->gemPositionX_*(kGemSize+20.0)+kGemSize/2;
        float positionY = kGemSize/2+kOffsetY + gem->gemPositionY_*kGemSize;
        
        if(gem->gemSprite_)
		gem->gemSprite_->setPosition(ccp(positionX,positionY));
	}
    
}

bool GameLayer::areAdjacent(CCPoint p1,CCPoint p2){
    
    if( p2.equals(ccpAdd(p1,ccp(-1,0))) || p2.equals(ccpAdd(p1,ccp(1,0))) || p2.equals(ccpAdd(p1,ccp(0,-1))) || p2.equals(ccpAdd(p1,ccp(0,1))))
    {
        return true;
    }
    return false;
}

bool GameLayer::isValidPosition(CCPoint point){
	if (point.x < 0 || point.x >= kBoardWidth) return false;
	if (point.y < 0 || point.y >= kBoardHeight) return false;
    
    return true;
}


bool GameLayer::isValidMove(CCPoint p1,CCPoint p2){
    
    
    if(p1.x==p2.x && p1.y==p2.y){
        return false;
    }
    
    return true;
}
void GameLayer::swapCandiesWithAnimation(CCPoint p1, CCPoint p2, bool withCallbackCheck){
    
    
    GemSprite *emoji1 =(GemSprite *) gBoardSprites[(int)p1.x][(int)p1.y];
    GemSprite *emoji2 =(GemSprite *) gBoardSprites[(int)p2.x][(int)p2.y];
    
    
    if(emoji1==NULL || emoji2 ==NULL)
    {
        return;
    }
    
    int emoji1Type = gBoard[(int)p1.x][(int)p1.y];
    int emoji2Type = gBoard[(int)p2.x][(int)p2.y];
    
    
    
    gBoard[(int)p1.x][(int)p1.y] = emoji2Type;
    gBoard[(int)p2.x][(int)p2.y] = emoji1Type;
    gBoardSprites[(int)p1.x][(int)p1.y] = emoji2;
    gBoardSprites[(int)p2.x][(int)p2.y] = emoji1;
    
    
    CCPoint emoji1Position = ccp(kOffsetX+kGemSize/2 + (((int)p1.x)*(kGemSize+20.0)), kOffsetY+kGemSize/2 + (int)(((int)p1.y)*kGemSize));
    CCPoint emoji2Position = ccp(kOffsetX+kGemSize/2+ (((int)p2.x)*(kGemSize+20.0)), kOffsetY+kGemSize/2 + (int)(((int)p2.y)*kGemSize));
    
    CCMoveTo * movAction1 = CCMoveTo::create(0.2,emoji2Position);
    CCJumpTo * jumpAction = CCJumpTo::create(0.3, emoji2Position, 40.0, 1);
    CCMoveTo * movAction2 = CCMoveTo::create(0.3,emoji1Position);
    emoji2->runAction(movAction2);

    if(withCallbackCheck){
        
        CCFiniteTimeAction *actionDelay = CCDelayTime::create(0.001);

        CCSequence * seqAction1 = CCSequence::create(jumpAction,actionDelay,CCCallFuncN::create(this, callfuncN_selector(GameLayer::checkGridForCombinations)),NULL);
        emoji1->runAction(seqAction1);
    }
    else{
        emoji1->runAction(jumpAction);
    }
    this->reorderChild(emoji1, zIndexCandies-p2.y);
    this->reorderChild(emoji2, zIndexCandies-p1.y);
    
}


bool GameLayer::isValidEmojiCombinationAt (CCPoint p1,CCPoint p2,CCPoint p3, int type ,int board[][kBoardHeight]){
    if(isValidPosition(p1) && isValidPosition(p2) && isValidPosition(p3))
    {
        
        if(type == kLightningPowerUpTag)
        {
            if(board[(int)p1.x][(int)p1.y]== board[(int)p2.x][(int)p2.y])
            {
                powerUpMatchedWithType = board[(int)p1.x][(int)p1.y];
                return true;
            }
            else if(board[(int)p2.x][(int)p2.y]== board[(int)p3.x][(int)p3.y])
            {
                powerUpMatchedWithType = board[(int)p2.x][(int)p2.y];
                return true;
            }
            else if(board[(int)p1.x][(int)p1.y]== board[(int)p3.x][(int)p3.y])
            {
                powerUpMatchedWithType = board[(int)p1.x][(int)p1.y];
                return true;
            }

            return false;
        }
        
        
        else if(board[(int)p1.x][(int)p1.y]==board[(int)p2.x][(int)p2.y] && board[(int)p3.x][(int)p3.y]==type && board[(int)p2.x][(int)p2.y]==type)
        {
            return true;
        }
    }
    return false;
}



CCPoint GameLayer::getHintPoint(){
    
    
    int boardCopy[kBoardWidth][kBoardHeight];
    for(int i=0;i<kBoardWidth ; i++){
        for(int j=0;j<kBoardHeight ; j++){
            boardCopy[i][j] = gBoard[i][j];
        }
    }
    
    for(int j=(kBoardHeight-1);j>=0 ; j--){
        for(int i=0;i<kBoardWidth ; i++){
            
            //case 1
            CCPoint p1 = CCPoint(i,j);
            CCPoint p2 = ccpAdd(p1,ccp(1,0));
            if(isValidPosition(p2)){
                
                
                int tempValue = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = boardCopy[(int)p2.x][(int)p2.y];
                boardCopy[(int)p2.x][(int)p2.y] = tempValue;
                
                
                if(checkGridPositionForValidMoveOnBoard(p2,boardCopy))
                {
                    return p1;
                }
                boardCopy[(int)p2.x][(int)p2.y] = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = tempValue;
                
                
                
            }
            
            
            p2 = ccpAdd(p1,ccp(0,1));
            if(isValidPosition(p2)){
                
                
                int tempValue = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = boardCopy[(int)p2.x][(int)p2.y];
                boardCopy[(int)p2.x][(int)p2.y] = tempValue;
                
                
                if( checkGridPositionForValidMoveOnBoard(p2,boardCopy))
                {
                    return p1;
                }
                boardCopy[(int)p2.x][(int)p2.y] = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = tempValue;
                
                
            }
            
            
            
            p2 = ccpAdd(p1,ccp(-1,0));
            if(isValidPosition(p2)){
                
                int tempValue = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = boardCopy[(int)p2.x][(int)p2.y];
                boardCopy[(int)p2.x][(int)p2.y] = tempValue;
                
                //TODO: do process
                
                if( checkGridPositionForValidMoveOnBoard(p2,boardCopy))
                {
                    return p1;
                }
                boardCopy[(int)p2.x][(int)p2.y] = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = tempValue;
                
                
            }
            
            
            
            p2 = ccpAdd(p1,ccp(0,-1));
            if(isValidPosition(p2)){
                
                int tempValue = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = boardCopy[(int)p2.x][(int)p2.y];
                boardCopy[(int)p2.x][(int)p2.y] = tempValue;
                
                //TODO: do process
                
                if( checkGridPositionForValidMoveOnBoard(p2,boardCopy))
                {
                    return p1;
                }
                boardCopy[(int)p2.x][(int)p2.y] = boardCopy[(int)p1.x][(int)p1.y];
                boardCopy[(int)p1.x][(int)p1.y] = tempValue;
                
                
            }
        }
    }
    
    
    return   ccp(-1,-1);
}

bool GameLayer::checkGridPositionForValidMoveOnBoard(CCPoint point,int board[][kBoardHeight]){
    int emojiType = board[(int)point.x][(int)point.y];
    
    
    if(emojiType <= -1 && emojiType >= 5){
        return false;
    }
    
    //comb 1
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(1,0)), ccpAdd(point,ccp(2,0)), emojiType,board)){
        return true;
    }
    
    //comb 2
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(0,-1)), ccpAdd(point,ccp(0,-2)), emojiType,board)){
        return true;
    }
    
    //comb 3
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(-1,0)), ccpAdd(point,ccp(-2,0)), emojiType,board)){
        return true;
    }
    
    //comb 4
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(0,1)), ccpAdd(point,ccp(0,2)), emojiType,board)){
        return true;
    }
    
    //comb 5
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(-1,0)), ccpAdd(point,ccp(1,0)), emojiType,board))
        return true;
    
    //comb 6
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(0,-1)), ccpAdd(point,ccp(0,1)), emojiType,board))
        return true;
    
    
    
    return false;
    
}

bool GameLayer::checkGridPositionForValidMove(CCPoint point){
    
    
    int emojiType = gBoard[(int)point.x][(int)point.y];
    
    
    if(emojiType <= -1 && emojiType >= kGameUniqueCandies){
        return false;
    }
    
    //comb 1
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(1,0)), ccpAdd(point,ccp(2,0)), emojiType,gBoard)){
        return true;
    }
    
    //comb 2
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(0,-1)), ccpAdd(point,ccp(0,-2)), emojiType,gBoard)){
        return true;
    }
    
    //comb 3
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(-1,0)), ccpAdd(point,ccp(-2,0)), emojiType,gBoard)){
        return true;
    }
    
    //comb 4
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(0,1)), ccpAdd(point,ccp(0,2)), emojiType,gBoard)){
        return true;
    }
    
    //comb 5
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(-1,0)), ccpAdd(point,ccp(1,0)), emojiType,gBoard))
        return true;
    
    //comb 6
    if(isValidEmojiCombinationAt(point, ccpAdd(point,ccp(0,-1)), ccpAdd(point,ccp(0,1)), emojiType,gBoard))
        return true;
    
    
    
    return false;
    
}

void GameLayer::checkGridForCombinations(CCNode *sender){
    bool moved1 = checkGridPositionForValidMove(movePoint1);
    bool moved2;
    if(moved1){
        movesAllowed_--;
        movesLabel->setString(CCString::createWithFormat("%i",movesAllowed_)->getCString());
        removeConnectedGems(CCArray::create(CCInteger::create(movePoint1.x), CCInteger::create(movePoint1.y),NULL));
        
    }
    else{
        moved2 = checkGridPositionForValidMove(movePoint2);
        if(moved2){
            movesAllowed_--;
            movesLabel->setString(CCString::createWithFormat("%i",movesAllowed_)->getCString());
            removeConnectedGems(CCArray::create(CCInteger::create(movePoint2.x), CCInteger::create(movePoint2.y),NULL));
        }
        
    }
    
    if(!moved1 && !moved2){
        swapCandiesWithAnimation(movePoint1,movePoint2, false);
    }
    
    isMakingMove_ = false;

}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if(isMakingMove_  || !startTouchEnable)
        return false;
    
    if (!gIsGameOver)
    {
        gLastMoveTime  = nowInSeconds();
        
        
        gHintLayer->removeAllChildrenWithCleanup(true);
		gIsDisplayingHint = false;
        
        CCPoint  pointBegin = pTouch->getLocationInView();
        pointBegin = CCDirector::sharedDirector()->convertToGL(pointBegin);
        touchBegan_ = pointBegin;
        
        int x = floor((pointBegin.x-kOffsetX)/(kGemSize+20.0) );
        int y = floor((pointBegin.y-kOffsetY)/kGemSize);
        CCSprite * currentGemTile = gBoardSprites[x][y];
        
        
        if(!this->isValidPosition(ccp(x,y))){
            return false;
        }
        if(selectionSprite_== NULL){
            selectionSprite_ = CCSprite::create(HINT_PNG);
            selectionSprite_->setScale(0.75);
            selectionSprite_->setOpacity(0);
            this->addChild(selectionSprite_,zIndexEmojiBG);
        }
        
        if(!isSelected_){
            selectionSprite_->setOpacity(100);
            selectionSprite_->setPosition(ccp(kOffsetX+kGemSize/2 + x*(kGemSize+20.0), kOffsetY+kGemSize/2 + y*kGemSize));
            currentGemTile->setColor(ccWHITE);
            isSelected_ = true;
            CCActionInterval * rot1 = CCRotateTo::create(0.1, 5.0);
            CCActionInterval * rot2 = CCRotateTo::create(0.1, -5.0);
            CCAction * act = CCRepeatForever::create(CCSequence::create(rot1,rot2,NULL));
            act->setTag(kShakeActionTag);
            currentGemTile->runAction(act);

        }
        else
        {
            CCPoint oldPosition = selectionSprite_->getPosition();
            
            int oldX = (int)floorf((oldPosition.x-kOffsetX)/(kGemSize+20.0));
            int oldY = (int)floorf((oldPosition.y-kOffsetY)/kGemSize);
            CCSprite * oldGemTile = gBoardSprites[oldX][oldY];
            oldGemTile->stopActionByTag(kShakeActionTag);
            
            if(!areAdjacent(ccp(x,y), ccp(oldX,oldY))){
                selectionSprite_->setOpacity(100);
                selectionSprite_->setPosition(ccp(x*(kGemSize+20.0)+kOffsetX+kGemSize/2, y*kGemSize+kOffsetY+kGemSize/2));
                CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage(TILE_SELECTED_IMAGE);
                oldGemTile->setTexture(tex);
               // oldGemTile->setColor(ccBLACK);
                currentGemTile->setColor(ccWHITE);
                
            }
            else{
                //TODO:playmove
                CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage(TILE_SELECTED_IMAGE);
                oldGemTile->setTexture(tex);
                
                if(isMakingMove_)
                    return false;
                isMakingMove_ = true;
                movePoint1 = ccp(x,y);
                movePoint2 = ccp(oldX,oldY);
                
                swapCandiesWithAnimation(ccp(x,y), ccp(oldX,oldY), true);
                selectionSprite_->setOpacity(0);
                isSelected_ = false;
                
            }
            
        }
        
    }
    
 
    

    
    
    
    
    
    
    return true;
}

void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
    if(isMakingMove_)
        return ;
    
    if (!gIsGameOver && isSelected_)
    {
        
        CCPoint  pointMoved = pTouch->getLocationInView();
        pointMoved = CCDirector::sharedDirector()->convertToGL(pointMoved);
        
        float distance = ccpDistance(pointMoved, touchBegan_);
        if( distance>=kGemSize ){
            
            
            int distance = kGemSize;
            
            
            int xMovement = pointMoved.x-touchBegan_.x;
            int yMovement = pointMoved.y-touchBegan_.y;
            
            bool xSwipeComplete = abs(xMovement)>distance;
            bool ySwipeComplete = abs(yMovement)>distance;
            
            if(xSwipeComplete && ySwipeComplete){
                //diagonal
            }
            
            int x,y;
            CCPoint oldPosition = selectionSprite_->getPosition();
            
            int oldX = (int)floorf((oldPosition.x-kOffsetX)/(kGemSize+20.0));
            int oldY = (int)floorf((oldPosition.y-kOffsetY)/kGemSize);
            
            if(xSwipeComplete){
                
                if(xMovement>0){
                    x = oldX+1;
                    y = oldY;
                }
                else{
                    x = oldX-1;
                    y = oldY;
                }
            }
            else{
                if(yMovement>0){
                    x = oldX;
                    y = oldY+1;
                }
                else{
                    x = oldX;
                    y = oldY-1;
                }
                
                
            }
            
            if(!isValidPosition(CCPoint(x,y))){
                return;
            }
            
            CCSprite * oldGemTile = gBoardSprites[oldX][oldY];
            oldGemTile->stopActionByTag(kShakeActionTag);
            oldGemTile->setColor(ccBLACK);
            isMakingMove_ = true;
            movePoint1 = ccp(x,y);
            movePoint2 = ccp(oldX,oldY);
            
            swapCandiesWithAnimation(ccp(x,y), ccp(oldX,oldY), true);
            selectionSprite_->setOpacity(0);
            isSelected_ = false;
        }
        
    }
    
    
}

void GameLayer::GameOverScreen()
{
    this->unschedule(schedule_selector(GameLayer::GameOverScreen));
    if(GAME_SHARED_HANDLER->addLevelProgress(localScore,gameLevel_)){
    }
    CCLog("shauket:: in GameLayer");
    GAME_SHARED_HANDLER->updateScoreToServer();
    CCLog("shauket:: in GameLayer2");

    
    CCScene *endGameScene = EndGameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(endGameScene);
    
}

void GameLayer::onEnter()
{
    CCLayer::onEnter();
}

void GameLayer::onExit()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(SPLASH_PLIST);
    CCLayer::onExit();
}


# pragma mark - junaid addition
void GameLayer::shakeSpritesRandomly()
{
    if(!isMakingMove_)
    {
        int x = arc4random()% kBoardWidth;
        int y = arc4random()% kBoardHeight;
//        CCLog(" junaid: x= %d  y = %d ",x,y);
        
        CCActionInterval * rot1 = CCRotateTo::create(0.1, 5.0);
        CCActionInterval * rot2 = CCRotateTo::create(0.1, -5.0);
        CCActionInterval * rot3 = CCRotateTo::create(0.1, 0.0);
        CCAction * act = CCSequence::create(rot1,rot2,rot1,rot2,rot3,NULL);
        CCSprite * tempSprite = gBoardSprites[x][y];
        if(tempSprite)
            tempSprite->runAction(act);
    }
    
}
void GameLayer::enablelightningPowerUp()
{
    lightningPowerAdded = false;
}
void GameLayer::removeLightningEffect()
{
    for (int i=0;i<lightningArray->count(); i++)
    {
        FXLightning * obj = (FXLightning*)lightningArray->objectAtIndex(i);
        this->removeChild(obj, true);
    }
    for (int i=0;i<flashesArray->count(); i++)
    {
        CCSprite * obj = (CCSprite*)flashesArray->objectAtIndex(i);
        this->removeChild(obj, true);
    }
    lightningArray->removeAllObjects();
    flashesArray->removeAllObjects();
    lightningArray->release();
    flashesArray->release();
    schedule(schedule_selector(GameLayer::update), 0.0167);
}
