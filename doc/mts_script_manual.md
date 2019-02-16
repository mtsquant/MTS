# MTS 量化脚本API手册 - MTS Python SDK 


> `2016.09 ~ 2019.2


[TOC]
***

## 1. MTS 简介及概念

### 1.1 简介 
MTS SDK标准化了量化交易的数据访问和交易接口，并以易用、开放、高性能为设计目标针对交易进行优化。MTS 量化脚本语言基于流行通用的Python语言，以标准python模块的形式提供给用户使用，是对其量化业务逻辑的扩展。
所以用户的策略脚本可以使用完整的python的生态系统(建议使用Anaconda)，包括数量众多，功能完备的第三方库，图形化编程环境IDE等都可以供用户选择。
MTS 根据用户协议不同，分为：
* 开源版本 - 基本的实盘交易模块。包括实盘报价订阅，下单，报单管理和仓位管理。
* 免费版本 - 在开源版本的基础上增加回测模块，可以进行历史回测。
* 专业版本 - 包含开源版本和免费版本的全部功能，添加beta模式支持，增加智能报单，均线指标的支持。支持参数优化以及分布式回测。

### 1.2 MTS 版本
MTS 分为三个版本，分别对应不同的功能模块的可用。
* 开源版
GitHub/Gitee上的开源版本，任何人都可以clone下来自己编译。虽然只包含基本模块，但已经是完整的量化交易系统。此版本只支持实盘交易，不能回测。
* 免费版
此版本可以免费下载使用，覆盖开源版的所有功能，同时支持回测--回测引擎是简单的乐观模型。
* 专业版
最全面的功能支持，包含所有功能模块，更好的回测引擎，GUI界面以及技术支持等。

### 1.3 MTS 系统组成 
MTS 由三个主要组件构成：
* 行情报价组件（feeds）
* 报单及仓位管理组件（opms）
* 下单交易组件（trade）
其中feeds 属于输入系统，trade 属于输出系统。这两个组件都有两种不同的工作模式（分别是 simu 和 real），但不同的工作模式向opms提供一致的接口，所以opms 不用区分具体的模式。
1. feeds
	1. `feeds simu` 报价行情回放，读取行情数据进行回放。 (**专业版提供**)
	2. `feeds real` 接收实时真实行情。
1. trade
	1. `trade simu` 模拟交易所，负责模拟撮合成交  (**专业版提供**)
	2. `trade real` 接驳到真实券商，最终发到真实交易所交易


### 1.4 MTS 三种运行模式 
MTS 根据上述组件的不同模式的组合，提供了三种不同的运行模式，以便满足回测，实盘模拟和实盘交易等不同的需求。

#### 1.4.1 回测模式 - simu  (**免费版，专业版提供**)
`feeds simu + trade simu`
此模式读取报价记录文件进行回放，并且用户下单到MTS自身的模拟交易所，进行高速回测。

#### 1.4.2 实盘测试模式 - beta (**专业版提供**)
`feeds real + trade simu`
此模式接收实盘报价，但用户下单到MTS自身的模拟交易所，以真实速度进行实时测试。

#### 1.4.3 实盘交易模式 - real (**开源版提供**)
`feeds real + trade real`
此模式接收实盘报价，而且用户下单到真实券商，进行实盘交易。当然，在此模式下，用户可以接驳到模拟盘（比如 simunow）进行模拟交易（更真实的模拟），对于MTS而言，其不区分用户接驳的是模拟盘环境还是实盘环境。

>模式表：

>| mode| feeds| trade|
>|-----|------|------|
>| 回测模式 - simu (**免费版，专业版提供**) | simu | simu |
>| 实盘测试模式 - beta (**专业版提供**)|  real | simu |
>| 实盘交易模式 - real|  real | real |



### 1.5 MTS 相关概念 

* 回调函数：由MTS的各种事件（报价的变化，下单的状态变化，成交回报推送，定时器等）触发并由MTS主动回调的用户定义的函数。
这些函数由MTS回调，用户不需要也不应该直接调用他们。一般而言以`on`打头。
* symbol： 金融衍生品的交易代码。对于期货就是交易所的代码。
`比如：ag1712（上期所），i1709（大商所），RM801（郑商所），IF1708（中金所）`
* MtsSymbol： MTS 统一格式化后的交易代码，屏蔽不同交易所的取名规则的差异。
商品期货统一用大写字母配合4位数字的月份的方式。
`比如：AG1712（上期所），I1709（大商所），RM1801（郑商所），IF1708（中金所）`
* 交易日： 表示交易日期。
对于商品期货而言，日盘的交易日期和真实日期一致。夜盘的0点之前的交易时段的交易日期和真实日期不一致。
* 连续主力合约： 期货日期月份为`888`的symbol表示连续主力合约。
MTS会映射其到当前的主力合约，并会在主力合约变换的时候自动切换。MTS 也支持用户自己定义主力合约序列。连续主力合约并非物理存在的合约，因而其只能用于回测模式，不能用于实盘，也不能下单。

## 2. MTS 通知回调系统

MTS 采用通知回调的方式推送给用户其关心的报价的变化，下单的状态变化，成交回报等，用户在此函数中实现自己的算法和策略。
通知回调函数放在类StrategyBase中，供用户的交易策略类继承override使用。
其函数名称一般以`on`开头，参数表都采用如下统一的格式，以便记忆：
```python
class Strategy(mts.StrategyBase):
	def onCallback (self,notifyData):
    	...
