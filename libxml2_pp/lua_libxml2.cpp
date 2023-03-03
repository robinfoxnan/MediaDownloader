#include "pch.h"
#include "windows.h"


#ifdef _cplusplus  
extern "C" {
#endif
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
	
int luaopen_libxml2pp(lua_State* L);

#ifdef _cplusplus 
}
#endif

#include <LuaBridge/LuaBridge.h>
#include "RxXmlpp.h"
#include <string>

#pragma comment(lib, "Lua54.lib")

using namespace bird2fish;
using namespace std;
using namespace luabridge;
using namespace luabridge::detail;
;
//原文链接：https ://blog.csdn.net/AllenPocket/article/details/52440256

//static const struct luaL_Reg funcs[] = {
//	{ "isquare",isquare },
//	{ "alert",alert },
//	{NULL,NULL}
//};//这就是luaL_Reg的用法，它给c函数一个供lua调用的名，在lua中你就可以通过该名调用该c函数
//luaL_register(L, "func", funcs);//func就是lua中table的名

int luaopen_libxml2pp(lua_State* L) 
{
	getGlobalNamespace(L).addFunction("parseXmlFile",   Xml2Doc::tryParseFile);
	getGlobalNamespace(L).addFunction("parseXmlString", Xml2Doc::tryParseString);

	luabridge::getGlobalNamespace(L)
		.beginClass<Xml2Attr>("XmlAttr")
		.addConstructor<void(*)(const Xml2Attr&)>()
		.addFunction("isNull", &Xml2Attr::isNull)
		.addFunction("getName", &Xml2Attr::getName)
		.addFunction("getValue", &Xml2Attr::getValue)
		.endClass();


	// Array的At函数比较麻烦，这里映射为全局函数
	auto funcAttrAt = std::function <Xml2Attr(const std::vector<Xml2Attr>*, int)>(
		[](const std::vector<Xml2Attr>* vec, int i) { return (*vec)[i]; }
	);

	// Array的At函数比较麻烦，这里映射为全局函数
	auto funcNodeAt = std::function <Xml2Node(const std::vector<Xml2Node>*, int)>(
		[](const std::vector<Xml2Node>* vec, int i) { return (*vec)[i]; }
	);

	luabridge::getGlobalNamespace(L)
		.beginClass<Xml2AttrArray>("XmlAttrArray")
		.addConstructor<void(*)(void)>()
		.addFunction("size", &Xml2AttrArray::size)
		.addFunction("at", funcAttrAt)
		.endClass();


	luabridge::getGlobalNamespace(L)
		.beginClass<Xml2NodeArray>("XmlNodeArray")
		.addConstructor<void(*)(void)>()
		.addFunction("size", &Xml2NodeArray::size)
		.addFunction("at", funcNodeAt)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass<Xml2Node>("XmlNode")
		.addConstructor<void(*)(const Xml2Node&)>()
		.addFunction("isNull", &Xml2Node::isNull)
		.addFunction("getType", &Xml2Node::getType)
		.addFunction("getName", &Xml2Node::getName)
		.addFunction("getValue", &Xml2Node::getValue)
		
		.addFunction("getAttrsAll", &Xml2Node::getAttrsAll)
		.addFunction("getAttrByName", &Xml2Node::getAttrByName)

		.addFunction("getChildrenElements", &Xml2Node::getChildrenElements)
		.addFunction("getChildrenByType", &Xml2Node::getChildren)
		.addFunction("getChildrenByName", &Xml2Node::getChildrenByName)
		
		.addFunction("xpathSimple", &Xml2Node::getDeepElementsByName)
		.addFunction("xpath", &Xml2Node::xpath)

		.addFunction("getChildByIndex", &Xml2Node::getChildByIndex)
		.addFunction("getParent", &Xml2Node::getParent)
		.addFunction("getPreBrother", &Xml2Node::getPreBrother)
		.addFunction("getNextBrother", &Xml2Node::getNextBrother)

		.endClass();


	luabridge::getGlobalNamespace(L)
		.beginClass<Xml2Doc>("XmlDoc")
		.addConstructor<void(*)(const Xml2Doc&)>()
		.addFunction("isNull", &Xml2Doc::isNull)
		.addFunction("getRoot", &Xml2Doc::getRoot)
		.addFunction("getErr", &Xml2Doc::getErr)
		.addFunction("dump", &Xml2Doc::dump)
		.addFunction("xpath", &Xml2Doc::getByXPath)
		.addFunction("xpathSimple", &Xml2Doc::getByName)
		.endClass();


	return 1;
}

