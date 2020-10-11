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

## Customization
You could customize your bar via editing `config.h` and then compiling it.


