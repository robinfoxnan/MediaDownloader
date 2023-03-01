



## lua引擎接口说明文档

| 版本 | 0.1                                                          |
| ---- | ------------------------------------------------------------ |
| 日期 | 2023-03-01                                                   |
| 约定 | 默认情况下，脚本使用ansi编码，比如中国使用gbk或者gb2312编码，代码页936 |
| 状态 | 提供基础的HTTP访问基本功能，已经完成音乐搜索下载基本测试；   |
| 计划 | 增加视频编解码与下载功能，增加API性能测试                    |



### 一、基本信息

| 函数名                    | 参数类型 | 返回值类型 | 含义                                               |
| ------------------------- | -------- | ---------- | -------------------------------------------------- |
| engine_name()             | 无       | 无         | 获取当前引擎名字                                   |
| engine_version()          | 无       | 无         | 获取当前引擎版本，用于判断函数支持程度             |
| printMessage(msg)         | 字符串   | 无         | 输出各种信息                                       |
| notifyData(dataType, tbl) | 整数，表 | 无         | 通知界面获取到了某类信息，目前1表示音乐，2表示视频 |

音乐的table设置示例如下：

```lua
local tbl = {
		singer = "程响",
		song = "云底人间",
		album = "某专辑",
		tags = "云底",
		size = "5.4M",
		url = "HTTS://AFSDAFAAF",
	}
	-- 当解析到某个音乐条目的时候，可以使用此函数通知界面
	notifyData(1, tbl)

```

媒体的table的设置示例如下：

```lua
local tbl = {
		uploader = "李子柒",
		title = "云底人间",
		album = "某专辑",
		tags = "",
		size = "5.4M",
		url = "HTTS://AFSDAFAAF",
	}
	-- 当解析到某个音乐条目的时候，可以使用此函数通知界面
	notifyData(2, tbl)
```



### 二、字符串编解码

字符集与编解码，提供了4个基础函数用于处理ansi字符集与utf8之间的转换，以及url的转换，

因为：默认情况下，脚本使用ansi编码，而网络收发数据基本默认就是UTF-8，windows默认按照本地字符集编解码，当前程序使用多字节ansi编码；


| 函数名        | 参数类型 | 返回值类型 | 含义                                                         |
| ------------- | -------- | ---------- | ------------------------------------------------------------ |
| ansiToUtf8    | 字符串   | 字符串     | 从本地编码转为utf-8                                          |
| utf8ToAnsi    | 字符串   | 字符串     | 从utf-8转为本地编码                                          |
| urlEncodeAnsi | 字符串   | 字符串     | get和post发送的参数需要转义时，将ansi类型字符串转义          |
| urlDecodeAnsi | 字符串   | 字符串     | 对转义的url解码，直接转为ansi字符串，（这是为了界面实现方便） |

示例如下：

```lua
-- 脚本使用ansi字符集编码
key = urlEncodeAnsi('双笙')
local url ="http://www.ku.cn/api/www/search/searchMusicBykeyWord?key=" .. key
local code = client:doGet(url, header)
```



### 三、HTTP操作功能

描述HTTP1.1 的header部分使用类：

#### 3.1 HttpHeader与HttpClient

| 函数名     | 参数类型       | 返回值类型 | 含义                   |
| ---------- | -------------- | ---------- | ---------------------- |
| HttpHeader | 无             | HttpHeader | 构造函数               |
| setItem    | string, string | 无         | 设置单个条目           |
| size       | 无             | int        | 获取条目个数           |
| getItem    | string         | string     | 根据键值索取条目字符串 |
| getItems   | 无             | lua table  | 获取所有的条目表       |

示例代码如下：


```lua
local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
    header:setItem("Cookie", "kw_token=CBQHDVFFOMD;kw_token=7URL5LI6H9X;")
    --header:setItem("ContentType", "application/json")
    header:setItem('csrf','CBQHDVFFOMD')
    
    header:setItem("Accept-Encoding", "gzip, deflate" )
    
    header:setItem("Postman-Token", "60c800d3-6c22-4fa7-a8f2-45bf5c83161e")
    header:setItem('Host','www.ku.cn')
    header:setItem('Pragma','no-cache')
    header:setItem('Referer', "http://www.ku.cn/")
```

HTTP、HTTPS访问服务器支持GET和POST方法：
| 函数名        | 参数类型 | 返回值类型 | 含义                                                         |
| ------------- | -------- | ---------- | ------------------------------------------------------------ |
| HttpClient | 无             | HttpClient | 构造函数               |
| doGet() | int | http 应答码或者负数错误值 | 执行Get操作 |
| doGetToFile() | int | http 应答码或者负数错误值 | 直接下载文件到本地 |
| getBodyAsString() | string | 返回原始服务器器的应答字符串 | 执行Get操作，并返回字符串 |
| getBodyAsAnsiString() | string | 返回ansi格式的字符串（执行了uft8转ansi） | 执行GET或者POST后，获取body为string，（隐含转码） |
| getBodyAsJson() | int | http 应答码或者负数错误值 | 执行GET或者POST后，获取body为json对象，（未转码） |
| getBodyAsHtml() | int | http 应答码或者负数错误值 | ?? |
| getHeader() | 无 | HttpHeader对象 | 执行GET或者POST后，获取头部信息，方便解析Cookie等操作 |
| doPost() | int | http 应答码或者负数错误值 | ？ |
| doPostAction() | int | http 应答码或者负数错误值 | ？ |
| doPostFileAction() | int | http 应答码或者负数错误值 | ？ |