```

#### 2.1 系统通知回调 
##### 2.1.1 初始化完成通知  `onInitialized()`  
此通知会在MTS初始化完成之后，MTS 开始运行之前进行回调。用户一般可以进行策略的初始化工作。比如，订阅报价，创建一些标准指标等。
```python
class Strategy(mts.StrategyBase):
	def onInitialized (self,account):
    	...
```
* 参数 account： 账户对象

	| 属性或方法 | 描述 |
	|--------|--------|
	| positions<br>getPositions() | 当前账户的仓位信息数组|
	| activeOrders<br>getActiveOrders() | 当前账户的active order数组|
	| find(symbol/mtsSymbol)|得到指定symbol/mtsSymbol的合约的详情(InstrumentProperty)， <br>找不到返回空的InstrumentProperty（使用其isNull()方法判断）|

	* InstrumentProperty 对象

	| 属性或方法 | 描述 |属性或方法 | 描述 |
	|--------|--------|--|--|
	|symbol<br>getSymbol() | 合约代码 |mtsSymbol<br>getMtsSymbol() | MTS 规范化合约代码       |
	|name<br>getName() | 合约名称 |createdDate<br>getCreatedDate()|创建日期|
	|openDate<br>getOpenDate()|上市日|expireDate<br>getExpireDate()|到期日|
	|priceTick<br>getPriceTick() | 最小变动价位       |volumeMultiple<br>getVolumeMultiple() | 合约数量乘数   |
	|startDelivDate<br>getStartDelivDate()|开始交割日|endDelivDate<br>getEndDelivDate()|结束交割日|
  |isNull()|判断是否为空|toString()|返回字符串形式（JSON）|

##### 2.1.2 交易日变化通知  `onBusinessDateChanged()`  
此通知会在每个交易日开始的时候进行回调。一般用来初始化某些每日的数据和当日循环定时器。因为每个定时器的有效时间为一个交易日，所以一般在此处创建定时器。
```python
class Strategy(mts.StrategyBase):
	def onBusinessDateChanged (self,date):
    	...
```
* 参数 date： 当前交易日期



##### 2.1.3 定时器通知 `onTimer()`
一旦有某个定时器到了触发时间，MTS 进行此回调。如果创建了多个定时器，用户可以使用参数（timerId）来区分究竟是那个定时器被触发了，从而进行不同的逻辑处理。
```python
class Strategy(mts.StrategyBase):
	def onTimer (self,timerId):
    	...
```
* 参数 timerId： 此次回调触发的定时器的id

#### 2.2. 报价更新通知回调 
##### 2.2.1 实时报价更新通知 `onQuoteSnapshotUpdate()`  
当用户订阅的合约有报价更新时，触发此回调。
```python
class Strategy(mts.StrategyBase):
	def onQuoteSnapshotUpdate(self,quote):
    	...
