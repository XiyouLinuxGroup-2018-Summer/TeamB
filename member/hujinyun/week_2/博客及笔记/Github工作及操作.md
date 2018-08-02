# Github的工作原理

标签： github

---这几天一直都在使用Github提交代码，日程。但是只是简单的git add， git push感觉懂得很皮毛的皮毛。有点好奇团队的工作原理，那就来记录一下吧^O^ 

![github的工作原理图][1]


 - [x] 蓝色代表功能分支，负责开发某一工作的工作流。
 - [x] 橙色代表功能总汇分支，里边是所有功能的汇总
 - [x] 黄色表示测试分支，用于产品内侧
 - [x] 紫色表示bug修复分支，只已发布的产品发现有bug时进行修复的分支。
 - [x] 绿色表示发布分支，用于发布产品，对外展示
 
虽然以上我大部分用不到（小声bb不会用），但是了解一下嘛。
如果团队在做开发,A做语音功能开发,B做页面功能开发,C做聊天功能开发..那么ABC三人都是在蓝色的Feature分支上进行各自的工作,平行方向的每个箭头则是pull request的结果.

当他们觉得自己负责的功能写的差不多了,就可以pull resquest 到橙色的Develop分支.

管理员D将程序员ABC推送过来的内容进行汇总,就可以 pull request 到橙色的Develop分支了.

如果测试合格就可以pull resquest到绿色的Master分支发布了.如果不合格就修改合格,然后再pull request到Master分支发布,同时pull request到橙色的总汇分支进行备份.

一个合格的产品就上线了.

当然不可能事事完美.已经上线的产品被发现有bug时,紫色的Hotfix分支的作用就出来了,这时就需要将Master分支pull request给Hotfix进行漏洞修复,修复完成后重新pull request 给Master发布,同时Pull request给橙色的总汇总备份.

---
---此时是没有分支时的团队操作可能出现的问题(分支在下边)
当我们把文件往git版本库里添加的时候是分两步执行的:
1.用git add将文件添加进去,实际就是把文件修改添加到暂存区.
`git add name(本地文件(夹)名) `
2.用git commit提交更改,实际是吧暂存区的所有内容提交到当前分支.
`git commit -m "提交进行的注释"`
![git提交过程][2]

然后如果是进行团队合作时,如果远程仓库与你的本地仓库不一致时(在你对你的本地仓库进行修改期间,其他队友或许在此期间给团队的仓库提交了新的代码),直接 `git push `就会出现"无法推送引用,更新被拒绝"
![git push不成功][3]

所以我们需要更新远程仓库的代码本地仓库(fetch),然后将内容合并到当前分支(merge)
`git pull`进行更新将远程仓库更新文件更新到本地,将自己仓库与远程仓库进行整合.
整合完成后.就可以`git push`了
但是在这里我还遇到过另一个问题,当你已经提交过一个文件,而你在本地做了修改.但是你下次提交时并不想提交此文件,而是选择其他文件进行提交,就会出现"请在合并前提交您的修改或者保存进度。"
此时需要你要不提交新本地版本,要不放弃本地版本,在git pull整合分支时被git上的版本覆盖掉.
解决:输入一下命令：
git stash
git stash pop
再根据终端提示进行操作(上传或覆盖)
(具体图等下次再遇到再贴)

---
当我们在进行团队合作时为了不想每次更新都将别人的新提交的信息更新到本地仓库,我们可以新建一个自己的分支
###注:以下都为终端操作
#以下为Github分支的操作
首先将当前目录设置为仓库目录
一.创建本地分支
1.查看有哪些分支:`git branch`
2.创建一个分支:`git branch name` name是你想创建的分支名
3.切换到分支:`git checkout name`
下面就可以在当前分支上进行相关的文件操作了.
<mark>注意:如果用`git checkout master`切换到主分支,在name分支下进行的文件变更的内容无法被看到.</mark>切换回name分支就又可以看到了.
二.提交分支到github服务器
`git push orgin name`
三.将分支的更新信息内容合并到master分支
1.切换到master分支,`git checkout master`
2.合并name分支到当前master分支:`git mergename` 
注意:这时合并到master上的内容还没有提交到github上,需要push.
3.将新分支发布到github上`git push orgin name` 
创建分支大功告成!

以下为其他分支操作
四.删除分支
删除本地分支:`git branch -d name`
删除服务器上的分支:`git push orgin:name`与三.3操作对应的哦, 分支名前的冒号代表删除.

五.clone分支
1.克隆github上的仓库到本地,默认会把仓库的所有内容clone到本地.`git clone git@.....(下图点击复制的内容)`
![git clone][4]
2.但此时只会在本地创建一个master分支.这时需要用` git branch -r` 才能看到所有分支的名字.
3.此时使用 `git checkout name(分支名)`操作就把远程分支取用到本地.
此时,不用输入 -r 的`git branch`就可以看到刚才操作的分支名啦

当你想在你的分支和主分支之间切换提交时 用`git checkout name(分支名)`
后进行常规操作 常规操作和可能出现的问题在上一块内容.



---

fork和clone的区别:
 1，如果我的目的是把专案搬到GitHub上，就需要fork； 2，如果我的目的是把专案搬到我的电脑上，就需要clone； 3，如果我的目的想对别人的专案进行修改，就需要先fork到自己的Github上，然后从自己的Github上clone专案到自己的电脑，然后修改，修改完之后push到自己的专案，然后在自己的GitHub上pull request，这样专案所有者会收到请求，并决定要不要接受你的代码；

  fork的对象是仓库的全部内容，而pull resquest的对象是各分支



  [1]: https://upload-images.jianshu.io/upload_images/2551650-d9749a84a1389f8f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700
  [2]: https://images2017.cnblogs.com/blog/886183/201711/886183-20171115205843843-801821514.jpg
  [3]: https://img-blog.csdn.net/20180731155324583?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  [4]: https://img-blog.csdn.net/20180731163027975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70

参考:http://www.cnblogs.com/schaepher/p/4933873.html
