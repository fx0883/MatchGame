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

#include "AppDelegate.h"
#include "cocos2d.h"
#include "GameConstants.h"
#include "GameLayer.h"
#include "PlayMenu.h"
#include "GameState.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
	CCLOG("shauket:: in finish lauching");
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);

     CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ResolutionPolicy policy =kResolutionExactFit;
//    if(winSize.height == 1024 || winSize.height == 2048){
//        policy = kResolutionShowAll;
//    }
//    policy = kResolutionShowAll;

    //******************************setting resource directories*****************************
    vector<string> searchPath;
    CCSize designResolutionSize = TARGET_DESIGN_RESOLUTION_SIZE;
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, policy);
    CCSize frameSize = pEGLView->getFrameSize();

    CCLog("shauket:: designResolutionSize szie %f designResolutionSize %f height",designResolutionSize.width,designResolutionSize.height);
    CCLog("shauket:: frame szie %f width %f height",frameSize.width,frameSize.height);
//    if (frameSize.height >= resource3.size.height)
//    {
//        searchPath.push_back(resource3.directory);
//        pDirector->setContentScaleFactor(MIN(resource3.size.height/designResolutionSize.height, resource3.size.width/designResolutionSize.width));
//    }
     if (frameSize.height >= resource2.size.height)
    {
    	 CCLog("shauket:: hd1");
        searchPath.push_back(resource2.directory);

        float scaleValue = MIN(resource2.size.height/designResolutionSize.height, resource2.size.width/designResolutionSize.width);

       pDirector->setContentScaleFactor(scaleValue);
    }
    else
    {
    	CCLog("shauket:: sd");
        searchPath.push_back(resource1.directory);
        pDirector->setContentScaleFactor(MIN(resource1.size.height/designResolutionSize.height, resource1.size.width/designResolutionSize.width));
    }

   // GAME_SHARED_HANDLER->schedule(schedule_selector(GAME_SHARED_HANDLER::timerForLifeCheck),1/60);
//     GameState::sharedGameState();
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    //******************************setting resource directories*****************************

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
//
    // create a scene. it's an autorelease object
    CCScene *pScene;
    pScene = PlayMenu::scene();
    // run
    
    pDirector->runWithScene(pScene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
    CCLog("BackEnd");
    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    //    ccGLInvalidateStateCache();
    //    CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
    //    ccDrawInit();
    //    CCTextureCache::reloadAllTextures();
    //    //CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
    //    CCDirector::sharedDirector()->setGLDefaultValues();
    
    CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    
    CCLog("ForeEnd");
    // if you use SimpleAudioEngine, it must resume here
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