```
* 参数 quote ： 实时报价对象当前快照

	| 属性 | 描述 |属性 | 描述 |
	|--------|--------|--|--|
  |symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
  |exchId| 交易所代码|ticksSinceEpoch|更新时间UTC 1970/01/01 00:00:00|
  |tradingDay|交易日期|dateTime| 更新时间（DateTime 对象）|
	|bidPrice| 申买价一|bidVolume| 申买量一|
	|askPrice| 申卖价一|askVolume| 申卖量一|
	|lastPrice| 最新成交价|lastVolume| 成交数量|
  |totalVolume|总成交量|preClosePrice| 昨收盘|
	|openPrice|今开盘|closePrice| 今收盘|
	|highPrice| 最高价|lowPrice| 最低价|
	|openInterest|持仓量|turnover|  成交金额|
	|upperLimitPrice| 涨停板价|lowerLimitPrice| 跌停板价|
	|settlementPrice| 本次结算价|preSettlementPrice| 上次结算价|
  |openInterest|持仓量|

	| 方法 | 描述 |方法 | 描述 |
	|--------|--------|--------|--------|
	|isTradeUpdate()|是否trade更新|isQuoteUpdate()|是否quote更新|
  |isBidUpate()|是否bid更新|isAskUpdate()|是否Ask更新|
  |isNull()|判断是否为空|toString()|返回字符串形式（JSON）|
  

##### 2.2.2 bar数据（分钟K线）更新通知 `onBarUpdate()`  
```python
class Strategy(mts.StrategyBase):
	def onBarUpdate(self,bar):
    	...
```
* 参数 bar ： bar数据对象

	| 属性 | 描述 |属性 | 描述 |
	|--------|--------|--------|--------|
	|symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
  |exchId| 交易所代码|tradingDay|交易日期|
  |beginTicksSinceEpoch|开始时间ticks|endTicksSinceEpoch|结束时间ticks|
  |beginDateTime|开始时间|endDateTime|结束时间|
	|openPrice| bar开始价格|closePrice| bar结束价格|
	|highPrice| bar最高价|lowPrice| bar最低价|
  |volume|成交总量|openInterest|持仓量|
  |vwap|成交量加权均价|spread|价差|
  |fairPrice|公允价格|bidAskDepth|报价深度|
  |intervalSec|bar 时间长度|turnover|成交金额|

	| 方法 | 描述 |方法 | 描述 |
	|--------|--------|--------|--------|
  |toString()|返回字符串形式（JSON）|

#### 2.3. 交易通知回调 
##### 2.3.1 下单成功通知 `onOrderNewDone()` 
```python
class Strategy(mts.StrategyBase):
	def onOrderNewDone (self,newDone):
    	...
```
* 参数 newDone ： 下单成功报告对象 

	| 属性 | 描述 |属性 | 描述 |
	|--------|--------|--------|--------|
  |symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
	|exchId| 交易所代码|priceType|市价单还是限价单|
	|referenceId| mts order id|orderExchId|order exch id|
  |tradingDay|交易日期|createTicksSinceEpoch|创建时间UTC|
  |directionSide|买卖方向|offsetFlag|开平标记|
  |price|下单价格|volume|下单数量|
  |fillVolume|成交数量|fillAmount|成交金额|
  |leftVolume|剩余数量|timeCondition|成交时间限制|
  |note|注释|createSrc|创建源|
  |instanceId|下单进程ID|strategyId|下单策略ID|

##### 2.3.2 下单失败通知 `onOrderNewReject()` #####
```python
class Strategy(mts.StrategyBase):
	def onOrderNewReject (self,newReject):
    	...
```
* 参数 newReject ： 下单拒绝报告对象

	| 属性 | 描述 |属性 | 描述 |
	|--------|--------|--------|--------|
  |symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
	|exchId| 交易所代码|priceType|市价单还是限价单|
	|referenceId| mts order id|orderExchId|order exch id|
  |tradingDay|交易日期|createTicksSinceEpoch|创建时间UTC|
  |directionSide|买卖方向|offsetFlag|开平标记|
  |price|下单价格|volume|下单数量|
  |leftVolume|剩余数量|fillVolume|成交数量|
  |fillAmount|成交金额|timeCondition|成交时间限制|
  |note|注释|createSrc|创建源|
  |instanceId|下单进程ID|strategyId|下单策略ID|

##### 2.3.3 成交回报通知 `onOrderFill()` 
```python
class Strategy(mts.StrategyBase):
	def onOrderFill (self,fill):
    	...
