#ifndef __DEMO_SCENE_H__
#define __DEMO_SCENE_H__
#include "Banner.h"
#include "Interstitial.h"
#include "NativeAd.h"

#include "cocos2d.h"


namespace com_ucweb_union_ads_sdk{
	class AdListener;
}
using namespace com_ucweb_union_ads_sdk;
namespace cocos2d{
	class Ref;
	class Label;
	namespace ui{
		class ImageView;
	}
}
namespace com_ucweb_union_ads_demo {
class DemoScene : public cocos2d::Layer
{
public:
	DemoScene();
	~DemoScene();
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(DemoScene);
private:
    void showBanner(Ref* ref, int x, int y, int w, int h);
    void showInterstitial(Ref* ref);
    void showNativeAd(Ref* ref);
    void exitApp(Ref* ref);
    void nativeAdLoaded();
    void updateIcon(bool success, const std::string& savePath);
    void updateCover(bool success, const std::string& savePath);
private:
	Banner mBanner;
	Interstitial mInterstitial;
	NativeAd mNativeAd;
	AdListener* mBannerAdListener;
	AdListener* mInterstitialAdListener;
	AdListener* mNativeAdListener;

	cocos2d::Label* mTitle;
	cocos2d::Label* mDescription;
	cocos2d::ui::ImageView* mIcon;
	cocos2d::ui::ImageView* mCover;
};
}//namespace com_ucweb_union_ads_demo
#endif // __DEMO_SCENE_H__
