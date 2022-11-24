# HTTP转WebSocket网关

## 项目结构

本项目是一个简化的Http到WebSocket网关的转换网关程序，主要用于解决使用LabVIEW编程实现WebSocket协议较为麻烦的问题，也可用于其他http客户端。
- src目录为本项目源码，基于Qt6.4开发，采用Qt要求的LGPL授权。
- release目录为编译的windows系统下运行的可执行程序
- testvi包含一个基于LabVIEW的测试vi和一个可以通过VS Code的REST Client打开的http客户程序，也可以通过文本编辑器打开来查看http请求格式

## 项目功能

- 支持用户通过Http的get请求，获取HTTP服务器暂存的消息，暂存消息包括服务器在线/离线的消息，以及来自websocket服务器的消息。
- 支持用户通过http的post请求，发送消息到http服务器，用户post消息需要采用JSON格式，JSON消息中 load 字段的 字符串值会被发送到WebSocket服务器。
- 支持WebSocket服务器断线重连功能，WebSocket服务器离线后每10秒钟尝试重新连接。
- 支持配置websocket服务器地址，设置http服务器端口（主要考虑到http服务器端口可能被占用，使用的websocket服务器地址需要调整），用户可修改config.json文件。
- 用户get命令地址为 http://<用户主机ip>：<端口>/v1/query/ ,**需要包含最后的斜线**，query后是否跟数字id均可，该功能未做区分，仅用于从http服务器获取一条缓存的消息
- 用户post命令地址为 http://<用户主机ip>：<端口>/v1/sendws/ ，命令格式为JSON，需要包含{"load":"WebSocket消息内容"},WebSocket消息内容部分会被发送到WebSocket服务器，同时http服务器将用户发送的消息返回给用户。
- 用户应该定时查询http服务器缓存的消息，get请求返回的消息为字符串格式，"NA"表示无缓存消息，WebSocket Server Online和WebSocket Server offline表示服务器上线和下线。

## 使用方法

- 为了方便服务器测试，本项目从Qt的官方案例编译了echo_WebSocket_server服务器，该websocket服务器将用户发送到服务器的消息回声返回给客户端，可用于测试服务器功能，该服务器默认端口为3390
- TestSimpleHttpClient.vi使用NI公司LabVIEW 2018编写，可用于测试发送http消息并观察反馈，`client.http`通过VS Code打开并发送命令。
- GateWay.zip解压缩并运行其中exe文件，可以启动http转Websocket网关
- 启动方式，websocket服务器和网关都可以直接运行exe文件，使用默认值运行，如果要进行调试，可以使用命令行运行并添加 `-d` 指令用于打开调试信息。 websocket服务器还可以通过 `-p 1234`格式的命令修改服务器端口为1234. 例如，在解压缩后的文件夹中打开windows终端，运行 `./gateway.exe -d `.
- 可以通过config.json文件修改网关中的websocket服务器地址和http服务器端口。
- 用户可以通过在线网站 [http://www.websocket-test.com/](http://www.websocket-test.com/)，使用这个websocket客户端来单独测试websocket服务器。

