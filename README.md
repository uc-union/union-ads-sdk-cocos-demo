union-ads-sdk-cocos-demo工程是Union Ads的Unity版本的SDK的示例工程。    
demo工程是以git submodule的方式集成SDK的，开发者可按照以下步骤获取Demo工程源码。    
以下步骤假定你是使用Git作为VCS的，如果不是，请安装并使用Git.    
#####clone 示例工程  
```
git clone https://github.com/uc-union/union-ads-sdk-cocos-demo.git
```
#####更新sdk工程    
```
git submodule update --init --recursive
git submodule update --remote
```
    
通过以上步骤，你就可以获取最新的demo及其依赖的sdk了。接下来，你需要修改demo工程的根目录下的`local.properties`文件，修改其中的
`cocos2d.home`变量为你本地的cocos2d根目录。    
请确保cocos/platform/android在该目录下。

OK,配置完`cocos2d.home`之后你就可以直接运行demo了。
更多接入相关内容请查看wiki开发接入指南[https://github.com/uc-union/union-ads-sdk-cocos-demo/wiki](https://github.com/uc-union/union-ads-sdk-cocos-demo/wiki)

>注意，Demo中是使用多个文字按钮作为功能入口，点击不同的按钮会加载并展示不同的广告。