访问HTTP服务器使用HttpClient示例如下：

```lua
 key = urlEncodeAnsi('双笙')
    local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
    header:setItem("Cookie", "kw_token=CBQHDVFFOMD;kw_token=7URL5LI6H9X;")
    --header:setItem("ContentType", "application/json")
    header:setItem('csrf','CBQHDVFFOMD')
    
    header:setItem("Accept-Encoding", "gzip, deflate" )
    
    header:setItem("Postman-Token", "60c800d3-6c22-4fa7-a8f2-45bf5c83161e")
    header:setItem('Host','www.ku.cn')
    header:setItem('Pragma','no-cache')
    header:setItem('Referer', "http://www.ku.cn/")
    

    client = HttpClient()
    
    local url ="http://www.ku.cn/api/www/search/searchMusicBykeyWord?key=" .. key
    local code = client:doGet(url, header)
    printMessage("HTTP 应答： " .. code)
    --printMessage("=======<\n")
    local retHeader = client:getHeader()

     printMessage("返回的头部：")
    for k,value in pairs(retHeader:getItems() ) do
        printMessage(k .. ":" .. value)
    end

    printMessage("开始解析JSON数据")
    --printMessage(client:getBodyAsAnsiString())

```



#### 3.2 JSON编解码功能

核心的类主要是Json用于描述文档或者其中的某一个节点，

提供了如下全局函数用于解析转为：

| 函数          | 参数类型 | 返回类型 | 含义                       |
| ------------- | -------- | -------- | -------------------------- |
| intToJson     | 整数     | Json对象 |                            |
| stringToJson  | 字符串   | Json对象 |                            |
| numToJson     | 浮点数   | Json对象 |                            |
| parseJsonStr  | 字符串   | Json对象 | 解析字符串                 |
| parseJsonFile | 字符串   | Json对象 | 从本地文件加载，并解析内容 |

Json对象本身表示一个节点，可能是对象 JsonObject，一个数组JsonArray，或者单个值，

Json对象的成员函数如下：

| 函数       | 参数类型 | 返回类型 | 含义                     |
| ---------- | -------- | -------- | ------------------------ |
| Json       | 字符串   | Json对象 | 构造函数                 |
| isNull   | 无       | bool     | 确定是否为空             |
| isNumber | 无       | bool     | 确定是否为整数或者浮点数 |
| isBool   | 无       | bool     | 确定是否为BOOL           |
| isString | 无       | bool     | 确定是否为string         |
| isArray   | 无       | bool     | 确定是否为BOOL           |
| isObject | 无       | bool     | 确定是否为string         |
| toInt | 无       | int  | 转换为int   |
| toBool | 无       | bool     | 转换为bool |
| toNum | 无       | double | 转换为double |
| toString | 无       | string | 转换为string       |
| toArray | 无       | JsonArray | 转换类型对数组  |
| get | 无       | Json | 根据键值，直接提取元素 |
| dump | 无       | string |  用于序列化为文件，使用字符集ansi，如果需要网络提交，则需要自行转码  |

JsonArray表示一个Json数组队形，成员函数如下：
| 函数      | 参数类型 | 返回类型      | 含义     |
| --------- | -------- | ------------- | -------- |
| JsonArray | 无       | JsonArray对象 | 构造函数 |
| size | 无       | int | 获取大小 |
| clear | 无       | 无 | 清空 |
| append | Json   | 无 | 追加Json类型数据到队列 |
| appendInt | int    | 无 | 追加int类型数据到队列 |
| appendNum | double | 无 | 追加double类型数据到队列 |
| appendStr | string | 无 | 追加string类型数据到队列 |
| at | int  | Json对象 | 从队列中提取某位置的元素 |
| toJson | 无       | Json对象 | 整体转换为Json |



单个Json对象在构造的时候，使用JsonObject表示，内置函数toJson用于转换为Json对象类型；

成员函数如下：

| 函数      | 参数类型 | 返回类型      | 含义     |
| --------- | -------- | ------------- | -------- |
| JsonObject     | 无 | JsonObject | 构造函数 |
|  size    | 无 | int | 获取元素个数 |
|  clear    | 无 | 无 | 清空 |
| at | string | Json | 根据键值提取子元素 |
| set | string,  Json | 无 | 设置Json类型子元素 |
| setInt | string, int | 无 | 设置int类型子元素 |
| setNum | string, double | 无 | 设置double类型子元素 |
| setStr | string, string | 无 | 设置string类型子元素 |
| remove | string | 无 | 删除一个子元素 |
| toJson | 无 | Json | 转换为Json类型 |
|      |      |      |      |
|      |      |      |      |
|      |      |      |      |

