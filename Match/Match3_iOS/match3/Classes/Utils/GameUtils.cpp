/****************************************************************************
 

 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ****************************************************************************/

#include "GameUtils.h"
#include "GameConstants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include "rapidjson.h"
#include "document.h"
#include "ParseUtil.h"
using namespace std;
using namespace rapidjson;

CCSize GameUtils::getDesignResolution() {

  return TARGET_DESIGN_RESOLUTION_SIZE;
}

int GameUtils::getIntFromCCString(CCString *str)
{
    
    return atoi(str->getCString());
    
}

CCString* GameUtils::getCCStringFromInt(int num)
{
    char key[100];
    sprintf(key,"%d",num);
    CCString *str = new CCString(key);
    str->autorelease();
    return str;
}

CCString * GameUtils::getCCString(const char* arr)
{
	CCString *str = new CCString(arr);
	str->autorelease();
	return str;
}

bool GameUtils::isStringEqual(const char *pStr1, const char *pStr2)
{
    return strcmp(pStr1, pStr2) == 0;
}

bool GameUtils::isValid(const char* value)
{
	if(!value || isStringEqual(value, ""))
	{
		return false;
	}
	return true;
}
std::string GameUtils::getStdStringFromInt(int value)
{
    char key[100];
    sprintf(key,"%d",value);
    return std::string(key);
}

std::string GameUtils::getStdStringFromLongInt(int64_t value)
{
	 CCLog("log:: vaulue %d",value);
    char key[100];
    sprintf(key,"%I64u",value);
    return std::string(key);
}

CCString* GameUtils::getCCStringFromDouble(double value)
{

	char key[100];
	sprintf(key, "%f", value);
	CCString *str = new CCString(key);
	str->autorelease();
	return str;
}

CCObject* GameUtils::convertDocumentToCObject(const char* data)
{
	if(!data)
	{
		CCLog("Zuluu:: Data is NULLLL");
	}
	Document document;
	if(data != NULL && !document.Parse<0>(data).HasParseError())
	{
		CCDictionary *makerInfoDict = (CCDictionary* ) ParseUtils::parse(document);
		makerInfoDict->retain();
		makerInfoDict->autorelease();
		return makerInfoDict;
	}
	else
	{
		CCLOG("Zuluu:: Document PArse ERROR %s ::%s",document.GetParseError(),data);
	}
	return NULL;
}
