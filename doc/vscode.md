# Setup Visual Studio Code to work with the ctrlX AUTOMATION SDK

As Integrated Development Environment (IDE) we recommend to use Visual Studio Code.

So-called "extensions" expand the functionality of this IDE, so that you have all the functions available that are required for the development of ctrlX applications.

This chapter describes how to install Visual Studio Code and required extensions.

## Install Visual Studio Code

The installation of Visual Studio Code is described here [https://code.visualstudio.com](https://code.visualstudio.com).

!!! important
	Visual Studio Code is installed on your workstation computer.

## Install Visual Studio Code Extensions

Visual Studio Code is a framework with a lot of plugins - so called extensions. A set of extensions has to be installed to work with the ctrlX AUTOMATION SDK.

How to install extensions is described here [https://code.visualstudio.com/docs/editor/extension-marketplace](https://code.visualstudio.com/docs/editor/extension-marketplace)

!!! important
	An extension can be installed LOCAL (=on your workstation computer) and/or REMOTE (=on your remote virtual/real machine).

### Extensions for Remote Connections

If your workstation computer and your App Build Environment is NOT the same, you have to install the 'Remote SSH' extension to establish remote connections (SSH). This extension has to be installed __LOCAL__.

### Extensions to Work with the SDK Development Environment

In order to work with the sample projects of the SDK you will need to install additional extensions __into your build environment__.
This can be done easily from within Visual Studio Code as soon as you open a project folder.

Which extension you need depends from the kind of project you are working with. Visual Studio Code scans the files in the folder and makes proposals.

!!! important
	If your build environment is not your workstation computer select the "Install in ..." option.

Here's an example: Install in SSH

![vscode_remote_extensions](./images/vscode_remote_extensions.png)