```
* 参数 fill ： 成交回报对象 

	| 属性 | 描述 |属性 | 描述 |
	|--------|--------|--------|--------|
  |symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
	|exchId| 交易所代码|priceType|市价单还是限价单|
	|referenceId| mts order id|orderExchId|报单交易所编号|
  |tradingDay|交易日期|createTicksSinceEpoch|创建时间UTC|
  |directionSide|买卖方向|offsetFlag|开平标记|
  |fillId|成交id|fillTicksSinceEpoch|成交时间|
  |fillPrice|成交价格|fillVolume|成交数量|
  |fillAmount|成交金额|note|注释|
  |price|报单价格|volume|报单数量|
  |instanceId|下单进程ID|strategyId|下单策略ID|
  |createSrc|创建源|

	| 方法 | 描述 |方法 | 描述 |
	|--------|--------|--------|--------|
  |toString()|JSON 格式字符串表示|

##### 2.3.4 撤单成功通知 `onOrderCancelDone()` 
```python
class Strategy(mts.StrategyBase):
	def onOrderCancelDone (self,cancelDone):
    	...
```
* 参数 cancelDone ： 撤单成功报告对象

	| 属性 | 描述 |属性 | 描述 |
	|--------|--------|--------|--------|
  |symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
	|exchId| 交易所代码|priceType|市价单还是限价单|
	|referenceId| mts order id|orderExchId|order exch id|
  |tradingDay|交易日期|createTicksSinceEpoch|创建时间UTC|
  |directionSide|买卖方向|offsetFlag|开平标记|
  |price|下单价格|volume|下单数量|
  |fillVolume|成交数量|leftVolume|剩余数量|
  |fillAmount|成交金额|timeCondition|成交时间限制|
  |note|注释|createSrc|创建源|
  |instanceId|下单进程ID|strategyId|下单策略ID|

##### 2.3.5 撤单失败通知 `onOrderCancelReject()` 
```python
class Strategy(mts.StrategyBase):
	def onOrderCancelReject (self,cancelReject):
    	...
```
* 参数 cancelReject ： 撤单拒绝报告对象

	| 属性 | 描述 | 属性 | 描述 |
	|--------|--------|--------|--------|
  |symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
	|exchId| 交易所代码|priceType|市价单还是限价单|
	|referenceId| mts order id|orderExchId|order exch id|
  |tradingDay|交易日期|createTicksSinceEpoch|创建时间UTC|
  |directionSide|买卖方向|offsetFlag|开平标记|
  |price|下单价格|volume|下单数量|
  |fillVolume|成交数量|leftVolume|剩余数量|
  |fillAmount|成交金额|timeCondition|成交时间限制|
  |note|注释|createSrc|创建源|
  |instanceId|下单进程ID|strategyId|下单策略ID|

##### 2.3.6 仓位更新通知 `onPositionUpdate()` 
```python
class Strategy(mts.StrategyBase):
	def onPositionUpdate(self, position):
    	...
