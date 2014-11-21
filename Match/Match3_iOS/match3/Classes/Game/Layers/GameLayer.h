/****************************************************************************
 

 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ****************************************************************************/

#ifndef __match3__GameLayer__
#define __match3__GameLayer__

#include <iostream>
#include "GameConstants.h"
#include "cocos2d.h"
using namespace cocos2d;

class GameLayer : public cocos2d::CCLayer
{
private:
    int gameLevel_;
    int scoreTarget_;
    int movesAllowed_;
    
    
    float kOffsetX;
    float kOffsetY;
    CCSize windowSize;
    
    bool finalGameOverScreen;
    int localScore;
    CCSize winSize;
    CCSprite* bgLayerSprite;
    CCLabelBMFont* movesLabel;
    CCSprite* splashSprite_;
public:
    
    bool startTouchEnable;
    
    void fallingGem();
    void StartTouch();
    void GameStart();
    void GameOverScreen();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameLayer);
    
    void setupBoard();
    void removeMarkedGems();
//    bool removeConnectedGems(int x, int y);
    bool removeConnectedGems(CCArray * params);
    
    void onRemoveFromParent(CCNode * value);
    void pasueButtonPressed();
    int findMove();
    void displayHint();
    void removSplash(CCObject *sender);
    void createRandomMove();
    void createGameOver();
    void updateGameOver();
    double nowInSeconds();
    bool isValidPosition(CCPoint point);
    void swapCandiesWithAnimation(CCPoint p1, CCPoint p2, bool callback);
    void checkGridForCombinations(CCNode *sender);
    bool isValidMove(CCPoint p1,CCPoint p2);
    bool checkGridPositionForValidMove(CCPoint point);
    bool checkGridPositionForValidMoveOnBoard(CCPoint point,int board[][kBoardHeight]);
    CCPoint getHintPoint();
    bool isValidEmojiCombinationAt (CCPoint p1,CCPoint p2,CCPoint p3, int type, int board[][kBoardHeight]);
    int getGemType(int x, int y);
    void setGemType(int x, int y, int newType);
    
    CCArray * findConnectedGems(int x, int y);
    void findConnectedGems_(int x, int y, CCArray * arr, int gemType);
    virtual void update(float fDelta);
    
    bool areAdjacent(CCPoint p1,CCPoint p2);
    
    
    CCArray *currentEmojiFramesList_;
    CCSprite *gBoardSprites[kBoardWidth][kBoardHeight];
    int gBoard[kBoardWidth][kBoardHeight];
    int gNumGemsInColumn[kBoardWidth];
    int gTimeSinceAddInColumn[kBoardWidth];
    CCArray *gFallingGems[kBoardWidth];
    CCArray *gGameOverGems;
    
    bool gBoardChangedSinceEvaluation;
    int gPossibleMove;
    bool gIsGameOver;
    double gStartTime;
    long gLastMoveTime;
    bool gIsDisplayingHint;
    int gNumConsecutiveGems;
    bool gEndTimerStarted;
    void addSplashParticleEffect(int x, int y);
    int gScore;
    int gLastScore;
    CCLabelTTF *gScoreLabel;
    CCParticleSystemQuad *gPowerPlayParticles;
    CCLayerColor *gPowerPlayLayer;
    //sprtHeader
    CCProgressTimer*saveMeTimer;
    CCLabelBMFont *coinLabel_;
    CCProgressTimer *gTimer;
    CCNode * gHintLayer;
    CCSprite *selectionSprite_;
    bool isSelected_;
    CCPoint movePoint1;
    CCPoint movePoint2;
    bool isMakingMove_;
    CCPoint touchBegan_;
    
    void shakeSpritesRandomly();
    void enablelightningPowerUp();
    bool lightningPowerAdded;
    int powerUpMatchedWithType;
    CCArray * lightningArray;
    CCArray * flashesArray;
    void removeLightningEffect();
    
};
#endif /* defined(__match3__GameLayer__) */
