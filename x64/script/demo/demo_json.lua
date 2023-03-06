author = "Robin"
version = 1.0

setting = {
    name = "json方法演示脚本",
    dir = "d:\\",
	desc = "演示内置的JSON编解码功能",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}

demo = [[本示例演示json编解码功能，
	涉及的函数有：
		intToJson()
		stringToJson()
		numToJson()
		parseJsonStr()
	涉及的类有：
		Json{
			Json()
			isNull()
			isNumber()
			isBool()
			isString()
			isArray()
			isObject()

			toInt()
			toBool()
			toNum()
			toString()
			toArray()
			get()
			dump()
		}

		JsonArray{
			size()
			clear()
			append()
			appendInt()
			appendNum()
			appendStr()
			at()
			toJson()
		}

	需要注意的是网络收发的是utf8字符集，windows默认使用ansi字符集，
	ansiToUtf8
	utf8ToAnsi
	下面演示使用方法：
	]]
	
function lua_main(keyWord, pageIndex, pageSize)
    printMessage()
      

    printMessage("开始构造JSON数据")
    obj = JsonObject()
    obj: set("name", stringToJson("robin"))
    obj: set("age",  intToJson(26))
    obj: set("weight",  numToJson(60.5))

    obj: setInt("grade", 6)
    obj: setNum("mark", 97.5)
    obj: setStr("class", "five")

    vec = JsonArray()
    vec: append(intToJson(1))
    vec: appendInt(2)

    obj:set("list", vec:toJson())

    json = obj: toJson()
    strJson = json: dump()
    printMessage("生成结果为：" .. strJson)


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


    return 0
end

