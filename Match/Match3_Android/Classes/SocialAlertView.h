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

#ifndef __SocialPuzle__SocialAlertView__
#define __SocialPuzle__SocialAlertView__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class SocialAlertView: public CCLayer
{
private:
    CCSprite* background;
    void setupGUI();
public:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSize winSize;
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    virtual void onEnter();
    virtual void onExit();
    
    //function
    void initWithMessageAndMenus(const char* msg,const char* okButtonTitle,const char* cancelButtonTitle,bool okButtonVisible,bool cancelButtonVisible);
    void okButtonPressed(CCObject *pSender);
    void cancelButtonPressed(CCObject *pSender);
    void closeButtonPressed();
    void askButtonPressed();
    void shopButtonPressed();
    void settingPopUP();
    void lifePopUp();

    
    // implement the "static node()" method manually
    CREATE_FUNC(SocialAlertView);
};

#endif /* defined(__SocialPuzle__SocialAlertView__) */
