<h1 align="center">DBLParse 科学文献管理系统</h1>
<p align="center">
  <a href="https://github.com/tootal/DBLParse/releases">
    <img alt="Github Release" src="https://img.shields.io/github/release/tootal/DBLParse.svg" target="_blank" />
  </a>
  <a href="https://github.com/tootal/DBLParse/actions">
    <img alt="Github Actions" src="https://github.com/tootal/DBLParse/workflows/Windows/badge.svg?branch=master" target="_blank" />
  </a>
</p>
[数据结构大作业任务书](docs/TASKBOOK.md)  
[开发文档](docs/CONTRIBUTING.md)  
[关于DBLP的一些信息](docs/DBLPXML.md)  

## 功能实现

F1. （必须实现） 基本搜索功能。输入作者名，能展示该作者发表的所有论文信息。输入完整的论文的题目，能展示该论文的其他相关信息。

F2. （必须实现） 相关搜索。输入作者名，能展示于该作者有合作关系的其他所有作者。

F2. （必须实现） 作者统计功能。输出写文章最多的前100名作者。

F3. (扩展实现) 热点分析功能。分析每一年发表的文章中，题目所包含的单词中，出现频率排名前10的关键词。

F4. (扩展实现) 部分匹配搜索功能。给定若干个关键字，能快速搜索到题目中包含该关键字的文章信息

F5. (扩展实现) 聚团分析。作者之间的合作关系可以看成是一个图，每个作者对应一个顶点，任两个作者之间如果存在合作关系，则在两个顶点之间建立连边。这个图中的每一个完全子图我们称为一个聚团（所谓完全子图指的是该子图的任意顶点都和该子图的其他顶点有连边，完全子图的顶点个数称为该完全子图的阶数），请统计整个图中各阶完全子图的个数。

F6. (扩展实现) 可视化显示。通过图形化界面，展示作者之间合作关系图及其相关文章信息。
