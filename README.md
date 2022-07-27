# ShootingGameDemo
这是一个单人TPS游戏的小Demo，个人练习的成果汇总。主要涉及的技术点：
- 武器系统：枪支的装备和挂载、切换、丢弃和枪支换弹；
- 属性系统：角色具备生命值和体力值，受到AI攻击生命减少，奔跑消耗体力，属性变化绑定动态多播委托
- 交互系统：通过射线检测可以与实现Interface的Actor进行交互，完成诸如枪支武器的拾取；
- 动作系统：使用GameplayTags控制不同动作（例如开火、奔跑、换弹）的切换，同时避免动作干涉；
- AI系统：使用AIPerception进行玩家搜索，使用Task、Service和EQS协调控制AI的追击、攻击和躲避行为；
- 商店系统：玩家可使用积分购买商品，目前尚在构思阶段；
- GameplayBlueprintLibrary：定义了一些常用的全局静态函数，如GameplayTags的比较，伤害的传递等；
- PlayerController：控制玩家的属性和组件更新和重置；
- PlayerState：存储玩家在当局游戏的积分，积分可通过击杀AI获取；
- GameMode：控制不同时间AI的生成位置和生成数量，控制玩家的死亡重生，定义游戏的结束条件；
- 存档系统：借助Subsystem和SaveGame实现，主要用于存储玩家的属性和积分，目前尚未完成。