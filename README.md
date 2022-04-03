:rocket: Get Started

- **Clone WebSocketServer插件，复制到项目的Plugins目录下**
- 在内容浏览器中找到WebSOcketServer C++类下的WebSocketServerActor，将其拖入场景中
- Find the **WebSocketServerActor** under the **WebSocketServer C++ Class** in the **Content Browser** and drag it into the Unreal Editor
- 点击运行，WebSocketServer就构建好了
- Click Run, WebSocketServer is built， Finished！


### :clap: Featured Demos

- 以Actor构建WebSocket服务器，方便与你的工程衔接
- Build WebSocket server with Actor to facilitate connection with your project
- 支持自定义端口与蓝图调用
- Support custom port and blueprint call
- 提供方便、灵活的API
- Provide convenient and flexible API

### :green_book: License

[Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0.html). WebSocketServer is free for both commercial and non-commercial use.

### :hammer_and_wrench: Developing with WebSocketServer
#### Windows
##### Prerequisities
* Unreal Engine 4.26 or newer.
* Visual Studio 2017 or newer.

##### Build Steps
1. Point your Unreal Engine Project, create Plugins  to the `WebSocketServer.uplugin` file to load the plugin into Unreal Engine.

2. To package the plugin, run the `RunUAT.bat` command :

   ```powershell
   %UE4.27 Path%/Engine/Build/BatchFiles/RunUAT.bat BuildPlugin Plugin=%Base Path%/WebSocketServer/WebSocketServer.uplugin -Package=%Base Path%/WebSocketPlugin/Package/ -VS2017 -TargetPlatforms=Win64 -Rocket

