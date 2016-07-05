#include "DemoScene.h"

#include <ctime>
#include <cstring>
#include <string>
#include <list>

#include "ui/CocosGUI.h" 
#include "cocos/platform/CCFileUtils.h"

#include "ImageFetcher.h"
#include "AdSDK.h"
#include "Banner.h"
#include "Interstitial.h"
#include "NativeAd.h"
#include "AdRequest.h"
#include "impl/JNICommon.h"
#include "AdListener.h"
#include "AdError.h"
#include "Util.h"

namespace com_ucweb_union_ads_demo {
USING_NS_CC;
using cocos2d::ui::Button;
using cocos2d::ui::ImageView;
using namespace cocos2d::ui;
using namespace com_ucweb_union_ads_sdk;
using com_ucweb_union_ads_sdk::Image;
Scene* DemoScene::createScene()
{

    AdSDK::init();

    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DemoScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
namespace {
class BannerAdListener : public AdListener
{
public:
    BannerAdListener(Banner& banner) : mBanner(banner){}
    virtual ~BannerAdListener(){

    }
    virtual void onLoaded() override{
        LOGD("BannerAdListener::onLoaded");
        mBanner.show();
    }
    virtual void onClosed()  override{
         LOGD("BannerAdListener::onClosed"); 
    }
    virtual void onOpened() override{
          LOGD("BannerAdListener::onOpened");
    }
    virtual void onClicked() override{
         LOGD("BannerAdListener::onClicked");
    }
    virtual void onError(const AdError& error)  override{
        LOGD("BannerAdListener::onError");  
    }
private:
    Banner& mBanner;
};

class InterstitialAdListener : public AdListener
{
public:
    InterstitialAdListener(Interstitial& interstitial) : mInterstitial(interstitial){}
    virtual ~InterstitialAdListener(){

    }
    virtual void onLoaded() override{
        LOGD("InterstitialAdListener::onLoaded");
        mInterstitial.show();
    }
    virtual void onClosed()  override{
         LOGD("InterstitialAdListener::onClosed"); 
    }
    virtual void onOpened() override{
          LOGD("InterstitialAdListener::onOpened");
    }
    virtual void onClicked() override{
         LOGD("InterstitialAdListener::onClicked");
    }
    virtual void onError(const AdError& error)  override{
        LOGD("InterstitialAdListener::onError");  
    }
private:
    Interstitial& mInterstitial;
};
class NativeAdListener : public AdListener
{
public:
    typedef std::function<void()> NativeAdLoadedCallback;
    NativeAdListener(NativeAd& nativeAd, const NativeAdLoadedCallback& callback) : mNativeAd(nativeAd), mNativeAdLoadedCallback(callback){}
    virtual ~NativeAdListener(){

    }
    virtual void onLoaded() override{
        LOGD("NativeAdListener::onLoaded");
        LOGD("NativeAdListener:%s", mNativeAd.getTitle().c_str());
        LOGD("NativeAdListener:%s", mNativeAd.getDescription().c_str());
        LOGD("NativeAdListener:%s", mNativeAd.getIconUrl().c_str());
        const std::vector<Image>& covers = mNativeAd.getCovers();
        LOGD("CoverImageCount[%d]",covers.size());
        for(int i = 0; i < covers.size(); ++i){
            const Image& image = covers[i];
            if(image.isValid()) {
              LOGD("CoverImage[%d][size:%dX%d][url:%s]", i, image.getWidth(),image.getHeight(), image.getUrl().c_str());
            }
        }
        Image image = mNativeAd.filterImageBySize(320, 480);
        if(image.isValid()) {
          LOGD("FilteredImage[size:%dX%d][url:%s]", image.getWidth(),image.getHeight(), image.getUrl().c_str());
        }
        mNativeAdLoadedCallback();
    }
    virtual void onClosed()  override{
         LOGD("NativeAdListener::onClosed"); 
    }
    virtual void onOpened() override{
          LOGD("NativeAdListener::onOpened");
    }
    virtual void onClicked() override{
         LOGD("NativeAdListener::onClicked");
    }
    virtual void onError(const AdError& error)  override{
        LOGD("NativeAdListener::onError");  
    }
private:
    NativeAd& mNativeAd;
    NativeAdLoadedCallback mNativeAdLoadedCallback;
};

std::string now() {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  memset(buffer, 0, 80);
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%Y%m%d%H%M%S",timeinfo);
  return buffer;
}
}//namesapce

namespace{
    static const std::string kIconPath = "/sdcard/union_cocos_demo_icon_bg";
    static const std::string kCoverPath = "/sdcard/union_cocos_demo_cover_bg";
}
using std::placeholders::_1;
using std::placeholders::_2;
void DemoScene::updateIcon(bool success, const std::string& savePath){
    LOGD("DemoScene::updateIcon:%d", success); 
    if(success) { 
        mIcon->loadTexture(savePath);
        FileUtils::getInstance()->removeFile(savePath);
    }
}
void DemoScene::updateCover(bool success, const std::string& savePath){
    LOGD("DemoScene::updateCover:%d", success);  
    if(success) {
        mCover->loadTexture(savePath);
        FileUtils::getInstance()->removeFile(savePath);
    }
}

void DemoScene::nativeAdLoaded(){
    mTitle->setString(mNativeAd.getTitle());
    mDescription->setString(mNativeAd.getDescription());
    if(!mNativeAd.getIconUrl().empty()){
        LOGD("DemoScene::Start fetch icon..."); 
        ImageFetcher::fetch(mNativeAd.getIconUrl().c_str(), kIconPath + now(), std::bind(&DemoScene::updateIcon, this, _1, _2));
    }
    Image image = mNativeAd.filterImageBySize(320, 480);
    const Image * pImg = 0;
    if(image.isValid()) {
        pImg = &image;
    }else{
        const std::vector<Image>& covers = mNativeAd.getCovers();
        if(!covers.empty()){
            pImg = &(covers[0]);
        }
    }
    if(pImg && !pImg->getUrl().empty()){
        LOGD("DemoScene::Start fetch cover..."); 
        ImageFetcher::fetch(pImg->getUrl().c_str(), kCoverPath + now(), std::bind(&DemoScene::updateCover, this, _1, _2));
    }
}
DemoScene::DemoScene() {
    mTopBannerAdListener = new BannerAdListener(mTopBanner);
    mBottomBannerAdListener = new BannerAdListener(mBottomBanner);
    mInterstitialAdListener = new InterstitialAdListener(mInterstitial);
    mInterstitialVideoAdListener = new InterstitialAdListener(mInterstitialVideo);
    mNativeAdListener = new NativeAdListener(mNativeAd, std::bind(&DemoScene::nativeAdLoaded, this));
}
DemoScene::~DemoScene() {
    delete mTopBannerAdListener;
    mTopBannerAdListener = 0;
    delete mBottomBannerAdListener;
    mBottomBannerAdListener = 0;
    delete mInterstitialAdListener;
    mInterstitialAdListener = 0;
    delete mInterstitialVideoAdListener;
    mInterstitialVideoAdListener = 0;
    delete mNativeAdListener;
    mNativeAdListener = 0;
}
void DemoScene::showBanner(Ref* ref){
    {
        AdRequest& request =  AdRequest::newBuilder().pub("ssr@debugbanner").build();
        mTopBanner.setShowPosition(0, 0);
        mTopBanner.setListener(mTopBannerAdListener);
        mTopBanner.load(request);
    }
    {
        AdRequest& request =  AdRequest::newBuilder().pub("ssr@debugbanner").build();
        Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
        mBottomBanner.setShowPosition(0, frameSize.height - Util::dp2Pixel(50));
        mBottomBanner.setListener(mBottomBannerAdListener);
        mBottomBanner.load(request);
    }
}
void DemoScene::showInterstitial(Ref* ref, const std::string& pub, Interstitial* interstitial, AdListener* adListener){
    AdRequest& request =  AdRequest::newBuilder().pub(pub).build();
    interstitial->setListener(adListener);
    interstitial->load(request);
}
void DemoScene::showNativeAd(Ref* ref){
    AdRequest& request =  AdRequest::newBuilder().pub("ssr@debugnative").build();
    mNativeAd.setListener(mNativeAdListener);
    mNativeAd.load(request);
}

void DemoScene::exitApp(Ref* ref) {
    Director::getInstance()->end();
}

// on "init" you need to initialize your instance
bool DemoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    double itemHeight = visibleSize.height / 6;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    origin.x += 20;
    origin.y += 50;
    
    //Title
	mTitle = Label::create();
    mTitle->ignoreAnchorPointForPosition(true); 
    mTitle->setSystemFontSize(10);
    mTitle->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + itemHeight * 5));
    mTitle->setContentSize(Size(visibleSize.width / 2, itemHeight));
    mTitle->setMaxLineWidth(visibleSize.width/2 - 10);
    this->addChild(mTitle, 1);
    //Description
    mDescription = Label::create();
    mDescription->ignoreAnchorPointForPosition(true); 
    mDescription->setSystemFontSize(10);
    mDescription->setPosition(Vec2(origin.x + visibleSize.width/2,
                          origin.y + itemHeight * 4));
    mDescription->setContentSize(Size(visibleSize.width / 2, itemHeight));
    mDescription->setMaxLineWidth(10);
    mDescription->setLineBreakWithoutSpace(true);
    this->addChild(mDescription, 1);
    //Icon
    mIcon = ImageView::create();
    mIcon->ignoreAnchorPointForPosition(true); 
    double iconY = itemHeight *3;
    mIcon->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + iconY));
    mIcon->setContentSize(Size(itemHeight, itemHeight));
    this->addChild(mIcon, 1);
    //Cover
    mCover = ImageView::create();
    mCover->ignoreAnchorPointForPosition(true); 
     double coverY = itemHeight * 2;
     double coverHeight = itemHeight * 2;
    mCover->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y));
    mCover->setContentSize(Size(visibleSize.width / 2, coverHeight));
    this->addChild(mCover, 1);

    Button* btnBanner = Button::create();  
    btnBanner->setTitleText("Banner");    
    btnBanner->setTitleFontSize(15);  
    btnBanner->ignoreAnchorPointForPosition(true); 
    btnBanner->setPosition(Vec2(origin.x, origin.y));  
    btnBanner->addClickEventListener(std::bind(&DemoScene::showBanner, this, this));  
    this->addChild(btnBanner,1);  

    Button* btnInterstitial = Button::create();  
    btnInterstitial->setTitleText("Interstitial");  
    btnInterstitial->setTitleFontSize(15);  
    btnInterstitial->ignoreAnchorPointForPosition(true); 
    btnInterstitial->setPosition(Vec2(origin.x,origin.y + itemHeight));  
    btnInterstitial->addClickEventListener(std::bind(&DemoScene::showInterstitial, this, this, "ssr@debuginterstitial", &mInterstitial, mInterstitialAdListener));  
    this->addChild(btnInterstitial,1);  

    Button* btnInterstitialVideo = Button::create();  
    btnInterstitialVideo->setTitleText("Interstitial Video");  
    btnInterstitialVideo->setTitleFontSize(15);  
    btnInterstitialVideo->ignoreAnchorPointForPosition(true); 
    btnInterstitialVideo->setPosition(Vec2(origin.x,origin.y + itemHeight * 2));  
    btnInterstitialVideo->addClickEventListener(std::bind(&DemoScene::showInterstitial, this, this, "ssr@debugvideo", &mInterstitialVideo, mInterstitialVideoAdListener));  
    this->addChild(btnInterstitialVideo,1);  

    Button* btnNativeAd = Button::create();  
    btnNativeAd->setTitleText("NativeAd");  
    btnNativeAd->setTitleFontSize(15);  
    btnNativeAd->ignoreAnchorPointForPosition(true); 
    btnNativeAd->setPosition(Vec2(origin.x,origin.y + itemHeight * 3));  
    btnNativeAd->addClickEventListener(std::bind(&DemoScene::showNativeAd, this, this));  
    this->addChild(btnNativeAd,1);  

    Button* btnNativeExit = Button::create();  
    btnNativeExit->setTitleText("Exit");  
    btnNativeExit->setTitleFontSize(15);  
    btnNativeExit->ignoreAnchorPointForPosition(true); 
    btnNativeExit->setPosition(Vec2(origin.x,origin.y + itemHeight * 4));  
    btnNativeExit->addClickEventListener(std::bind(&DemoScene::exitApp, this, this));  
    this->addChild(btnNativeExit,1);  

    return true;
}




void DemoScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
}//namespace com_ucweb_union_ads_demo