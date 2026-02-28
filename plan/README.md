# plan 文件夹

该文件夹包含查询计划相关的实现，负责生成和优化SQL查询的执行计划。

## 内容

- **BasicQueryPlanner.c/h**: 基本查询规划器，生成简单的查询执行计划。
- **BasicUpdatePlanner.c/h**: 基本更新规划器，生成简单的更新执行计划。
- **BetterQueryPlanner.c/h**: 改进的查询规划器，生成更优化的查询执行计划。
- **OptimizedProductPlan.c/h**: 优化的乘积计划，改进了连接操作的性能。
- **Plan.c/h**: 查询计划的基类和通用接口。
- **Planner.c/h**: 规划器的主入口，协调查询计划的生成。
- **ProductPlan.c/h**: 乘积计划，实现表连接操作。
- **ProjectPlan.c/h**: 投影计划，实现SELECT子句的投影操作。
- **SelectPlan.c/h**: 选择计划，实现WHERE子句的选择操作。
- **TablePlan.c/h**: 表计划，实现表扫描操作。

## 用途

- 生成SQL查询的执行计划
- 优化查询执行计划，提高查询效率
- 支持各种查询操作（选择、投影、连接等）
- 实现不同的查询优化策略
- 协调查询计划的生成和执行