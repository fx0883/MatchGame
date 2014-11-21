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

#include "PlayMenu.h"


#include "SimpleAudioEngine.h"

#include "GameConstants.h"
#include "GameLayer.h"
#include "GameUtils.h"
#include "GameState.h"

#define FACEBOOK_LOGIN_IMAGE_PRESSED "SocialUI/btn_fblogin_pressed.png"
#define FACEBOOK_LOGOUT_IMAGE_PRESSED "SocialUI/btn_fblogout_pressed.png"
#define ENVLOPE_IMAGE_PRESSED  "SocialUI/FriendHelp/envelope_pressed.png"
#define ENVLOPE_IMAGE  "SocialUI/FriendHelp/envelope.png"
#define FACEBOOK_LOGIN_IMAGE "SocialUI/btn_fblogin.png"
#define FACEBOOK_LOGOUT_IMAGE "SocialUI/btn_fblogout.png"
#define PLAY_IMAGE_PRESSED "SocialUI/btn_play_pressed.png"
#define PLAY_IMAGE "SocialUI/btn_play.png"
#define BACKGROUND "SocialUI/background.png"
#define LOGO "SocialUI/img_logo.png"
#define LIFE_COUNT_STRING "Life Count %i"


using namespace cocos2d;

void PlayMenu:: onEnter()
{
  
  CCLayer::onEnter();
}


CCScene* PlayMenu::scene()
{
    // 'scene' is an autorelease object
  CCScene *scene = CCScene::create();
  
    // 'layer' is an autorelease object
  PlayMenu *layer = PlayMenu::create();
  
    // add layer as a child to scene
  scene->addChild(layer);
  
    // return the scene
  return scene;
}

  // on "init" you need to initialize your instance
bool PlayMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if (!cocos2d::CCLayer::init())
    {
        return false;
    }
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
//    winSize = TARGET_DESIGN_RESOLUTION_SIZE;
    winSize = CCDirector::sharedDirector()->getWinSize();

    setupGUI();
    this->setKeypadEnabled(true);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("MainMenuMusic.mp3",true);
  return true;
}

void PlayMenu:: keyBackClicked(void) {
    CCLayer::keyBackClicked();
    CCDirector::sharedDirector()->end();
}
void PlayMenu::timerForLifeCheck()
{
    float totalSec=GAME_SHARED_HANDLER->getRemainingTimeForLife();
   
    if (totalSec<=0)
    {
        GAME_SHARED_HANDLER->timerForLifeCheck();

    }
    
    int hour=(int)totalSec/3600;
    totalSec=totalSec-3600*hour;
    
    int minute=(int)totalSec/60;
    
    totalSec=(int)totalSec-60*minute;

    
    
    string hourString = static_cast<ostringstream*>( &(ostringstream() << hour) )->str();
    string minuteString = static_cast<ostringstream*>( &(ostringstream() << minute) )->str();
    string SecondString = static_cast<ostringstream*>( &(ostringstream() << totalSec) )->str();

    SecondString=hourString + ":" + minuteString+":" + SecondString ;
    timerForLife->setString(SecondString.c_str());
    
   
    
}
void PlayMenu::play()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("ButtonClick.mp3");
    CCScene *scene = CCScene::create();
    GameLayer *gameLayer = GameLayer::create();
    gameLayer->GameStart();
    scene->addChild(gameLayer);
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5,scene));

}
void PlayMenu:: onExit()
{
    CCLayer::onExit();
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

}
void PlayMenu::setupGUI()
{
//    this->schedule(schedule_selector(PlayMenu::timerForLifeCheck),1/60);
    {
        CCLabelTTF*  timerLabel=  CCLabelTTF::create(NEXT_LIFE_IN, GAME_LABEL_FONT_NATHAN, 29);
        //  coinsLabel->setColor(ccc3(87, 0, 5));
        timerLabel->setPosition(ccp(winSize.width/2-170,winSize.height/2+430));
        timerLabel->setColor(ccBLUE);
        timerLabel->setTag(KTAG_PLYMENU_COINSLBL);
//        this->addChild(timerLabel,11-KTAG_LAYERING_PARAM);

    }
    timerForLife=  CCLabelTTF::create("", GAME_LABEL_FONT_NATHAN, 29);
    //  coinsLabel->setColor(ccc3(87, 0, 5));
    timerForLife->setPosition(ccp(winSize.width/2-20,winSize.height/2+430));
    timerForLife->setColor(ccBLACK);
    timerForLife->setTag(KTAG_PLYMENU_COINSLBL);
//    this->addChild(timerForLife,11-KTAG_LAYERING_PARAM);
/// Total Life Left
//    {
//        CCLabelTTF*  TotalLifeLabel=  CCLabelTTF::create("", GAME_LABEL_FONT_NATHAN, 29);
//        //  coinsLabel->setColor(ccc3(87, 0, 5));
//        TotalLifeLabel->setPosition(ccp(winSize.width-120,winSize.height/2+430));
//        TotalLifeLabel->setColor(ccBLUE);
//        TotalLifeLabel->setTag(KTAG_Life_Count);
//        this->addChild(TotalLifeLabel,11-KTAG_LAYERING_PARAM);
//        string TotalLifeCount = static_cast<ostringstream*>( &(ostringstream() << GAME_SHARED_HANDLER->getLives()) )->str();
//        TotalLifeCount="Life Count " + TotalLifeCount;
//        TotalLifeLabel->setString(TotalLifeCount.c_str());
//        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PlayMenu::updateLifeCountLabel),N_LIFE_COUNT_UPDATED,NULL);
//    }
    
    
    //add bg.
    
    CCSprite* bckg = CCSprite::create(BACKGROUND);
    bckg->setPosition( ccp(winSize.width/2,winSize.height/2) );
    CCLOG("win size %f ",winSize.width);
    this->addChild(bckg, 0-KTAG_LAYERING_PARAM);
    
//    CCSprite *logobg = CCSprite::create(LOGO);
//    logobg->setPosition(ccp(winSize.width/2, winSize.height/2+(155)));
//    this->addChild(logobg);
    
    CCMenuItemImage *playMenuItem = CCMenuItemImage::create(PLAY_IMAGE, PLAY_IMAGE_PRESSED, this,menu_selector(PlayMenu::play));
//    facebookMenuItem =  CCMenuItemImage::create(FACEBOOK_LOGIN_IMAGE, FACEBOOK_LOGIN_IMAGE_PRESSED, this,menu_selector(PlayMenu::facebookConnect));
    
    
    
    CCMenu *menu = CCMenu::create(playMenuItem,NULL);
    
    menu->setPosition(ccp(0, 0));
    playMenuItem->setPosition(ccp(winSize.width/2, winSize.height/2-(275)));
//    facebookMenuItem->setPosition(ccp(winSize.width/2, winSize.height/2-(325)));
    
    this->setFBStatus();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PlayMenu::setFBStatus),N_USER_LOG_STATUS_CHANGED,NULL);
    

    this->checkAndAddRequestsMenu();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PlayMenu::checkAndAddRequestsMenu),N_REQUESTS_DATA_UPDATED,NULL);

    
    
    
    
    this->addChild(menu);
    
    
}

