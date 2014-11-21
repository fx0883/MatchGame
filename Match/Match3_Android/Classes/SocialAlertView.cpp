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

#include "SocialAlertView.h"
#include "GameConstants.h"
using namespace cocos2d;

#define SHOP_IMAGE "SocialUI/btn_shop.png"
#define HELP_GAME_LFIE "Help me with a game life :("
#define ASK_FRIEMD_IMAGE "SocialUI/btn_askfriends.png"
#define SEETING_BG "SocialUI/SettingLayer/settingBg.png"
#define SHOP_IMAGE_PRESSED "SocialUI/btn_shop_pressed.png"
#define ASK_FRIEMD_IMAGE_PRESSED "SocialUI/btn_askfriends_pressed.png"
#define MSG_FOR_LIFE "Ask your facebook frind \n for more live or check the shop"

CCScene* SocialAlertView::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SocialAlertView *layer = SocialAlertView::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SocialAlertView::init()
{
    //////////////////////////////
    // 1. super init first
    if (!cocos2d::CCLayer::init())
    {
        return false;
    }
    winSize = TARGET_DESIGN_RESOLUTION_SIZE;
    this->setTouchEnabled(true);
    return true;
}

void SocialAlertView:: onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void SocialAlertView:: initWithMessageAndMenus(const char* msg,const char* okButtonTitle,const char* cancelButtonTitle,bool okButtonVisible,bool cancelButtonVisible){
    
    setupGUI();
    CCLabelTTF *lableDescription = CCLabelTTF::create(msg, GAME_LABEL_FONT_NATHAN, 40.0, CCSizeMake(450, 200), kCCTextAlignmentCenter);
    lableDescription->setPosition(ccp(winSize.width/2-(40),winSize.height/2 -(160)));
    
    background->addChild(lableDescription);
    
    CCMenuItemImage * okMeunuItem = CCMenuItemImage::create(BUTTON_START_GAME, BUTTON_START_GAME_PRESSED, this, menu_selector(SocialAlertView::okButtonPressed));
    okMeunuItem->setVisible(okButtonVisible);
    
    CCMenuItemImage * cancelMeunuItem = CCMenuItemImage::create(BUTTON_START_GAME, BUTTON_START_GAME_PRESSED, this, menu_selector(SocialAlertView::cancelButtonPressed));
    cancelMeunuItem->setVisible(cancelButtonVisible);
    
    CCLabelTTF *okLabel = CCLabelTTF::create(okButtonTitle, GAME_LABEL_FONT_NATHAN, 40.0,CCSizeMake(okMeunuItem->getContentSize().width, okMeunuItem->getContentSize().height), kCCTextAlignmentCenter);
    okLabel->setPosition(ccp(okMeunuItem->getContentSize().width/2, okMeunuItem->getContentSize().height/2 -(40)));
    okMeunuItem->addChild(okLabel);
    
    CCLabelTTF *cancelLabel = CCLabelTTF::create(cancelButtonTitle, GAME_LABEL_FONT_NATHAN, 40.0,CCSizeMake(cancelMeunuItem->getContentSize().width, cancelMeunuItem->getContentSize().height), kCCTextAlignmentCenter);
    cancelLabel->setPosition(ccp(cancelMeunuItem->getContentSize().width/2, cancelMeunuItem->getContentSize().height/2 -(40)));
    cancelMeunuItem->addChild(cancelLabel);
    
    CCMenu * Menu = CCMenu::create(okMeunuItem,cancelMeunuItem,NULL);
    Menu->setPosition(CCPointZero);
    background->addChild(Menu);
   
    
    if ((okButtonVisible && cancelButtonVisible) == true) {
        okMeunuItem->setPosition(ccp(winSize.width/2-(170), winSize.height/2-(355)));
        cancelMeunuItem->setPosition(ccp(winSize.width/2+(90), winSize.height/2-(355)));
    }
    else if (okButtonVisible)
    {
        okMeunuItem->setPosition(ccp(winSize.width/2 -(20), winSize.height/2-(355)));
        cancelMeunuItem->setEnabled(false);
    }
    else
    {
        
    }

}
void SocialAlertView::okButtonPressed(CCObject *pSender)
{
    this->removeFromParent();
    
}

