# 数据结构大作业任务书
选题：DBLParse（科学文献管理系统）
请发挥你的想象力，选用合适的数据结构与算法。需要理解的是，数据结构大作业重点考核的是数据结构和算法的运用；好的界面呈现可以锦上添花，但并不是决定因素。

## 设计内容
科研工作者的日常工作离不开查阅科学文献，并对其中的信息进行分析、筛选、挖掘和管理。请你为科研工作者设计一个管理系统，提高科学文献的管理效率。

目标用户：科研工作者。

数据配置：请通过以下方法下载数据文件dblp.xml.gz.

http://dblp.uni-trier.de/xml/dblp.xml.gz

将该数据文件解压后，其中包含一个dblp.xml文件。该文件由科学文献的记录序列组成，记录的格式如下所示：

```
 <article mdate="2002-01-03" key="persons/Codd71a">
<author>E. F. Codd</author>
<title>Further Normalization of the Data Base Relational Model.</title>
<journal>IBM Research Report, San Jose, California</journal>
<volume>RJ909</volume>
<month>August</month>
<year>1971</year>
<cdrom>ibmTR/rj909.pdf</cdrom>
<ee>db/labs/ibm/RJ909.html</ee>
</article>
```

每个记录对应一篇文章，其中包含对作者，题名，发表杂志，卷号，出版时间等的详细说明。请基于该数据，设计能满足后述功能的文献管理系统。

注：
1）dblp.xml的大小超过1G，所以不要直接点击打开该文件。需要通过命令行命令’more’ 或者自行编程查看。
2） 编程过程中，不允许使用数据库系统。需要自己建立管理文献数据的数据结构。

### 功能要求
F1. （必须实现） 基本搜索功能。输入作者名，能展示该作者发表的所有论文信息。输入完整的论文的题目，能展示该论文的其他相关信息。

F2. （必须实现） 相关搜索。输入作者名，能展示于该作者有合作关系的其他所有作者。

F2. （必须实现） 作者统计功能。输出写文章最多的前100名作者。

F3.  (扩展实现)   热点分析功能。分析每一年发表的文章中，题目所包含的单词中，出现频率排名前10的关键词。

F4.  (扩展实现)  部分匹配搜索功能。给定若干个关键字，能快速搜索到题目中包含该关键字的文章信息

F5.  (扩展实现) 聚团分析。作者之间的合作关系可以看成是一个图，每个作者对应一个顶点，任两个作者之间如果存在合作关系，则在两个顶点之间建立连边。这个图中的每一个完全子图我们称为一个聚团（所谓完全子图指的是该子图的任意顶点都和该子图的其他顶点有连边，完全子图的顶点个数称为该完全子图的阶数），请统计整个图中各阶完全子图的个数。

F6.  (扩展实现) 可视化显示。通过图形化界面，展示作者之间合作关系图及其相关文章信息。


## 时间安排
时间和进度安排建议如下：

|            项  目  内  容             |          时间           |
| ------------------------------------ | ----------------------- |
| 选定题目和任务分配                     | 2019.1.1 ～ 2019.1.15   |
| 需求分析和概要设计                     | 2019. 1.21 ～ 2019.3.1  |
| 算法设计和编码调试                     | 2019. 3.2 ～ 2019.4.15  |
| 系统整合和撰写文档                     | 2019. 4.16 ～ 2019.4.20 |
| 答辩（包括结果演示、提交小组和个人文档）| 2019年4月下旬            |

## 设计要求

1. 根据问题的工作量和难易程度，由2~4名同学组队完成。

2. 下学期将安排大作业答辩。答辩以小组为单位进行答辩，论述实现的软件的主要功能、所采用的数据结构和核心算法，以及测试的结果。

3. 所实现的程序，须满足以下要求：

界面友好：图形界面，支持鼠标操作。有合理的中文提示，每个功能可以设立菜单，根据提示，可以完成相关的功能。出现非法输入，会给出异常提示。

物理存储：相关数据要求存储在数据文件中，在程序中完成文件的读写操作。

逻辑结构：根据问题的要求，采用线性或非线性结构。需要考虑大数据量问题。

4. 每位同学还需要针对自己所承担的部分工作，提交报告文档。该文档是评分的重要依据之一，请认真对待。该文档须包括如下内容：

需求分析： (陈述要解决的问题，要实现的功能),

详细设计：包括设计算法流程图、算法分析、使用的数据结构 （要求详细论证）；

软件测试：包括测试数据和测试结果记录

总结： 设计过程中遇到的问题及解决方法；尚未解决的问题及考虑应对的策略；收获和心得；

参考文献：在提交的设计报告中必须要有参考文献！必须按学校规定的模板来写！可以是课本以及与课程设计相关的工具书、论文、网页等。要求在报告中按规定格式详细列举所用参考文献。

具体格式请参照《课程设计报告书》模板。

5. 在提交的设计报告中，要能体现出合理的分工和充足的工作量。要求每人都要交设计报告，系统整体设计部分可以相同或相似，但自己做的部分内容要详细。

6. 以小组作为单元，提交如下内容：   

源代码。要求注释清晰，编写规范，模块化。编程语言: C++/Java/phython/…

可运行的文件。

答辩PPT。