void PlayMenu::checkAndAddRequestsMenu(){
    
//    bool shouldAdd = false;
//    CCArray * pendingRequest = CPP_FacebookGameController::getInstance()->FB_GetAllPendingRequestsArray();
//    if(pendingRequest!=NULL && pendingRequest->count()>0){
//        shouldAdd = true;
//    }
//   
//    if(shouldAdd){
//        if( pendingRequestMenu_==NULL){
//            CCMenuItemImage *facebookPendingReq = CCMenuItemImage::create(ENVLOPE_IMAGE, ENVLOPE_IMAGE_PRESSED, this,menu_selector(PlayMenu::faceBookPendingReq));
//            facebookPendingReq->setPosition(ccp(winSize.width/2 -(255), 55));
//            pendingRequestMenu_ = CCMenu::create(facebookPendingReq,NULL);
//            pendingRequestMenu_->setPosition(ccp(0, 0));
//            this->addChild(pendingRequestMenu_);
//        }
//    }
//    else{
//        if( pendingRequestMenu_!=NULL){
//            this->removeChild(pendingRequestMenu_);
//            pendingRequestMenu_=NULL;
//        }
//    }
//
    
    
    
}

void PlayMenu::updateLifeCountLabel(){
    CCLabelTTF *lifeCountLabel = (CCLabelTTF *) this->getChildByTag(KTAG_Life_Count);
    string TotalLifeCount = static_cast<ostringstream*>( &(ostringstream() << GAME_SHARED_HANDLER->getLives()) )->str();
    TotalLifeCount="Life Count " + TotalLifeCount;
    lifeCountLabel->setString(TotalLifeCount.c_str());
}



void PlayMenu::setFBStatus(){
//    checkAndAddRequestsMenu();
//    if(!CPP_FacebookGameController::getInstance()->FB_IsLogedIn()){
//        CCLog("shauket:: set login images");
//
//        facebookMenuItem->setNormalImage(CCSprite::create(FACEBOOK_LOGIN_IMAGE));
//        facebookMenuItem->setSelectedImage(CCSprite::create(FACEBOOK_LOGIN_IMAGE_PRESSED));
//    }
//    else{
//        CCLog("shauket:: set logout images");
//
//        facebookMenuItem->setNormalImage(CCSprite::create(FACEBOOK_LOGOUT_IMAGE));
//        facebookMenuItem->setSelectedImage(CCSprite::create(FACEBOOK_LOGOUT_IMAGE_PRESSED));
//    }
//    
}

void PlayMenu::faceBookPendingReq()
{
   // if (GAME_SHARED_HANDLER->getisNetWorkAvailAble() == false) {
//	 if (!CPP_FacebookGameController::getInstance()->checkNetworkStatus()) {
//
//        
//        SocialAlertView *alertView = SocialAlertView::create();
//        alertView->initWithMessageAndMenus(NET_WORK_ERROR, "Ok", "cancel", true, true);
//        this->addChild(alertView);
//        return;
//    }
//    
//    if(CPP_FacebookGameController::getInstance()->FB_IsLogedIn()){
//
//        FBRequestLayer * reqLayer = FBRequestLayer::create();
//        this->addChild(reqLayer,100);
//
//    }
}
void PlayMenu::facebookConnect()
{
   // if (GAME_SHARED_HANDLER->getisNetWorkAvailAble() == false) {
//	 if (!CPP_FacebookGameController::getInstance()->checkNetworkStatus()) {
//		 CCLog("shauket:: facebookconnect 1");
//        SocialAlertView *alertView = SocialAlertView::create();
//        alertView->initWithMessageAndMenus(NET_WORK_ERROR, "Ok", "cancel", true, true);
//        this->addChild(alertView);
//        return;
//    }
//
//	 CCLog("shauket:: facebookconnect 2");
//    if(!CPP_FacebookGameController::getInstance()->FB_IsLogedIn())
//    {
//        CPP_FacebookGameController::getInstance()->FB_Login();
//    }
//    else{
//        CPP_FacebookGameController::getInstance()->FB_Logout();
//
//    }
}

