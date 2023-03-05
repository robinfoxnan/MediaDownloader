author = "范例"
version = 1.0

setting = {
    name = "铜钟音乐搜索",
    dir = "d:\\MP3",
	desc = "根据关键字搜索歌曲，搜索时同时搜索几个主流网站",
	input1 = "搜索词",
	input2 = "未使用",
	input3 = "未使用",

}
  
client = HttpClient()
header = HttpHeader()
header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')

function search(url, key)
	local urlSearch = url .. key
    local code = client:doGet(urlSearch, header)
    printMessage("HTTP 应答： " .. code)
	
    printMessage("开始解析JSON数据")
    printMessage(utf8ToAnsi(client:getBodyAsString()))
    --printMessage(client:getBodyAsAnsiString())
    local doc = client:getBodyAsJson()
    data = doc:get('data')
    if  data:isNull() then
		return 0
	end
	
    songs = data:get("songs")
    if not(songs:isArray()) then 
        return 0
    end

    n = 0
    if songs:isNull() then 
        return 0
    end
	
	
    --- 提取数据
    jsonList = songs:toArray()
    
    count = jsonList:size()
    printMessage(count)
    
    for i = 0,  count-1 do
        item = jsonList:at(i)
		
		rid = item:get('originalId'):toInt()
		if rid == 0 then
			rid = item:get('originalId'):toString()
		end		
		
        song_name = item:get('name'):toString()
        
		platform = item:get('platform'):toString()
		
		artistList = item:get( "artists"):toArray()
		singer = artistList:at(0):get('name'):toString()
 
        album_name = item:get("album"):get("name"):toString()
        
        --https://music-api.tonzhon.com/song_source/netease/561999164
        music_info_url = "https://music-api.tonzhon.com/song_source/".. platform .. "/" .. rid
		album_pic = ""
        song_len = ""
		
		
        --song_name = string.gsub(song_name, "&nbsp;", " ")
        --singer = string.gsub(singer, "&nbsp;", " ")
        --album_name = string.gsub(album_name, "&nbsp;", " ")
        --重要的接口其他都类似

        music_url = getInfo(music_info_url)
        printMessage(singer .. " | " .. song_name .. " | " .. album_name  .. " | ".. music_url)
        
        --downloadMp3(music_url, singer, song_name)
        --downloadMp3(music_url, keyWord, song_name)
        
		local tbl = {
			singer = singer,
			song = song_name,
			album = album_name,
			tags = platform .. "[" ..rid .. "]",
			size = song_len,
			url = music_url,
		}
		-- 当解析到某个音乐条目的时候，可以使用此函数通知界面
		notifyData(1, tbl)
    end --for
	return count
end


function lua_main(keyWord, pageIndex, pageSize)
    printMessage("准备搜索")
    printMessage(keyWord)

    --printMessage("engine name is ".. engine_name())
    --printMessage("engine version is ".. engine_version())
	printMessage("当前目录:" .. setting.dir)
 
    --key = urlEncodeAnsi('双笙')
    local key = urlEncodeAnsi(keyWord)

	local n = 0
	n = n + search("https://tonzhon.com/api/exact_search?keyword=", key)
	n = n + search("https://tonzhon.com/api/fuzzy_search?keyword", key)
	n = n + search("https://music-api.tonzhon.com/search?platform=qq&keyword=", key)
	n = n + search("https://music-api.tonzhon.com/search?platform=kuwo&keyword=", key)
	n = n + search("https://music-api.tonzhon.com/search?platform=netease&keyword=", key)
   

    return n
end

-- 获取详细信息
function getInfo(music_info_url)
	
	print(music_info_url)
    local code = client:doGet(music_info_url, header)
    if code ~= 200 then 
        printMessage("获取音乐的资源地址请求错误：".. code)
        return ""
    end

    --printMessage(client1:getBodyAsAnsiString())
	print(client:getBodyAsString())
    local music_json = client:getBodyAsJson()
    sucsess = music_json:get("success"):toBool()
    if  not sucsess then 
		return "获取出错，可能需要收费"
    end
    
    local music_url = music_json:get('data'):get('songSource'):toString()
    return music_url
end

function downloadMp3(music_url, singer, song)



    subDir = combinePath(setting.dir, singer)
	
	mkDir(subDir)
    fileName = subDir .. "\\"..  song .. ".mp3"
    printMessage(fileName)
    code = client:doGetToFile(music_url,header, fileName)
    print(code)
end
