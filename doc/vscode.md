# Setup Visual Studio Code to work with the ctrlX AUTOMATION SDK

As source code editor we recommend to use Visual Studio Code (VSCode).

This chapter describes how to install VSCode and required extensions.

## Install Visual Studio Code

The installation of Visual Studio Code is described here [https://code.visualstudio.com](https://code.visualstudio.com).

!!! important
	VSCode is installed on your workstation computer. 
	
## Install Visual Studio Code Extensions

VSCode is a framework with a lot of plugins - so called extensions. A set of extensions has to be installed to work with the ctrlX AUTOMATION SDK. 

How to install extensions is described here [https://code.visualstudio.com/docs/editor/extension-marketplace](https://code.visualstudio.com/docs/editor/extension-marketplace)

!!! important
	Extension can be installed LOCAL (=on your working computer) and/or REMOTE (=on your remote virtual/real machine). 

### Extensions for Remote Connections

If your workstation computer and the your SDK development environment is NOT the same you have to install extension to establish remote connections. Therefor to install these extensions __LOCAL__:

- Remote SSH if you use QEMU or VB VMs
- Remote WSL if you use WSL

### Extensions to Work with the SDK Development Environment 

In order to work with the sample projects of the SDK you will need to install additional extensions __REMOTE__. 
This can be done easily from within VS Code as soon as you open a project folder. 

Which extension you need depends from the kind of project you are working with. VSCode scans the files in the folder and makes proposals.

!!! important
	Click the field __Install in SSH__ respectively __Install in WSL__

Here's an example

![vscode_remote_extensions](./images/vscode_remote_extensions.png)
