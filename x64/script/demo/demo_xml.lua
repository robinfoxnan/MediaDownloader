author = "Robin"
version = 1.0

setting = {
    name = "xml方法演示脚本",
    dir = "d:\\",
	desc = "演示内置的xml解码功能",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}
function printAttrs(node)
	attrArray = node:getAttrsAll()
	local i = 1
	msg = ""
	for i=1, attrArray:size()  do
		attr = attrArray:at(i-1)
		msg = msg .. attr:getName() .. " = " .. attr:getValue() .. ", "
		
	end
	return msg
end

function lua_main(keyWord, pageIndex, pageSize)
    
	xmlStr=[[<?xml version="1.0" encoding="GBK"?>
<root>
  <person name="Alice" age='25'/>
  <person name="Bob" age="30"/>
  <person name="Charlie" age="35"/>
</root>]]

	-- XML_PARSE_NOBLANKS
	opt =  256
	doc = parseXmlString(xmlStr, "GBK", opt)
    if (doc:isNull()) then
        printMessage("解析遇到错误")
		printMessage(doc:getErr())
		return -1
    end
    
	root = doc:getRoot()
	
	nodeArray = root:getChildrenElements()
	local i = 1
	for i=1, nodeArray:size() do
		--item = jsonAt(valArray, i-1)
		item = nodeArray:at(i-1)
		
		printMessage(item:getName() .. ": " .. printAttrs(item))
	
	 end
    


    return 0
end

