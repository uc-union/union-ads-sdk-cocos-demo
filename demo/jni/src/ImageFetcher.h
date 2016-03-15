#ifndef _COM_UCWEB_UNION_ADS_DEMO_IMAGE_FETCHER_
#define _COM_UCWEB_UNION_ADS_DEMO_IMAGE_FETCHER_
#include <string>
#include <functional>

#include <base/ccMacros.h>
#include <base/CCRef.h>

namespace cocos2d {
	namespace network{
		class HttpRequest;
		class HttpResponse;
		class HttpClient;
	}
	class Texture2D;
}
namespace com_ucweb_union_ads_demo {
	typedef std::function<void(bool, const std::string&)> Callback;
	class ImageFetcher : public cocos2d::Ref
	{
	public:
		ImageFetcher(const std::string& url, const std::string& savePath, const Callback& callback);
		~ImageFetcher();

		static void fetch(const std::string& url, const std::string& savePath, const Callback& callback);
	private:
 		void start();
		void onResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
		cocos2d::network::HttpRequest* mRequest;
		std::string mUrl;
		std::string mSavePath;
		Callback mCallback;
        ImageFetcher(const ImageFetcher&);
        ImageFetcher& operator=(const ImageFetcher&);
		//DISALLOW_COPY_AND_ASSIGN(ImageFetcher);
	};
}
#endif //_COM_UCWEB_UNION_ADS_DEMO_IMAGE_FETCHER_
