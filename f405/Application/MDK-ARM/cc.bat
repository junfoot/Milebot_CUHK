@echo off

set str1=%cd%
cd ..
set str2=%cd%

set tmp1=%str1%
set tmp2=%str2%
::��str1���Ƶ�str
:next1
::��ǩ������goto��ת
::ע����ע����������ע��������ð�ſ�ͷ����ǩ��Ϊһ��ð��
if not "%tmp2%"=="" (
::�ж�str�ǲ��ǿմ������������ִ���±ߵ����
set /a num1+=1
set /a num2+=1
::�������㣬ʹnum��ֵ����1���൱��num++����++num���
set "tmp1=%tmp1:~1%"
set "tmp2=%tmp2:~1%"
::��ȡ�ַ�������������
goto next1
::��ת��next1��ǩ
::��������goto�ͱ�ǩ������ѭ���ṹ
)

set "tmp1=%tmp1:~1%"

cd %tmp1%

set prjName=%tmp1%
title Milebot %prjName%����������...
echo �������%prjName%����,���Ե�......


rmdir /S /Q .\DebugConfig
del /f /s /q .\list\*
del /f /s /q .\obj\*.*

del /f /s /q .\debug.log
del /f /s /q .\JLinkLog.txt
del /f /s /q .\JLinkSettings.ini
del /f /s /q .\%prjName%.bin
del /f /s /q .\*.uvguix.*
del /f /s /q  .\EventRecorderStub.scvd

echo ���%prjName%�������!
::echo. & pause
