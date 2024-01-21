# hych
Window minimization plugin for hyprland,Enhancements to special workspaces.

This plugin some feature(click waybar icon) is not yet available for latest V0.34 version of hyprland,This bug is being fixed, refer to issue:[https://github.com/DreamMaoMao/hych/issues/1]


### what can it do:

- Support remember the workspace which the window was in

- Support restore minimize window by 'alt-tab' mode like switch window in "gnome alt-tab" 

- Support minimize window and restore window by clicking the icon on the waybar

- Support customize the minimization status on the waybar

- Support for minimize buttons in app

- Support use shortcut keys to Ergodic restore in the normal Workspace(without select)

This plugin uses the special workspace built into hyprland to hide windows, aiming to enhance the functionality of the special workspace, so it can also be compatible with the previous special workspace operations.

For example, you can restore the windows that you put into the special workspace by the old way by clicking the waybar icon or using the shortcut keys of this plugin to the normal workspace.


## by click waybar icon or app minimize button

https://github.com/DreamMaoMao/hych/assets/30348075/32162394-963b-4eeb-9608-03da36c0acfd

## alt-tab select mode

https://github.com/DreamMaoMao/hych/assets/30348075/c42c38db-8d21-4ca9-886c-5e21a4c14864


## no select mode

https://github.com/DreamMaoMao/hych/assets/30348075/60799d58-190e-4e19-8305-0cbd12b2be36




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

bind=ALT,i,hych:minimize # minimize window
bind=ALT_SHIFT,i,hych:restore_minimize #manual restore window
bind = ALT, w, hych:toggle_restore_window # restore by alt-tab mode

plugin {
    hych {
        enable_alt_release_exit = 1 # alt swith mode,see readme for detail
        alt_replace_key = code:64 # alt swith mode,see readme for detail
    }
}
```

## click waybar icon
- when the window is not active, clicking the waybar icon button will activate the window 
- when the window is active, clicking the icon button will minimize the window
- when the window has been minimized, clicking the icon button will restore the window

## alt-tab mode

such as bind to `alt + w`:

- 1.`alt + w` will enter special workspace when you not in special workspace(please hold alt,don't make it release)

- 2.`alt + w` will switch window focus circularly when you in special workspace. (please hold alt,don't make it release)

- 3.when you release `alt` , it will auto exit special workspace and restore the select window.

<details>
<summary> If you don't want to use `alt` as MainKey in alt mode</summary>

such as use `super` to repalce `alt`
- 1.bind toggleoverview
```
bind = SUPER,w,hych:toggle_restore_window
```
- 2.use `alt_replace_key` to specify what is the detection key on release.
```
# use keyname
alt_replace_key = Super_L # Alt_L,Alt_R,Super_L,Super_R,Control_L,Control_R,Shift_L,Shift_R

# use keycode
alt_replace_key = code:133 # use `xev` command to get keycode
```

</details>

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
