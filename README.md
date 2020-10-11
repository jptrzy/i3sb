# i3sb
I3 Simple Bar is as in the name, simple bar to i3 window manager, with some pywal suport.

## Installation

**Manual/Git install**
```
git clone https://github.com/jptrzy/i3sb
cd i3sb
make
sudo make install
```

**Past Installation**

If you get i3sb installed you must edit i3 config file 
from
```
bar {
    status_command i3status
}
```
to
```
bar {
    status_command i3status
}
```
and then reload i3 to see results.

**Manual/Git uninstall**
'''
sudo make uninstall
```

## Signal Changing

If you want send command to special block to update you could do that. For example pulseaudio modul normaly don't update, but if you run 'pkill -RTMIN+1 i3sb' it will update. All blocks have **Signal** value taht could be in range of 0-30. This commend 'pkill -RTMIN+<Signal> i3sb' will update block with varible **Signal** equal to '<Signal>'.

## Customization
You could customize your bar via editing `config.h` and then compiling it.


