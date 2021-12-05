:: 自动提交
git add . 
set /p var="Please into commit message: "
@echo git commit -m "bat auto push:%date:~0,10%,%time:~0,8%" 
@git commit -m  "%VAR%"
git push STM32_gitee master
@echo 已经完成
pause