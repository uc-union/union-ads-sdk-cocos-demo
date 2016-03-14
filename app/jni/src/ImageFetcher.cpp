#include "ImageFetcher.h"

#include <functional>
#include <vector>

#include <network/HttpClient.h>

#include "CCImage.h"
#include "cocos/platform/CCFileUtils.h"

#include "impl/JNICommon.h"


namespace com_ucweb_union_ads_demo {
	using namespace cocos2d::network;
	using namespace cocos2d;
	using std::placeholders::_1;
	using std::placeholders::_2;
	ImageFetcher::ImageFetcher(const std::string& url, const std::string& savePath, const Callback& callback):mRequest(0),
	mUrl(url), mSavePath(savePath), mCallback(callback)
	{
	}


	ImageFetcher::~ImageFetcher()
	{
	}

	void ImageFetcher::fetch(const std::string& url, const std::string& savePath, const Callback& callback){
		ImageFetcher* fetcher = new ImageFetcher(url, savePath, callback);
		fetcher->start();
	}

	void ImageFetcher::start()
	{
		LOGD("ImageFetcher::start fetch: %s!", mUrl.c_str());
		//Can only start once
		if (mRequest)
		{
			return;
		}
		mRequest = new HttpRequest();
		mRequest->setUrl(mUrl.c_str());
		mRequest->setRequestType(HttpRequest::Type::GET);
		mRequest->setResponseCallback(std::bind(&ImageFetcher::onResponse, this, _1, _2));
		std::string tag = "Get Image:";
		tag += mUrl;
		mRequest->setTag(tag.c_str());
		HttpClient::getInstance()->send(mRequest);
	}

	void ImageFetcher::onResponse(HttpClient* client, HttpResponse* response)
	{
		if (response->isSucceed())
		{
			std::vector<char>* responseData = response->getResponseData();
			const char* data = responseData->data();
			size_t size = responseData->size();
			if(responseData && data) {
				if(FileUtils::getInstance()->writeStringToFile(std::string(data, data+size), mSavePath))
				{
					mCallback(true, mSavePath);
					LOGD("ImageFetcher::onResponse success!"); 
					return;
				}
		    }
		}
		LOGD("ImageFetcher::onResponse failed!"); 
		mCallback(false, mSavePath);
	}


}//com_ucweb_union_ads_demo
