selfx = Self.get(selfPtr)

selfx:sayNextGroup(
    "#r�� 1 ��#k", 
    "#b�� 2 ��#k", 
    "#g�� 3 ��#k", 
    "�� 4 ��", 
    "�� 5 ��"
    );


selfx:askAvatar("Select : ", 4031249
    ,30030
    ,30020
    ,30000
    ,30480
    ,30310
    ,30330
    ,30060
    ,30150
    ,30410
    ,30210
    ,30140
    ,30120
    ,30200
)

s = selfx:askMenu("#L0#Test0#l\r\n#L1#Test1#l\r\n#L2#Test2#l\r\n#L3#Test3#l")
s = selfx:sayNext("��ܤF�ﶵ:" .. s)

s = selfx:askYesNo("���� Yes No")
if(s == 1) then
    selfx:say("Press Yes")
else
    selfx:say("Press No")
end

s = selfx:askText("��J�Y�Ǥ�r:", "��r", 0, 5)
selfx:sayNext("��J����r : " .. s)

s = selfx:askNumber("��J�Y�Ǽƭ�:", 100, 0, 10000)
selfx:sayNext("��J���ƭ� : " .. s)

s = selfx:sayNext("����")