```
* 参数 position： 仓位信息对象

	| 属性 | 描述 | 属性 | 描述 |
	|--------|--------|--------|--------|
  |symbol| 合约代码|mtsSymbol|MTS统一格式合约代码|
	|exchId| 交易所代码|
	|longOpenVolume|多头开盘仓位|shortOpenVolume|空头开盘仓位|
	|longCurrentVolume|多头持仓|shortCurrentVolume|空头持仓|
	|longActiveVolume|多头正在报单仓量|shortActiveVolume|空头正在报单仓量|
	|longCloseVolume|多头平仓量|shortCloseVolume|空头平仓量|
	|longActiveCloseVolume|多头正在报单平仓量|shortActiveCloseVolume|空头正在报单平仓量|
	|longCloseYesterdayVolume|多头平昨仓量|shortCloseYesterdayVolume|空头平昨仓量|
	|longActiveCloseYesterdayVolume|多头正在报单平昨仓量|shortActiveCloseYesterdayVolume|空头正在报单平昨仓量|
	|longLeftVolume|多头可平仓量|shortLeftVolume|空头可平仓量|
  |longLeftYesterdayVolume|多头剩余昨仓|shortLeftYesterdayVolume|空头剩余昨仓|
	|longFillVolume|多头成交仓量|shortFillVolume|空头成交仓量|
	|longFillAmount|多头成交金额|shortFillAmount|空头成交金额|
	|longOpenPnl|多头开盘盈亏|shortOpenPnl|空头开盘盈亏|
	|longFillPnl|多头成交盈亏|shortFillPnl|空头成交盈亏|
	|openPnl|开盘持仓盈亏|fillPnl|当日成交盈亏|
	|totalPnl|总和盈亏|attribute|当前仓位的合约信息|

	| 方法 | 描述 |方法 | 描述 |
	|--------|--------|--------|--------|
	|isNull()|判断是否为空|


## 3. MTS 对象系统 

### 3.1 系统描述对象 - mts 

1. 通过引入mts包的方式得到此对象：
```python
import mts.api as mts
```

1. 根据配置参数初始化MTS系统描述对象（三种调用形式）:
注意： 所有的MTS设施和方法必须在MTS系统初始化之后才能调用。
```python
mts.initialize(configParams)  #configParams 配置参数对象
mts.initialize(configJsonFile) # configJsonFile 配置参数文件路径（json格式的configParams 配置参数对象）
mts.initialize(configJsonFile,configParams) #结合上述两个参数，如果碰到同名的项，后面参数覆盖前面从文件里读取的项
```
 * 参数：configParams 配置参数对象
    不同的运行方式具备不同的配置参数，MTS 也是根据这些参数初始化相关的MTS组件。
    configParams 对象可以有如下参数项：
    * `instance_id`:(1~999)  指示当前python进程的id，用来在一个账户多个策略进程运行的情况下产生唯一的order id。
    * `mode`: 决定运行模式的参数项
        * `"simu"` 回测模拟模式 (**专业版提供**)
            * `clock_begin_date`: 回测开始日期。比如：20170124
            * `clock_end_date`: 回测结束日期。比如：20170424
		    * `feeds_begin_time`: （可默认）('hhmmsszzz') 指定feeds读取时间点。每天此时间点之前的报价将被忽略。`此参数用于加速某些只需要某点的报价的策略，不需要从开盘空跑。`
		    * `match_model`: 模拟交易的撮合模型。目前支持两种：乐观模型"fast" , 混合order 队列模型 "mixqueue"
        * `"beta"` 模拟测试模式 (**专业版提供**)
            * `feeds_front`: 报价服务器的协议地址和端口，包括用户名和密码。比如`"tcp://user:password@180.168.146.187:10013"`
            * `broker`: broker id
		    * `match_model`: 模拟交易的撮合模型。目前支持两种：乐观模型"fast" , 混合order 队列模型 "mixqueue"
        * `"real"` 实盘交易模式
            * `feeds_front`: 报价服务器的协议地址和端口，包括用户名和密码。比如`"tcp://user:password@180.168.146.187:10013"`
            * `trade_front`：交易服务器的协议地址和端口，包括用户名和密码。格式同上。
            * `broker`: broker id
    * `feeds_grade` ： （可默认）bar 数据的单位时间长度（秒）。比如： 60 表示一分钟bar, 0 表示关闭bar 数据更新通知回调（默认值）。
    * `continuous_contract`: （可默认）连续合约的配置json文件本地路径，默认读取config目录下的continuous_contract.json
    * `log_file`:  （可默认）指定log文件的路径。可以是目录或具体文件路径。如果是目录（默认为当前脚本所在路径下面的子目录log里），则使用时间戳产生log文件，如果是文件，则使用此文件。log 文件均是附加加入方式。
    
1. 开始执行MTS
此函数一般在脚本的最后一行调用，一旦调用MTS就进入事件循环开始工作。
```python
mts.execute()
```

1. 退出MTS
退出程序的执行。分为两种模式。
quit 内部调用sys.exit()直接退出程序
exit 退出MTS 事件循环进而退出程序
```python
mts.quit(exitValue)
mts.exit()
```
1. 转换hhmmsszzz时间格式为当天ticks
```python
mts.ticksFromMidnight(time)
```
1. 得到指定日期的连续合约id
```python
mts.getContinuousContract(productId,date)
```
1. 得到指定symbol的instrument 属性（等同于account.find()）
```python
mts.findInstrument(symbol)
```
1. 得到MTS初始化参数以及环境变量dict。可以用来从外部传递参数使用。
```python
mts.params()
```

1. mtsLog 的包装
调用MTS的log机制打印，其效果和MTS内部打印的一致。会包含时间戳，线程id等信息。
```python
mts.log("format %d,%s",2108,"hello")
mts.warn("format %d,%s",2108,"hello")
mts.error("format %d,%s",2108,"hello")
```

1. enums 的包装 
	1. Timezone
		* CST 中国标准时间
		* EST 美国东部时间
		* UTC 格林威治时间
	1. Direction
		* buy 买入开仓
		* sell 卖出平仓
		* short 卖出开仓
		* cover 买入平仓
	1. OpenCloseFlag
		* open 开仓
		* close 平仓
		* forceclose 强平
		* closetoday 平今
		* closeyesterday 平昨

1. tradingDate 交易日相关函数
	1. 得到指定交易日的下一个交易日，如果指定的日期非交易日返回无效日期99999999
	```python
    mts.nextTradingDate(date)
   ```
	2. 得到指定交易日的前一个交易日，如果指定的日期非交易日返回无效日期99999999
	```python
    mts.prevTradingDate(date)
   ```
	3. 得到指定交易日的下N个交易日，如果指定的日期非交易日或N超过有效范围，返回无效日期99999999
	```python
    mts.nextNTradingDate(date,n)
   ```
	4. 判断指定日期是否是交易日
	```python
    mts.isTradingDate(date)
   ```
	5. 转换指定日期为交易日。如果指定日期是交易日直接返回，否则返回最近的下一个交易日。
	```python
    mts.forceToTradingDate(date)
   ```
	
### 3.2. 策略对象 - StrategyBase 

策略对象几乎封装了对MTS的所有操作和回调函数。

#### 3.2.1 构造函数- \_\_init\_\_(self,name,id) 
```python
class Strategy(mts.StrategyBase):
    def __init__(self,name,id):
        super(MyStrategy, self).__init__(name,id)
