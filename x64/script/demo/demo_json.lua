author = "Robin"
version = 1.0

setting = {
    name = "json������ʾ�ű�",
    dir = "d:\\",
	desc = "��ʾ���õ�JSON����빦��",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}

demo = [[��ʾ����ʾjson����빦�ܣ�
	�漰�ĺ����У�
		intToJson()
		stringToJson()
		numToJson()
		parseJsonStr()
	�漰�����У�
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

	��Ҫע����������շ�����utf8�ַ�����windowsĬ��ʹ��ansi�ַ�����
	ansiToUtf8
	utf8ToAnsi
	������ʾʹ�÷�����
	]]
	
function lua_main(keyWord, pageIndex, pageSize)
    printMessage()
      

    printMessage("��ʼ����JSON����")
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
    printMessage("���ɽ��Ϊ��" .. strJson)


    printMessage("\n���Խ�����")
    doc = parseJsonStr(strJson)
 

    if (doc:isNull()) then
        printMessage("doc is null")
    else 
        valName = doc:get("name")
        printMessage(valName:toString())
        valAge = doc:get("age")
        printMessage( valAge:toInt())
        
       
        --������������
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