完整的创建JSON的示例如下：

```lua
	printMessage("开始构造JSON数据")
    obj = JsonObject()
    obj:set("name", stringToJson("robin"))
    obj:set("age",  intToJson(26))
    obj:set("weight",  numToJson(60.5))

    obj:setInt("grade", 6)
    obj:setNum("mark", 97.5)
    obj:setStr("class", "five")


    vec = JsonArray()
    vec:append(intToJson(1))
    vec:appendInt(2)

    obj:set("list", vec:toJson())

    json = obj:toJson()
    strJson = json:dump()
    printMessage("生成结果为：" .. strJson)
```

输出为：(格式化后)

```json
{
 "age": 26, 
 "class": "five", 
 "grade": 6, 
 "list": [1, 2], 
 "mark": 97.500000, 
 "name": "robin", 
 "weight": 60.500000
}

```



对此示例的解析如下：

```lua
	printMessage("\n测试解析：")
    doc = parseJsonStr(strJson)
 

    if (doc:isNull()) then
        printMessage("doc is null")
    else 
        valName = doc:get("name")
        printMessage(valName:toString())
        valAge = doc:get("age")
        printMessage( valAge:toInt())
        
       
        --遍历整个数组
        valList = doc:get("list")
        valArray = valList:toArray()
        printMessage("list len=" .. valArray:size())

        for i=1, valArray:size() do
            --item = jsonAt(valArray, i-1)
            item = valArray:at(i-1)
            if item:isNumber() then 
                printMessage("array item" ..i .. " = " .. item:toInt())
            end
         end
    end
```




#### 3.3 HTML与XML编解码功能

未实现



#### 3.4 文件操作功能

限制：所有文件操作，为了保证安全，都要求限制在setting["dir"]路径下：

| 函数             | 参数类型       | 返回类型 | 说明                 |
| ---------------- | -------------- | -------- | -------------------- |
| combinePath      | string, string | string   | 拼接文件到当前路径， |
| mkDir            | string         | bool     | 创建目录             |
| readFileAsString | string         | string   | 读文件为字符串       |
| writeToFile      | string, string | bool     | 写字符串到文件       |
| fileExist        | string         | bool     | 文件或者目录是否存在 |


示例代码：

```lua
	printMessage("当前目录：" .. setting.dir)

	singer = "周懂"
    subDir = combinePath(setting.dir,  singer)
	
	b = mkDir(subDir)
	if b then 
		print("创建成功" .. subDir)
	else 
		print("创建失败" .. subDir)
	end
	
	b = fileExist(subDir)
	if b then 
		print("存在" .. subDir)
	else 
		print("不存在" .. subDir)
	end
	
	print(b)
	song = "test"
    fileName = subDir .. "\\"..  song .. ".txt"
    printMessage("准备写文件" .. fileName)
	
    writeToFile("it is a test.", fileName)
    
    str = readFileAsString(fileName)
    print(str)
```



### 四、数据库操作

#### 4.1 sqlite

#### 4.2 redis

#### 4.2 mysql

#### 4.3 Elastic Search



### 五、约定

#### 5.1 脚本编码

脚本使用ansi编码，比如中国使用gbk或者gb2312编码，代码页936



#### 5.2 交互信息

所有的脚本必须提供如下的基础信息：

```lua
author = "Robin"
version = 1.0

setting = {
    name = "字符集使用",
    dir = "d:\\",
	desc = "展示字符集转码",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}

```

解释如下：

| 全局变量       | 含义                                                         |
| -------------- | ------------------------------------------------------------ |
| author         | 脚本作者                                                     |
| version        | 脚本的版本号                                                 |
| setting.name   | 脚本名称                                                     |
| setting.dir    | 约定的默认工作目录，限制文件操作在此范围内；引擎会重置此项！！ |
| setting.desc   | 描述脚本的使用方式等说明                                     |
| setting.input1 | 第一个输入的名称                                             |
| setting.input2 | 第二个输入的名称                                             |
| setting.input3 | 第三个输入的名称                                             |

#### 5.3 调用入口

调用的入口函数名必须为lua_main，并且有三个字符串参数，返回一个整数类型；

```lua
function lua_main(keyWord, pageIndex, pageSize)

return 0
end

```

#### 5.4 流程

用户在界面填写参数，通过脚本来执行数据解析，填写数据返回给界面展示，

下载资源的过程可以在脚本中实现（需要设置复杂的header的时候），也可以在界面操作。

返回的数据格式如下：

```lua
local tbl = {
		singer = "程响",
		song = "云底人间",
		album = "某专辑",
		tags = "云底",
		size = "5.4M",
		url = "HTTS://AFSDAFAAF",
	}
	-- 当解析到某个音乐条目的时候，可以使用此函数通知界面
	notifyData(1, tbl)
```

返回的数据使用table方式，但是键值的固定的！

