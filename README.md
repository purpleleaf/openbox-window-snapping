# openbox-window-snapping
Window snapping script for openbox. Multiple Monitors.

**Setup** Sorry, it's kind of a lot.

You need run the script when dragging a window. You can do this by adding the following to "~/config/openbox/rc.xml", or whatever the name of the file is.
```
<action name="Execute">
    <command>/home/$USER$/scripts/winDragDrog.sh</command>
</action>
````

I would suggest you add it to the file like this
```
<mouse>
    ...
    <context name="Titlebar">
        ...
        <mousebind button="Left" action="Drag">
            <action name="Execute">
                <command>/home/$USER$/scripts/winDragDrog.sh</command>
            </action>
            <action name="Move"/>
        </mousebind>
        ...
    </context>
    ...
</mouse>
```
You can change the name and the script location to your liking. Of course replace $USER$ with your username.


You will need to set your mouse, snap margins, and how big your titlebars are.
At the top of the script there are three variables, mouse, diff, and titleSize.

Run `xinput | grep pointer`
In the scirpt replace G604(my mouse) with whichever mouse is your main mouse. You can use id or mouse name, CAPS MATTER. Yours will look like `mouse='MOUSE_NAME'`. Make sure your mouse name is surrounded by quotes.

For the variable diff, replace 40 with a number of pixels you wish to be screen snapping borders. No quotes.

titleSize is how big you window titlebars are. Becasue they do not count as window size we must exlucde the size of titles from the size of the window. This will chnage depending on the font size of your titlebar and/or the icon size of your titlebar. The best way to figure out what works for you is to just play with it. A higher number will result in a smaller window vertically. I have not accounted for window left, right, and bottom borders so there may be window overlapping becasue of this.

Make sure the file is executable! `chmod +x winDragDrog.sh`

**Requirments**
```
xinput
xdotool
wmctrl
```
  
**Notes**

- This is currently laggy, and will lag your desktop.
- If have any issues PLEASE tell me, I'm new to releasing projects. While this isn't a big project I still want to be better.


**ToDO**

- Write this in python for better performace. Grep is apparently really slow.
- Auto select/use mouse/mice
- Auto determine title size
- Create terminal commands to set options
