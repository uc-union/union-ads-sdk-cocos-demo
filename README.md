##union-ads-sdk-cocos-demo

***

union-ads-sdk-cocos-demo is the Cocos2dx version of Union Ads SDK Demo project。    
SDK is integrated in the form of `git submodule`, follow steps below to get demo source code.
 
> All the instruction below is on the premise that your have installed git. If not, please download and install one.

  * clone Demo project   
```
git clone https://github.com/uc-union/union-ads-sdk-cocos-demo.git
```

  * update SDK project     
  
```
git submodule update --init --recursive
git submodule update --remote
```
   
Now you do have the latest demo project and all its dependencies. And then you should alter the `local.properties` file under project root folder, change `cocos2d.home` to your local cocos2d root path,make sure `cocos/platform/android` is under that path.

Just run the demo project and enjoy youself.

More details information about how to integrate  Union Ads SDK , refer [wiki](https://github.com/uc-union/union-ads-sdk-cocos-demo/wiki)