```
* 参数 `name`: 策略名称，默认用类名即可。
* 参数 `id`: （1~9）策略id,用于在多个策略运行在一个进程中的情况下，区别不同的策略。MTS 最多允许9个策略同时运行在一个python进程中。默认用1即可。

#### 3.2.2 查询交易账户信息 - getAllTradingCounts 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		accounts=self.getAllTradingCounts()
```
* 返回值：交易账户列表数组。
* TradingAccount 交易账户 

| 属性 | 描述 |
|--------|--------|
|id|交易账户ID|
|commission| 手续费|
|available| 可用资金|
|positionProfit/posProfit| 持仓盈亏|
|closeProfit| 平仓盈亏 |
|margin|当前保证金总额 |


#### 3.2.3 报价订阅 - self.subscribe() 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.subscribe(symbolArray)
```
* 参数 `symbolArray`: 订阅标的symbol/product 数组。如果只指定品种，则订阅continuous_contract.json中定义的主力合约

#### 3.2.4 取消报价订阅 - self.unsubscribe() 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.unsubscribe(symbolArray)
```
* 参数 `symbolArray`: 取消订阅标的symbol 数组


#### 3.2.5 取消所有报价订阅 - self.unsubscribeAll() 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.unsubscribeAll()
```

#### 3.2.6 查询当前报价快照 - self.getQuote() 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.getQuote(symbol)
```
* 参数 `symbol`: 查询的symbol

#### 3.2.7 查询当前bar数据 - self.getBar(symbol,prevCount) 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.getBar(symbol,10)
```
* 参数 `symbol`: 查询的symbol
* 参数 `prevCount`: 从当前时间往前的多少个bar
* 返回值：返回一个包含多个bar的数组


#### 3.2.8 查询当前日K线数据 - self.getDaily(symbol,prevCount) 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onInitialized(self,account):
		self.getDaily(symbol,10)
```
* 参数 `barJsonFile`: 一json 格式存放的bar数据文件。每一行表示一条bar。是一个bar 的json 表示形式（bar.toString()）
* 返回值：布尔值，载入成功与否

#### 3.2.9 创建一个新指标 (**专业版提供**)- self.indicator(name,symbol,params) 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onInitialized(self,account):
		self.ma5=self.indicator("MA","AG1712",{"n":5})
```
* 参数 `name`: 欲创建的indicator的名称，比如MA，EMA，MACD等。MTS 提供一些基本的指标计算 （`具体详见MTS标准指标系统`）
* 参数 `symbol`:  指标计算的symbol
* 参数 `params`:  指标计算的参数，不同指标不尽相同。
* 返回值：指标对象，一般存放进self里，便于获取数据。


#### 3.2.10 销毁释放一个指标 (**专业版提供**)- self.freeIndicator(indicator) 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.freeIndicator(self.MA5)
```
* 参数 `indicator`: 欲销毁的indicator对象。由self.indicator()创建出来的对象。


#### 3.2.11 委托下单 -  self.newOrder() 

```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.newOrder(orderRequest)
```
* 参数 `orderRequest`: 下单参数，分为Direct order , Net order 和 Box order 三种

* Direct order 的orderRequest