void SocialAlertView::cancelButtonPressed(CCObject *pSender)
{
    this->removeFromParent();
}
void SocialAlertView::settingPopUP()
{
    setupGUI();

}
void SocialAlertView::setupGUI()
{
    //add bg.
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    background = CCSprite::create(SEETING_BG);
    background->setPosition( ccp(windowSize.width/2, windowSize.height/2) );
    this->addChild(background);
    
    CCMenuItemImage *closeBttuon = CCMenuItemImage::create(BUTTON_CLOSE_NORMAL, BUTTON_CLOSE_PRESSED, this, menu_selector(SocialAlertView::closeButtonPressed));
    CCMenu *closeMenu = CCMenu::create(closeBttuon,NULL);
    closeMenu->setPosition(CCPointZero);
    closeBttuon->setPosition(ccp(windowSize.width/2+(240), windowSize.height/2+(240)));
    this->addChild(closeMenu);
}

void SocialAlertView::lifePopUp() // it open when user need more lives
{
    setupGUI();
    CCLabelTTF *moreLiveLabel = CCLabelTTF::create("More Lives", GAME_LABEL_FONT_NATHAN, 60.0);
    moreLiveLabel->setPosition(ccp(winSize.width/2-(40),winSize.height/2));
    background->addChild(moreLiveLabel);
    CCLabelTTF *MoreLiveLabelDescription = CCLabelTTF::create(MSG_FOR_LIFE, GAME_LABEL_FONT_NATHAN, 40.0, CCSizeMake(450, 200), kCCTextAlignmentCenter);//CCLabelTTF::create("More Lives", GAME_LABEL_FONT_NATHAN, 40.0);
    MoreLiveLabelDescription->setPosition(ccp(winSize.width/2-(45),winSize.height/2 -(160)));

    background->addChild(MoreLiveLabelDescription);
    
    CCMenuItemImage * askFbButton = CCMenuItemImage::create(ASK_FRIEMD_IMAGE, ASK_FRIEMD_IMAGE_PRESSED, this, menu_selector(SocialAlertView::askButtonPressed));
    CCMenuItemImage * shopButton = CCMenuItemImage::create(SHOP_IMAGE, SHOP_IMAGE_PRESSED, this, menu_selector(SocialAlertView::shopButtonPressed));
    
    
    CCMenu * toolMenu = CCMenu::create(askFbButton,shopButton,NULL);
    toolMenu->setPosition(CCPointZero);
    background->addChild(toolMenu);
    askFbButton->setPosition(ccp(winSize.width/2-(170), winSize.height/2-(355)));
    shopButton->setPosition(ccp(winSize.width/2+(90), winSize.height/2-(355)));

}
void SocialAlertView::closeButtonPressed()
{
    this->removeFromParent();
}
void SocialAlertView::askButtonPressed()
{
   // if (GAME_SHARED_HANDLER->getisNetWorkAvailAble() == false) {
//	 if (!CPP_FacebookGameController::getInstance()->checkNetworkStatus()) {
//
//        SocialAlertView *alertView = SocialAlertView::create();
//        alertView->initWithMessageAndMenus(NET_WORK_ERROR, "Ok", "cancel", true, true);
//        this->addChild(alertView);
//        return;
//    }
//
//
//    CPP_FacebookGameController::getInstance()->FB_SendRequest(0, kReqTypeAskLife,CCString::create(HELP_GAME_LFIE),NULL);
}
void SocialAlertView::shopButtonPressed()
{

}
void SocialAlertView::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

#pragma touches Delegate

bool SocialAlertView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void SocialAlertView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}
void SocialAlertView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}
void SocialAlertView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}
