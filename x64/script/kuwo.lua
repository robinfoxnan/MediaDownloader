author = "����"
version = 1.0

setting = {
    name = "�����������ؽű�",
    dir = "d:\\",
	desc = "���ݹؼ�����������������ʱ����ģ����ҳ���÷�ҳ��ѡ������ڼ�ҳ",
	input1 = "������",
	input2 = "ҳ��",
	input2 = "����/ҳ",

}


function lua_main(keyWord, pageIndex, pageSize)
    printMessage("������ʹ����ʾ�ű���ʾ�����ʹ�����ù���")
    --printMessage(keyWord)
    --printMessage(pageIndex)
    --printMessage(pageSize)
    printMessage("engine name is ".. engine_name())
    printMessage("engine version is ".. engine_version())
	printMessage("now dir is:" .. setting.dir)

    key = urlEncodeAnsi('˫��')
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
    printMessage("HTTP Ӧ�� " .. code)
    --printMessage("=======<\n")
    local retHeader = client:getHeader()

     printMessage("���ص�ͷ����")
    for k,value in pairs(retHeader:getItems() ) do
        printMessage(k .. ":" .. value)
    end

    printMessage("��ʼ����JSON����")
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
    
    --- ��ȡ����
    jsonList = data_list:toArray()
    count = jsonList:size()
    for i = 0,  count-1 do
        item = jsonList:at(i)

        song_name = item:get('name'):toString()
        singer = item:get('artist'):toString()
        album_name = item:get('album'):toString()
        rid = item:get('rid'):toInt()
        music_info_url = "http://www.kuwo.cn/api/v1/www/music/playUrl?mid=".. rid .. "&type=convert_url3"
        
        --song_name = string.gsub(song_name, "&nbsp;", " ")
        singer = string.gsub(singer, "&nbsp;", " ")
        album_name = string.gsub(album_name, "&nbsp;", " ")
        --��Ҫ�Ľӿ�����������

        music_url = getInfo(music_info_url)
        printMessage(singer .. " | " .. song_name .. " | " .. album_name  .. " | ".. music_url)
        downloadMp3(music_url, singer, song_name)
    end

    return count
end

-- ��ȡ��ϸ��Ϣ
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
    fileName = subDir .. "\\"..  song .. ".mp3"
    printMessage(fileName)
    --client1:doGetToFile(music_url, )
end