| 属性 | 描述 |
|--------|--------|
|type|order类型，必须为整数1或缺省 (**专业版提供，开源版默认**)|
|symbol| 合约代码|
|price| 价格，默认为0 表示市价单|
|volume|  数量，正数|
|direction|  方向由mts的四个常量值表示:<br> `mts.Direction.buy` <br>`mts.Direction.sell`  <br>`mts.Direction.short` <br> `mts.OrderType.cover`|
|offsetFlag| 开平仓标志:<br>`mts.OpenCloseFlag.open`(buy/short 的缺省值)<br>`mts.OpenCloseFlag.close`<br>`mts.OpenCloseFlag.closetoday`(sell/cover的缺省值)<br>`mts.OpenCloseFlag.closeyesterday`|

Direct Order 的辅助别名方法：
```python
self.newDirectOrder(symbol,price,volume,direction,offsetFlag)
```

* Net Order 的orderRequest： 此order 保证最小仓位，能平仓就平仓。 (**专业版提供**)

| 属性 | 描述 |
|--------|--------|
|type|order类型，必须为整数2。|
|symbol| 合约代码|
|price| 价格，默认为0 表示市价单|
|volume|  数量，正数为buy，负数为sell|
Net Order 的辅助别名方法：
```python
self.newNetOrder(symbol,price,volume)
```

* Box Order 的orderRequest： 此order 建立box仓位，节省平今手续费。(**专业版提供**)

| 属性 | 描述 |
|--------|--------|
|type|order类型，必须为整数3。|
|symbol| 合约代码|
|price| 价格，默认为0 表示市价单|
|volume|  数量，正数为buy，负数为sell|

Box Order 的辅助别名方法：
```python
self.newBoxOrder(symbol,price,volume)
```


#### 3.2.12 撤单 - self.cancelOrder() 

```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.cancelOrder(cancelRequest)
```
* 参数 `cancelRequest`: 撤单参数

| 属性 | 描述 |
|--------|--------|
|symbol| 合约代码|
|referenceId| mts order id|


#### 3.2.13 查询所有待成交单 - self.getActiveOrders()
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		orders=self.getActiveOrders(symnol)
```
* 返回值：返回所有的待成交单的数组；如果没有待成交单，返回空数组`[]`

#### 3.2.14 查询所有仓位信息 - self.getAllPositions() / self.allPositions 
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
        positions=self.allPositions	#positions=self.getAllPositions()
```
* 返回值：返回所有的仓位信息；如果没有，返回空数组`[]`

#### 3.2.15 查询指定合约的仓位信息 - self.getPosition()
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		pos=self.getPosition(symbol)
```
* 返回值：返回对应的仓位信息；如果没有，返回空仓位对象（使用isNull() 判断）

#### 3.2.16 创建单次定时器 - self.newOnceTimer() 
创建新单次定时器，在指定时间点单次触发回调
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		timerId=self.newOnceTimer(time)
```
* 参数 `time`: 定时器触发时间点（hhmmsszzz）
* 返回值：返回定时器 id，以便标识不同的定时器


#### 3.2.17 创建当日循环定时器 - self.newTimer() 
创建新的当日循环定时器，根据指定时间间隔反复触发回调。有效范围为当天内。
```python
 class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		timerId=self.newTimer(beginTime,interval)
```
* 参数 `beginTime`: 定时器启动时间点（hhmmsszzz）
* 参数 `interval`: 定时器间隔毫秒
* 返回值：返回定时器 id，以便标识不同的定时器

#### 3.2.18 取消循环定时器 - self.removeTimer()
取消指定定时器，不再触发|
```python
class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		self.removeTimer(timerId)
```
* 参数 `timerId`: 创建定时器时返回的定时器id


#### 3.2.19 得到当前时间 - self.getNow() / self.now
```python
 class Strategy(mts.StrategyBase):
	def onSomeEvent(): #eg.  def onQuoteSnapshotUpdate(self,qt):
		now=self.now #self.getNow() 
```
* 返回值：返回当前系统时间对象。real/beta 模式下等于当前系统时间，simu 模式下等于回测点对应的历史时间

	| 属性或方法 | 描述 |属性或方法 | 描述 |
	|--------|--------|--------|--------|
	|date<br/>getDate()|日期。<br/>比如： 20160205|time<br/>getTime()|时间。<br/>比如： 122100000|
	|ticksSinceMidnight<br/>getTicksSinceMidnight()|当天的ticks 数<br/>从午夜00：00：00开始|
	|year<br/>getYear()|年份|hour<br/>getHour()|小时|
  |month<br/>getMonth()|月份|minute<br/>getMinute()|分钟|
	|day<br/>getDay()|日|second<br/>getSecond|秒|
	|toUTCMillisecsSinceEpoch()|UTC ticks <br/>from 1970/1/1 00:00:00|millisecond<br/>getMilliSecond()|毫秒|
	|isUTC()|判断是否是UTC时间|toUTC()|转换到UTC 时间|
	|isNull()|判断是否为空|toString()|字符串表示|


