/****************************************************************************

 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ****************************************************************************/

//#import <Foundation/Foundation.h>
//#import "MObject.h"

#include "cocos2d.h"
//#import "CCNode.h"

USING_NS_CC;

#define K_NUMBER_OF_LIGHTNING_SAMPLES 60


typedef struct
{
	float x;
	float y;
	
} vec2;


typedef struct
{
	float x;
	float y;
	float z;
	
} vec3;

typedef struct
{
	float x;
	float y;
	float z;
	float w;
	
} vec4;

class FXLightning :public CCNode {
    
public:
    float tapper;
    float width;
    float spread;
    float speed;
    
	CCPoint start;
	CCPoint end;
    
    void render();

    bool initialize();
    virtual void draw();

private:
    CCGLProgram *pShaderProgram;
    ccVertex2F vtx[K_NUMBER_OF_LIGHTNING_SAMPLES];
	vec2 uvs[K_NUMBER_OF_LIGHTNING_SAMPLES];
    CCTexture2D * texture;

};



//@end
