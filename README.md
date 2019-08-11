# sudo.exe
The popular Unix/Linux command sudo for Windows - well, sort of.

![some picture, if not visible just ignore](https://i.imgur.com/DQNtMJi.png)

"sudo" command (do as super-user) for Windows' command prompt. Usually, when using a command line utility which needs elevated privileges, you'll need to open a new instance of the command processor via the system menu. There are also tools which make this process easier, but these usually still open a new command processor window, which means you have to re-type the command (or try to copy it, which is comparable horrible) instead of just using the "up" key and adding the "sudo" command, as one does on Linux.
This little program wants to help. It works pretty much just as Linux'es sudo:
```
sudo <program_to_execute>
```
will launch <program_to_execute> in the same command processor, but with elevated rights. After the command terminates, the next command will be executed under "normal" rights again. The output of the executed program will remain visible. Instead of being asked for your password, the "Confirm Admin rights" window will pop up.

# Binaries ("Download") / Installation
Here you go: https://ci.appveyor.com/project/hst125fan/sudo-exe/build/artifacts. For better accessibility, move the executable to some location which is referenced in your PATH system variable. As this program does some stuff with elevated rights, some anti-virus programs moan about it. Don't trust me (or the CI)? Read the source code and build it yourself ... see below.

# Build
Pretty straightforward, use VS2017 Community, open the contained soulution file and compile it. 

# Limitations
* Some long outputs or especially pre-formatted outputs which are created with direct access to the command handles (such as "dir") do not work properly, their output appears truncated.
* The program can aswell be used from a bash-port (MinGW, cygwin), but will not be able to access Linux system commands provided within these command processors (such as ls, rm, mv).  
* ``` sudo sudo ``` will not work (obviously) 
  
