author = "范例"
version = 1.0

setting = {
    name = "酷我音乐搜索",
    dir = "d:\\MP3",
	desc = "根据关键字搜索歌曲，搜索时可以模拟网页设置分页，选择浏览第几页",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",

}
  
  

function lua_main(keyWord, pageIndex, pageSize)
    printMessage("您正在使用演示脚本，示范如何使用内置功能")
    --printMessage(keyWord)
    --printMessage(pageIndex)
    --printMessage(pageSize)
    printMessage("engine name is ".. engine_name())
    printMessage("engine version is ".. engine_version())
	printMessage("now dir is:" .. setting.dir)
 
    key = urlEncodeAnsi('双笙')
    key = urlEncodeAnsi(keyWord)
    local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
    header:setItem("Cookie", "kw_token=CBQHDVFFOMD;kw_token=7URL5LI6H9X;")
    --header:setItem("ContentType", "application/json")
    header:setItem('csrf','CBQHDVFFOMD')
    
    header:setItem("Accept-Encoding", "gzip, deflate" )
    
    header:setItem("Postman-Token", "60c800d3-6c22-4fa7-a8f2-45bf5c83161e")
    header:setItem('Host','www.kuwo.cn')
    header:setItem('Pragma','no-cache')
    header:setItem('Referer', "http://www.kuwo.cn/")
    

    client = HttpClient()
    
    local url ="http://www.kuwo.cn/api/www/search/searchMusicBykeyWord?key=" .. key
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
    local doc = client:getBodyAsJson()
    doc_data = doc:get("data")
    if doc_data:isNull() then 
        return 0
    end

    data_list = doc_data:get("list")
    if data_list:isNull() then 
        return 0
    end
    
    --- 提取数据
    jsonList = data_list:toArray()
    count = jsonList:size()
    for i = 0,  count-1 do
        item = jsonList:at(i)

        song_name = item:get('name'):toString()
        singer = item:get('artist'):toString()
        album_name = item:get('album'):toString()
        rid = item:get('rid'):toInt()
        music_info_url = "http://www.kuwo.cn/api/v1/www/music/playUrl?mid=".. rid .. "&type=convert_url3"
		album_pic = item:get("pic"):toString()
        song_len = item:get("songTimeMinutes"):toString()
		
		
        --song_name = string.gsub(song_name, "&nbsp;", " ")
        singer = string.gsub(singer, "&nbsp;", " ")
        album_name = string.gsub(album_name, "&nbsp;", " ")
        --重要的接口其他都类似

        music_url = getInfo(music_info_url)
        printMessage(singer .. " | " .. song_name .. " | " .. album_name  .. " | ".. music_url)
        
        --downloadMp3(music_url, singer, song_name)
        downloadMp3(music_url, keyWord, song_name)
        
		local tbl = {
			singer = singer,
			song = song_name,
			album = album_name,
			tags = rid,
			size = song_len,
			url = music_url,
		}
		-- 当解析到某个音乐条目的时候，可以使用此函数通知界面
		notifyData(1, tbl)
    end

    return count
end

-- 获取详细信息
function getInfo(music_info_url)

    local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
   -- header:setItem("Cookie", "kw_token=CBQHDVFFOMD;kw_token=7URL5LI6H9X;")
    
   -- header:setItem('csrf','CBQHDVFFOMD')
    --header:setItem("Accept-Encoding", "gzip, deflate" )
   -- header:setItem("Postman-Token", "60c800d3-6c22-4fa7-a8f2-45bf5c83161e")
    --header:setItem('Host','www.kuwo.cn')
   -- header:setItem('Pragma','no-cache')
    --header:setItem('Referer', "http://www.kuwo.cn/")

    client1 = HttpClient()
    local code = client1:doGet(music_info_url, header)
    if code ~= 200 then 
        printMessage("get info error".. code)
        return ""
    end

    --printMessage(client1:getBodyAsAnsiString())

    local music_json = client1:getBodyAsJson()
    local music_url = music_json:get('data'):get('url'):toString()
    return music_url
end

function downloadMp3(music_url, singer, song)

    local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
    client1 = HttpClient()

    subDir = combinePath(setting.dir, singer)
	
	mkDir(subDir)
    fileName = subDir .. "\\"..  song .. ".mp3"
    printMessage(fileName)
    code = client1:doGetToFile(music_url,header, fileName)
    print(code)
end
