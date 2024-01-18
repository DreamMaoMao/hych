# hych
Window minimization plugin for hyprland.

https://github.com/DreamMaoMao/hych/assets/30348075/9d114048-a260-4447-bbe3-2162e8c86ac9



### what can it do:
- Support minimize window and restore window by clicking the icon on the waybar

- Support customize the minimization status on the waybar

- Support shortcut keys to minimize Windows and restore Windows

- Support for minimize buttons in app

This plugin uses the special workspace built into hyprland to hide windows, aiming to enhance the functionality of the special workspace, so it can also be compatible with the previous special workspace operations.

For example, you can restore the windows that you put into the special workspace by the old way by clicking the waybar icon or using the shortcut keys of this plugin to the normal workspace.
  
### Manual Installation

##### something need to notice
1.You need to manually relogin after it is installed. 
after the installation is complete, it may also trigger logout if you do not logout which is normal.

2.Only supports hyprland source code after 2023-10-22.
because the plugin requires this [commit](https://github.com/hyprwm/Hyprland/commit/a61eb7694df25a75f45502ed64b1536fda370c1d) in hyprland(was commited in 2023-10-21)


##### using cmake:

```shell
git clone https://github.com/DreamMaoMao/hych.git
cd hych
bash install.sh # `libhycov.so` path: /usr/lib/libhycov.so
```

### Usage (hyprland.conf)

```conf
plugin = /usr/lib/libhych.so

bind=ALT,i,hych:minimize
bind=ALT_SHIFT,i,hych:restore_minimize

```
- when the window is not active, clicking the waybar icon button will activate the window 
- when the window is active, clicking the icon button will minimize the window
- when the window has been minimized, clicking the icon button will restore the window

# waybar config demo
<details>
<summary>module config</summary>

```conf
"wlr/taskbar": {
    "format": "{icon}",
    "icon-size": 30,
    "all-outputs": false,
    "tooltip-format": "{title}",
    "on-click": "activate",
    "on-click-right": "close"
  },
```
</details>
<details>
<summary>css style</summary>

```css
#taskbar{
    background-color: transparent;
    margin-top: 10px;
    margin-bottom: 10px;
    margin-right: 8px;
    margin-left: 8px;
}

#taskbar button{
    box-shadow: rgba(0, 0, 0, 0.5) 0 -3 5 5px;
    /* text-shadow: 0 0 2px rgba(0, 0, 0, 0.8); */
    background-color: rgb(237, 196, 147);
    margin-right: 8px;
    padding-top: 4px;
    padding-bottom: 2px;
    padding-right: 10px;
    padding-left: 10px;
    font-weight: bolder;
    color: 	#ededed ;
    border: none;
    border-bottom: 6px solid rgb(193, 146, 103);
    border-radius: 15px;
}

#taskbar button.minimized{
    box-shadow: rgba(0, 0, 0, 0.5) 0 -3 5 5px;
    background-color: rgb(146, 140, 151);
    margin-right: 8px;
    padding-top: 4px;
    padding-bottom: 2px;
    padding-right: 10px;
    padding-left: 10px;
    font-weight: bolder;
    color: 	#cba6f7 ;
    border: none;
    border-bottom: 6px solid rgb(98, 97, 99);
    border-radius: 15px;
}

#taskbar button.urgent{
    box-shadow: rgba(0, 0, 0, 0.5) 0 -3 5 5px;
    background-color: rgb(238, 92, 92);
    margin-right: 8px;
    padding-top: 4px;
    padding-bottom: 2px;
    padding-right: 10px;
    padding-left: 10px;
    font-weight: bolder;
    color: 	#cba6f7 ;
    border: none;
    border-bottom: 6px solid rgb(183, 63, 63);
    border-radius: 15px;
}

#taskbar button.active{
    box-shadow: rgba(0, 0, 0, 0.5) 0 -3 5 5px;
    background-color: rgb(186, 238, 225);
    margin-right: 8px;
    padding-top: 4px;
    padding-bottom: 2px;
    padding-right: 10px;
    padding-left: 10px;
    font-weight: bolder;
    color: 	#cba6f7 ;
    border: none;
    border-bottom: 6px solid rgb(131, 184, 171);
    border-radius: 15px;
}
```
</details>




## Frequently Asked Questions
- some config not work, or the plugin not work.
```
if you use install hycov at first time,please try logout and relogin again.
```

- The numbers on the waybar are confused

```
1.Please pull the latest waybar source code compilation,
this issue has been fixed in the waybar project, fix date (2023-10-27)

2.Change the {id} field in hyprland/workspace field to {name}
```

- Compilation failure
```
Please pull the latest hyprland source code to compile and install. The plugin relies on a hyprland pr,pr submission date (2023-10-21)
```

- Unable to load
```
Check whether hyprland has been updated, and if so, please recompile hyprcov
```
