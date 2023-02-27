author = "Robin-Fox"
version = 1.0

setting = {
    name = "测试math",
    dir = "d:\\",
	desc = "测试内置函数库math",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}


function lua_main(keyWord, pageIndex, pageSize)
	--printMessage 仅能接收字符串

	print(math.abs(-10))  -- 输出 10
	print(math.abs(3.14)) -- 输出 3.14
	--math.ceil(x)
	--返回不小于 x 的最小整数。



	print(math.ceil(3.14)) -- 输出 4
	print(math.ceil(-3.14)) -- 输出 -3
	--math.floor(x)
	--返回不大于 x 的最大整数。



	print(math.floor(3.14)) -- 输出 3
	print(math.floor(-3.14)) -- 输出 -4
	--math.sqrt(x)
	--返回 x 的平方根。
	

	print(math.sqrt(4))  -- 输出 2
	print(math.sqrt(9))  -- 输出 3
	print(math.sqrt(16)) -- 输出 4

	--math.random([m[, n]])
	--返回一个 [m, n] 之间的随机整数。如果不传入参数，则返回一个 [0,1) 之间的随机小数。
	
	math.randomseed(os.time()) -- 初始化随机数种子，确保每次运行结果不同
	print(math.random())      -- 输出一个 [0,1) 之间的随机小数
	print(math.random(10))    -- 输出一个 [1,10] 之间的随机整数
	print(math.random(5, 10)) -- 输出一个 [5,10] 之间的随机整数
	
	--math.max(x1, x2, ...)
	--返回参数中的最大值。


	
	print(math.max(1, 2, 3))    -- 输出 3
	print(math.max(-1, -2, -3)) -- 输出 -1
	
	--math.min(x1, x2, ...)
	--返回参数中的最小值。
	
	
	print(math.min(1, 2, 3))    -- 输出 1
	print(math.min(-1, -2, -3)) -- 输出 -3
	
	--注意-- ：在使用 math.random 函数前，应该使用 math.randomseed 函数初始化随机数种子，以确保每次运行结果不同。

	
	print(math.ceil(3.14)) -- 输出 4
	print(math.ceil(-3.14)) -- 输出 -3
	
	--math.floor(x)
	--返回不大于 x 的最大整数。

	print(math.floor(3.14)) -- 输出 3
	print(math.floor(-3.14)) -- 输出 -4
	--math.sqrt(x)-- ：返回 x 的平方根。


	
	print(math.sqrt(4))  -- 输出 2
	print(math.sqrt(9))  -- 输出 3
	print(math.sqrt(16)) -- 输出 4
	
	--math.random([m[, n]])
	--返回一个 [m, n] 之间的随机整数。如果不传入参数，则返回一个 [0,1) 之间的随机小数。

	math.randomseed(os.time()) -- 初始化随机数种子，确保每次运行结果不同
	print(math.random())      -- 输出一个 [0,1) 之间的随机小数
	print(math.random(10))    -- 输出一个 [1,10] 之间的随机整数
	print(math.random(5, 10)) -- 输出一个 [5,10] 之间的随机整数


    return 0
 end