## 4. MTS 标准指标系统 (**专业版提供**)

### 4.1 MTS 指标对象 
所有的指标对应同样的接口，区别只是参数不同和背后的算法不同。
指标对象封装了统一的操作：

#### 4.1.1 指标名称 getName()/name 

* 返回值:此指标对象对应的指标名。比如：MA，EMA 等

#### 4.1.2 指标帮助说明信息 getHelpInfo()/helpInfo 

* 返回值:此指标的帮助信息。比如：名称，参数列表等

#### 4.1.3 指标当前值 value(name="") 

* 参数 name: 获取指标的某个值。对于多值的指标使用（如：KDJ），单值的指标不需指定。
* 返回值: 此指标的当前值

#### 4.1.4 指标添加新数据append(value) 

* 参数 value: 给指标添加新的数据点的值。

#### 4.1.5 指标更改最近一次添加的数据updateLast(value) 

* 参数 value: 修改此指标最后一次添加的数据点的值。

#### 4.1.6 清空指标，重新计算 clear() 
清空指标的所有数据点，重新使用。

### 4.2 MTS 标准内置指标 
MTS 提供一些标准指标，由用户使用。如下：

#### 4.2.1 MA - 移动平均线（Moving Average）
参数表如下：
* `n` ：移动数据点的个数

#### 4.2.2 EMA - 指数平均数指标（Exponential Moving Average）
参数表如下：
* `n` ：移动数据点的个数
* `alpha` ：平滑指数，默认值为2/(n+1)

#### 4.2.3 MACD - 指数平滑移动平均线指标（Moving Average Convergence / Divergence）
参数表如下：
* `short`：短期指数平滑移动平均线数据点的个数，默认值为12
* `long`：长期指数平滑移动平均线数据点的个数，默认值为26
* `dea` : 差离值指数平滑移动平均线数据点的个数，默认值为9
* `alpha`:平滑指数，默认值为2/(n+1)

#### 4.2.4 KDJ - 随机指标（Stochastics）
参数表如下：
* `k_n`: K移动数据点的个数
* `k_alpha`:K平滑指数，默认值为2/(n+1)
* `d_n`：D移动数据点的个数
* `d_alpha` ：D平滑指数，默认值为2/(n+1)
* `j_k_adjust`:J的K值因子，默认值为3
* `j_d_adjust`:J的D值因子，默认值为2


## 5. 附录 
### 5.1 一个例子 - MyStrategy 

```python
import mts.api as mts
class MyStrategy (mts.StrategyBase):
    def __init__(self,name,id):
        super(MyStrategy, self).__init__(name,id)
    
    def onInitialized(self,account):
        print 'python onInitialized'
        self.ag1706Info=account.find("ag1706")
        self.subscribeQuotes(['ag1706'])
        self.barClose=999999

	def onBusinessDateChanged(self,date):
        print "Business Date changed to ",date
   	
    def onQuoteSnapshotUpdate(self,qt):
        last=qt.lastPrice
        if(and qt.askPrice>self.barClose):
            print "python: send order"
            order={
            	'type':2,
                'symbol':qt.symbol,
                'volume': 1,   #1 表示 buy/cover 1 ； -1 表示 sell/short 1
                'price':qt.askPrice,
            }
            ordId=self.newOrder(order)
    
    def onBarUpdate(self,bar):
        print "onBarUpdate:",bar
        self.barClose=bar.closePrice

if __name__ == '__main__':
    strategy=MyStrategy("MyStrategy",1)   # 创建策略类实例
    simuParams={							#构建参数
        "instance_id":517,
		"mode":"simu",
        "clock_begin_date": 20170124,
        "clock_end_date": 20170424,
        "feeds_grade":60,
        "match_model":"mixqueue"
    }
    ret=mts.initialize(simuParams)) 		#初始化mts环境
    if ret:
        mts.execute()					   #进入mts事件循环，开始运行
    else:
        sys.exit(1)			
```


