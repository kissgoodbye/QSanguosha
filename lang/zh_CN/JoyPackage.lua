-- translation for JoyPackage

return {
	["joy"] = "欢乐", 
	["shit"] = "屎", 
	[":shit"] = "当此牌在<font color='red'><b>你的回合</b></font>内从你的<font color='red'>手牌</font>进入<font color='red'>弃牌堆</font>时，\
	你将受到自己对自己的一点伤害，其中梅花为雷电伤害，红桃花色产生火焰伤害，其他为无属性伤害\
	造成伤害的牌为此牌，在你的回合内，你可多次食用", 
	
	["deluge"] = "洪水",
	[":deluge"] = "出牌阶段，将【洪水】横置于你的判定区里，回合判定阶段进行判定：若判定结果为 A,K，从当前角色的牌随机取出和场上存活人数相等的数量置于桌前，从下家开始，每人选一张收为手牌，将【洪水】弃置。若判定结果不为AK，将【洪水】移到当前角色下家的判定区里",
	
	["typhoon"] = "台风",
	[":typhoon"] = "出牌阶段，将【台风】横置于你的判定区里，回合判定阶段进行判定：若判定结果为方块2~9之间，与当前角色距离为1的角色弃掉6张手牌，将【台风】弃置。若判定结果不为方块2~9之间，将【台风】移动到当前角色下家的判定区里",
	
	["earthquake"] = "地震",
	[":earthquake"] = "出牌阶段，将【地震】横置于你的判定区里，回合判定阶段进行判定：若判定结果为梅花2~9之间，与当前角色距离为1以内的角色弃掉装备区里的所有牌，将【地震】弃置。若判定结果不为梅花2~9之间，将【地震】移动到当前角色下家的判定区里",
	
	["volcano"] = "火山",
	[":volcano"] = "出牌阶段，将【火山】横置于你的判定区里，回合判定阶段进行判定：若判定结果为红桃2~5之间，当前角色上家受到3点火焰伤害；若判定结果为红桃6~9之间，当前角色下家受到3点火焰伤害，【火山】生效后即弃置。若判定结果不为红桃2~9之间，将【火山】移动到当前角色下家的判定区里",

}