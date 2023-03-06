author = "范例"
version = 1.0

setting = {
    name = "觅知网音乐搜索",
    dir = "d:\\MP3",
	desc = "网站执行搜索后也会返回一个静态网页地址，猜测是使用nginx做加速, 比如https://www.51miz.com/so-sound/86888.html",
	input1 = "网页地址",
	input2 = "未使用",
	input3 = "未使用",

}
  
client = HttpClient()
header = HttpHeader()
header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')

function search(url)
	local urlSearch = url 
	
    local code = client:doGet(urlSearch, header)
    printMessage("HTTP 应答： " .. code)
	
    printMessage("开始解析html数据")
    --printMessage(utf8ToAnsi(client:getBodyAsString()))
    --printMessage(client:getBodyAsAnsiString())
	
	body = client:getBodyAsString()
	
	-- XML_PARSE_NOBLANKS + XML_PARSE_RECOVER
	local opt =  256 + 1
	doc = parseXmlString(body, "", opt)
	if doc:isNull() then
		print("解析html错误")
		return 0
	end
	
	-- 查找所有带有data-id属性的elements
	nodes = doc:xpathSimple("div", "data-id", "");
	n = nodes:size()
	if n == 0 then
		return 0
	end
 
	count = 0
    for i = 0,  n-1 do
        
		local titleStr = ""
		local linkStr = ""
		local lenStr = ""
		item = nodes:at(i)
		
		titles = item:xpathSimple("div", "class", "SoundTitle")
		if (titles:size() > 0) then
			title_a = titles:at(0):getChildByIndex("a", 0)
			if not(title_a:isNull() ) then 
				titleStr = title_a:getValue()
				titleStr = utf8ToAnsi(titleStr)
				print(titleStr)
			end	
		else
			print("not found title")
			goto continue
		end
		
		audio = item:xpathSimple("source", "src", "")
		if  audio:size() > 0 then 
			src = audio:at(0):getAttrByName("src")
			if not src:isNull() then 
				linkStr = "https:" .. src:getValue()
				print(linkStr)
			end
		end 
		
		tms = item:xpathSimple("div", "class", "SoundEndTime fl end-time")
		if tms:size() > 0 then 
			lenStr = tms:at(0):getValue()
			print(lenStr)
		end
		
		
		singer = "音效"
        album_name = ""
        
        --printMessage(singer .. " | " .. titleStr .. " | " .. album_name  .. " | ".. linkStr)
        
        --downloadMp3(music_url, singer, song_name)
        --downloadMp3(music_url, keyWord, song_name)
        
		local tbl = {
			singer = singer,
			song = titleStr,
			album = album_name,
			tags =  titleStr,
			size = lenStr,
			url = linkStr,
		}
		-- 当解析到某个音乐条目的时候，可以使用此函数通知界面
		count = count + 1
		notifyData(1, tbl)
		::continue::
    end --for
	return count
end


function lua_main(url, pageIndex, pageSize)
    printMessage("准备搜索")
    printMessage(keyWord)

    --printMessage("engine name is ".. engine_name())
    --printMessage("engine version is ".. engine_version())
	printMessage("当前目录:" .. setting.dir)
 
	-- https://www.51miz.com/so-sound/86888.html
	local n = 0
	n = n + search(url)
   

    return n
end



function downloadMp3(music_url, singer, song)

    subDir = combinePath(setting.dir, singer)
	
	mkDir(subDir)
    fileName = subDir .. "\\"..  song .. ".mp3"
    printMessage(fileName)
    code = client:doGetToFile(music_url,header, fileName)
    print(code)
end
